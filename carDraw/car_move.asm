; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt
.MODEL SMALL
.STACK 64
.DATA

    TEXT_COLOR      equ 0                                                ; Black text on white background
    SCREEN_ATTR     equ 020h                                             ; White background (high nibble) and black text (low nibble)
    DASHEDLINEATTR  equ 0F0h

    BUFF_SIZE       equ 40*23

    CAR_HEIGHT      equ 40
    CAR_WIDTH       equ 23

    SCREEN_WIDTH    equ 320
    SCREEN_HEIGHT   equ 200


    carFile         db  'car5.bin', 0
    errorMsg        db  "Something went wrong with files !!", 10, "$"
    
    car             db  BUFF_SIZE dup(?)

                    dt  ?

    user1_posX      dw  10                                               ; Position X
    user1_posY      dw  10                                               ; Position Y
    user1_dir_bools db  7 dup(0)                                         ; up, right, down, left
                    db  0                                                ; margin
    user1_dir_arr   db  48h, 4dh, 50H, 4BH
    prev_user1_posX dw  0
    prev_user1_posY dw  0
                    db  0
    killSignal      db  0
                    dw  0
    origInt9Offset  dw  0
    origInt9Segment dw  0
.CODE

              include             car_m.inc
              include             macros.inc

my_isr PROC
              in                  al, 60H                                                                             ; put the scan code of the pressed or unpressed

              cmp                 al, 1h                                                                              ; pressing the esc key
              jz                  midKill

              lea                 si, user1_dir_arr
              lea                 di, user1_dir_bools
              call                CheckDir

              jmp                 dontKill

    midKill:  
              mov                 al, 0ffH
              mov                 killSignal, al                                                                      ; Call DOS interrupt to exit

    dontKill: 
              mov                 al, 20h                                                                             ; The non specific EOI (End Of Interrupt)
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
              mov                 AX , 1003h
              mov                 BL , 00h                                                                            ; 00h background intensity enabled , 01h blink enabled
              mov                 BH , 00h                                                                            ; to avoid problems on some adapters
              int                 10h
		
    ;---------------------------------------Screen Coloring------------------------------------------------
    ; paint the screen in white
              mov                 ax, 0600h
              mov                 bh, 0fh
              mov                 cx, 0h
              mov                 dx, 184fh
              int                 10h
					    
    ;-----------------------------------------Input file---------------------------------------------------
              mov                 dx, offset carFile
              call                inputFile
	
    ;------------------------------------------------------------------------------------------------;
              xor                 cx,cx
              xor                 dx,dx

    ; ------------------------------draw the intial position of the players--------------------------;
    ; draw the first user
              mov                 cx,user1_posX
              mov                 dx,user1_posY
              mov                 al,SCREEN_ATTR
              mov                 ah,0ch
              int                 10h

    ; ------------------this loop is like while(true) until the user press esc to exit the program---------;
    again:    

              mov                 ax, 8600H                                                                           ; AH = 86h (Delay function), AL = 00h (not used)
              xor                 cx, cx                                                                              ; CH = high order byte of delay count, CL = not used
              mov                 dx, 0F0FFH                                                                          ; DL = low order byte of delay count, DH = not used
              int                 15H                                                                                 ; Call BIOS delay function

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
              jnz                 update1                                                                             ; jump to the update if there is a change
              mov                 ax, prev_user1_posY
              cmp                 ax, user1_posY
              jz                  done_all                                                                            ; jump away if there is no change

    update1:  
              clear_prev_location prev_user1_posX, prev_user1_posY
    ; draw the first user
              mov                 cx,user1_posX
              mov                 dx,user1_posY
              mov                 al,SCREEN_ATTR
              mov                 ah,0ch
              int                 10h

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

              MOV                 AH, 4CH                                                                             ; Function to exit program
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

drawCar PROC

    ; Draw background
              mov                 di,0
              mov                 cx,64000
              mov                 al,00h

              rep                 stosb

              mov                 di, SCREEN_WIDTH*(SCREEN_HEIGHT/2 - CAR_HEIGHT/2) + SCREEN_WIDTH/2 - CAR_WIDTH/2    ; starting pixel of screen
              mov                 si, offset car                                                                      ; starting byte of car

              mov                 cx, CAR_HEIGHT                                                                      ; number of lines to draw

    outerLoop:

              mov                 dx, CAR_WIDTH                                                                       ; number of pixels to draw

    innerLoop:

              cmp                 byte ptr [si], 250
              jz                  skip

              movsb                                                                                                   ; copy byte from si to di (draw pixel)
              jmp                 continue

    skip:     
              inc                 si
              inc                 di

    continue: 
              dec                 dx

              jnz                 innerLoop

              add                 di, SCREEN_WIDTH - CAR_WIDTH                                                        ; move to next line

              loop                outerLoop

              ret

drawCar ENDP

END MAIN