
; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt
include macros.inc

.MODEL SMALL
.STACK 64
.DATA 

    TEXT_COLOR equ 0    ; Black text on white background
    SCREEN_ATTR equ 020h ; White background (high nibble) and black text (low nibble)
    DASHEDLINEATTR equ 0F0h ; 

    user1_posX      dw 10 ; Position X
    user1_posY      dw 10 ; Position Y

    user1_dir_bools db 8 dup(0) ; up, right, down, left, margin
	user1_dir_arr   db 48h, 0, 4dh, 0, 50H, 0, 4BH

    prev_user1_posX dw 0
	prev_user1_posY dw 0

.CODE

    MAIN PROC FAR
        mov ax, @data
        mov ds, ax

        ; clear and Set video mode
        clear

        ; Remove Blinking from the screen and allowing to use 16 colors as background
        rmBlink
            
        ;---------------------------------------Screen Coloring------------------------------------------------
        ; paint the screen in white
            mov ax, 0600h
            mov bh, 0fh
            mov cx, 0h
            mov dx, 184fh
            int 10h
                            
        ;------------------------------------------------------------------------------------------------;  
            xor cx, cx
            xor dx, dx

        ; ------------------------------draw the intial position of the players--------------------------;
        ; draw the first user        
            mov cx, user1_posX 
            mov dx, user1_posY
            mov al, SCREEN_ATTR
            mov ah, 0ch
            int 10h

        ; ------------------this loop is like while(true) until the user press esc to exit the program---------;
            again:

                mov ah, 86h     ; AH = 86h (Delay function), AL = 00h (not used)
                xor cx, cx      ; CH = high order byte of delay count, CL = not used
                mov dx, 0F0FFH  ; DL = low order byte of delay count, DH = not used
                int 15H         ; Call BIOS delay function

                in al, 60H ; put the scan code of the pressed or unpressed

                cmp al, 1h ; pressing the esc key
                jnz midKilNot
                jmp far ptr kill

                midKilNot:
                check_user1_dir

                ; update the location
                ; copy the current postions into prev_postions
                mov ax, user1_posX
                mov prev_user1_posX, ax
                mov ax, user1_posY
                mov prev_user1_posY, ax
                update_user1_pos

                ; check if there is a change or not
                mov ax, prev_user1_posX
                cmp ax, user1_posX
                jnz update1 ; jump to the update if there is a change
                mov ax, prev_user1_posY
                cmp ax, user1_posY
                jz done ; jump away if there is no change

                update1:
                clear_prev_location prev_user1_posX, prev_user1_posY

                ; draw the first user        
                mov cx, user1_posX 
                mov dx, user1_posY
                mov al, SCREEN_ATTR
                mov ah, 0ch
                int 10h

                done:
            jmp far ptr again

        kill:
        MOV AH, 4CH         ; Function to exit program
        INT 21H             ; Call DOS interrupt to exit
    MAIN ENDP
    END MAIN