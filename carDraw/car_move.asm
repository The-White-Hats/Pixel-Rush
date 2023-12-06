; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt
.MODEL SMALL
.STACK 64
.DATA

    TEXT_COLOR      equ 0                                                ; Black text on white background
    SCREEN_ATTR     equ 020h                                             ; White background (high nibble) and black text (low nibble)
    DASHEDLINEATTR  equ 0F0h
    BACK_GROUND     equ 0Ah

    BUFF_SIZE       equ 16*16

    CAR_HEIGHT      equ 16
    CAR_WIDTH       equ 16

    SCREEN_WIDTH    equ 320
    SCREEN_HEIGHT   equ 200

    FILE_NAME_LEN   equ 10
    ;------------Files-------------;
    carFile         db  'car10.bin', 0
                    db  'car11.bin', 0
                    db  'car12.bin', 0
                    db  'car13.bin', 0
                    db  'car14.bin', 0
                    db  'car15.bin', 0
                    db  'car16.bin', 0
                    db  'car17.bin', 0

    errorMsg        db  "Something went wrong with files !!", 10, "$"
    
    ;------------Car Directions---------;
    car             db  8 dup (BUFF_SIZE dup(?))

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
.CODE

                 include             car_m.inc
                 include             macros.inc

my_isr PROC
                 in                  al, 60H                             ; put the scan code of the pressed or unpressed

                 cmp                 al, 1h                              ; pressing the esc key
                 jz                  midKill

                 lea                 si, user1_dir_arr
                 lea                 di, user1_dir_bools
                 call                CheckDir

                 jmp                 dontKill

    midKill:     
                 mov                 al, 0ffH
                 mov                 killSignal, al                      ; Call DOS interrupt to exit

    dontKill:    
                 mov                 al, 20h                             ; The non specific EOI (End Of Interrupt)
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
    file_again:  
                 push                dx
                 push                cx
                 call                inputFile
                 pop                 cx
                 pop                 dx
                 add                 dx, FILE_NAME_LEN
                 add                 bp, BUFF_SIZE
                 inc                 cx
                 cmp                 cx, 8
                 jnz                 file_again

    ; ------------------------------draw the intial position of the players--------------------------;
    ; draw user
                 mov                 user1_posX, SCREEN_WIDTH/2
                 mov                 user1_posY, SCREEN_HEIGHT/2
                 setStartPixel
                 mov                 bx, 1                               ; to draw
                 call                drawCar

    ; ------------------this loop is like while(true) until the user press esc to exit the program---------;
    again:       
                 mov                 ax, 8600H                           ; AH = 86h (Delay function), AL = 00h (not used)
                 xor                 cx, cx                              ; CH = high order byte of delay count, CL = not used
                 mov                 dx, 0F0FFH                          ; DL = low order byte of delay count, DH = not used
                 int                 15H                                 ; Call BIOS delay function

    ; update the location
    ; copy the current postions into prev_postions
                 mov                 ax, user1_posX
                 mov                 prev_user1_posX, ax
                 mov                 ax, user1_posY
                 mov                 prev_user1_posY, ax
                 update_user1_pos

    ; check if there is a change or not
                 mov                 ax, prev_user1_posX
                 cmp                 ax, user1_posX
                 jnz                 update1                             ; jump to the update if there is a change
                 mov                 ax, prev_user1_posY
                 cmp                 ax, user1_posY
                 jz                  done_all                            ; jump away if there is no change

    update1:     
                 clear_prev_location prev_user1_posX, prev_user1_posY
    ; draw user
                 mov                 bx, 1                               ; to draw
                 setStartPixel
                 call                drawCar

    done_all:    
                 cmp                 killSignal, 0H
                 jnz                 kill
 
                 jmp                 again

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

                 MOV                 AH, 4CH                             ; Function to exit program
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
                 mov                 cx, BUFF_SIZE
                 mov                 al, current_frame
                 mov                 ah, 0
                 mul                 cx
                 add                 si, ax

                 mov                 al, BACK_GROUND

                 mov                 cx, CAR_HEIGHT                      ; number of lines to draw
    outerLoop:   

                 mov                 dx, CAR_WIDTH                       ; number of pixels to draw

    innerLoop:   
                 cmp                 byte ptr [si], 250
                 jz                  skip

                 cmp                 bx, 0                               ; 0 -> clear , 1 -> draw
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

                 add                 di, SCREEN_WIDTH - CAR_WIDTH        ; move to next line
                 loop                outerLoop

                 ret

drawCar ENDP

END MAIN