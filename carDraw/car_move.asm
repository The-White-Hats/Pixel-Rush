
.MODEL SMALL
.STACK 64
.DATA

    TEXT_COLOR      equ 0                                                ; Black text on white background
    SCREEN_ATTR     equ 020h                                             ; White background (high nibble) and black text (low nibble)
    DASHEDLINEATTR  equ 0F0h
    BACK_GROUND     equ 0Ah


    CAR_HEIGHT      equ 11
    CAR_WIDTH       equ 11

    BUFF_SIZE       equ CAR_HEIGHT * CAR_WIDTH

    CAR_OFFSET      equ 5

    SCREEN_WIDTH    equ 320
    SCREEN_HEIGHT   equ 200

    FILE_NAME_LEN   equ 10
    ;------------Files-------------;
    carFile         db  'car.bin', 0


    errorMsg        db  "Something went wrong with files !!", 10, "$"
    
    ;------------Car Directions---------;
    car             db  BUFF_SIZE dup(?)


                    dt  ?

    ; start from 0 on up and move clockwise
    current_frame   db  0
    current_dir     db  0
    
    user1_posX      dw  ?                                                ; Position X
    user1_posY      dw  ?                                                ; Position Y
    user1_dir_bools db  7 dup(0)                                         ; up, right, down, left
                    db  0                                                ; margin
    user1_dir_arr   db  48h, 4dh, 50H, 4BH
    prev_user1_posX dw  ?
    prev_user1_posY dw  ?
                    db  0
    killSignal      db  0
                    dw  0
    origInt9Offset  dw  0
    origInt9Segment dw  0
                    dt  ?
.CODE

                 include             macros.inc

my_isr PROC
                 in                  al, 60H                                       ; put the scan code of the pressed or unpressed

                 cmp                 al, 1h                                        ; pressing the esc key
                 jz                  midKill

                 lea                 si, user1_dir_arr
                 lea                 di, user1_dir_bools
                 call                CheckDir

                 jmp                 dontKill

    midKill:     
                 mov                 al, 0ffH
                 mov                 killSignal, al                                ; Call DOS interrupt to exit

    dontKill:    
                 mov                 al, 20h                                       ; The non specific EOI (End Of Interrupt)
                 out                 20h, al
                 iret
my_isr endp

MAIN PROC FAR
                 MOV                 AX , @DATA
                 MOV                 DS , AX
                 clear

    ; ---------------------------------------override int 9h----------------------------------------------;

    ; Disable interrupts
                 CLI
    ; Save the original interrupt vector for int 9h
                 mov                 ax, 3509h
                 int                 21h
                 mov                 origInt9Offset, bx
                 mov                 origInt9Segment, es

                 push                ds
                 mov                 ax, cs
                 mov                 ds, ax
    ; Change the interrupt vector for int 9h
                 mov                 ax, 2509h
                 lea                 dx, my_isr
                 int                 21h
    ; Re-enable interrupts
                 pop                 ds
                 STI

    ; Remove Blinking from the screen and allowing to use 16 colors as background
                 rmBlink
		
    ; draw on screen using video memory
                 mov                 ax, 0A000h
                 mov                 es, ax

    ;---------------------------------------Screen Coloring------------------------------------------------
    ; paint the screen in green
                 mov                 di, 0
                 mov                 cx, 64000
                 mov                 al, BACK_GROUND

                 rep                 stosb
					    
    ;-----------------------------------------Input files---------------------------------------------------
                 mov                 cx, 0
                 mov                 bp, 0
                 mov                 dx, offset carFile
                 call                inputFile
    ; ------------------------------draw the intial position of the players--------------------------;
    ; draw user
                 mov                 user1_posX, SCREEN_WIDTH/2
                 mov                 user1_posY, SCREEN_HEIGHT/2
                 setStartPixel
                 mov                 bx, 1                                         ; to draw
                 call                drawCar

    ; ------------------this loop is like while(true) until the user press esc to exit the program---------;

    kill:        
    ; Restore the original interrupt vector for int 9h
                 CLI
                 mov                 ax, origInt9Segment
                 mov                 dx, origInt9Offset
    
                 push                ds
                 mov                 ds, ax

                 mov                 ax, 2509h
                 int                 21h
    ; Re-enable interrupts
                 pop                 ds
                 STI

                 MOV                 AH, 4CH                                       ; Function to exit program
                 INT                 21H
MAIN ENDP

inputFile PROC

    ; Open file
                 openFile
                 jc                  error

    ; Read file
                 readFile            BUFF_SIZE, car
                 jc                  error

    ; Close file
                 closeFile

                 ret

    error:       
                 showMsg             errorMsg

                 ret

inputFile ENDP

    ;-----------------------


update_frame PROC

                 mov                 bl, 1
                 add_to_frame
                 cmp                 bl, current_dir
                 jz                  add_one

                 mov                 bl, 2
                 add_to_frame
                 cmp                 bl, current_dir
                 jz                  add_one

                 mov                 bl, 5
                 add_to_frame
                 cmp                 bl, current_dir
                 jz                  add_one

                 mov                 bl, 3
                 add_to_frame
                 cmp                 bl, current_dir
                 jz                  sub_one

                 mov                 bl, 6
                 add_to_frame
                 cmp                 bl, current_dir
                 jz                  sub_one

                 mov                 bl, 7
                 add_to_frame
                 cmp                 bl, current_dir
                 jz                  sub_one

                 jmp                 done_frame

    add_one:     
                 mov                 bl, 1
                 add_to_frame
                 mov                 current_frame, bl
                 jmp                 done_frame

    sub_one:     
                 mov                 bl, 7
                 add_to_frame
                 mov                 current_frame, bl

    done_frame:  
                 ret

update_frame ENDP

    ;-----------------------

drawCar PROC
                 mov                 si, offset car

    ; set the car frame


                 mov                 al, BACK_GROUND

                 mov                 cx, CAR_HEIGHT                                ; number of lines to draw
                 cmp                 bx, 0                                         ; 0 -> clear , 1 -> draw
                 jnz                 outerLoop
                 add                 cx, CAR_OFFSET*2
                 sub                 di, SCREEN_WIDTH * CAR_OFFSET + CAR_OFFSET
    outerLoop:   

                 mov                 dx, CAR_WIDTH                                 ; number of pixels to draw
                 cmp                 bx, 0                                         ; 0 -> clear , 1 -> draw
                 jnz                 innerLoop
                 add                 dx, CAR_OFFSET*2
    innerLoop:   
                 cmp                 byte ptr [si], 250
                 jz                  skip

                 cmp                 bx, 0                                         ; 0 -> clear , 1 -> draw
                 jz                  clear_car

    draw_car:    
                 movsb
                 jmp                 continue
              
    clear_car:   
                 stosb
                 jmp                 continue

    skip:        
                 inc                 si
                 inc                 di

    continue:    
                 dec                 dx
                 jnz                 innerLoop

                 add                 di, SCREEN_WIDTH - CAR_WIDTH                  ; move to next line
                 cmp                 bx, 0                                         ; 0 -> clear , 1 -> draw
                 jnz                 done_car
                 sub                 di, CAR_OFFSET*2
    done_car:    
                 loop                outerLoop

                 ret

drawCar ENDP

END MAIN