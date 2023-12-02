
; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt
include car_m.inc
.MODEL SMALL
.STACK 64
.DATA   
    TEXT_COLOR equ 0    ; Black text on white background
    SCREEN_ATTR equ 020h ; White background (high nibble) and black text (low nibble)
    DASHEDLINEATTR equ 0F0h ; 

    user1_posX db 10 ; Position X
    user1_posY db 10 ; Position Y
    user1_dir_bools db 4 dup(0) ; up, right, down, left
		user1_dir_arr db 48h, 4dh, 50H, 4BH
    prev_user1_posX db 0
		prev_user1_posY db 0

    user2_posX db 30 ; Position X
    user2_posY db 10 ; Position Y         
    user2_dir_bools db 4 dup(0) ; up, right, down, left
		user2_dir_arr db 11h, 20h, 1fH, 1eH
    prev_user2_posX db 0
		prev_user2_posY db 0
.CODE


MAIN 	PROC FAR
    MOV AX , @DATA
    MOV DS , AX  
    clear
        
		; Remove Blinking from the screen and allowing to use 16 colors as background
    mov AX , 1003h
		mov BL ,00h  ; 00h background intensity enabled , 01h blink enabled
		mov BH , 00h ; to avoid problems on some adapters
    int 10h
		
		;---------------------------------------Screen Coloring------------------------------------------------
    ; paint the screen in white
		mov ax ,0600h
  	mov bh,0fh
  	mov cx,0h
  	mov dx , 184fh
  	int 10h
					    
    ;------------------------------------------------------------------------------------------------;  
    xor cx,cx
		xor dx,dx

    ; ------------------------------draw the intial position of the players--------------------------;
    ; draw the first user        
		mov cl,user1_posX 
		mov dl,user1_posY
		mov al,SCREEN_ATTR
		mov ah,0ch
		int 10h

    ; draw the second user
    mov cl,user2_posX
		mov dl,user2_posY
		int 10h

    ; ------------------this loop is like while(true) until the user press esc to exit the program---------;
		again:

		mov ax, 8600H ; AH = 86h (Delay function), AL = 00h (not used)
		xor cx, cx ; CH = high order byte of delay count, CL = not used
		mov dx, 0F0FFH ; DL = low order byte of delay count, DH = not used
		int 15H ; Call BIOS delay function

    in al, 60H ; put the scan code of the pressed or unpressed

		cmp al, 1h ; pressing the esc key
    jz midKill
		jnz midKillnot
		midKill: jmp far ptr kill
		midKillnot:

		check_user1_dir
		check_user2_dir


    ; update the location
		mov al, user1_posX
		mov prev_user1_posX, al
		mov al, user1_posY
		mov prev_user1_posY, al
    update_user1_pos

		; check if there is a change or not
		mov al, prev_user1_posX
		cmp al, user1_posX
		jnz update1 ; jump to the update if there is a change
		mov al, prev_user1_posY
		cmp al, user1_posY
		jz label1 ; jump away if there is no change

		update1:
		clear_prev_location prev_user1_posX, prev_user1_posY
		; draw the first user        
		mov cl,user1_posX 
		mov dl,user1_posY
		mov al,SCREEN_ATTR
		mov ah,0ch
		int 10h

		label1:
    ; update the second user location
		mov al, user2_posX
		mov prev_user2_posX, al
		mov al, user2_posY
		mov prev_user2_posY, al
    update_user2_pos

		; check if there is a change or not
		mov al, prev_user2_posX
		cmp al, user2_posX
		jnz update2 ; jump to the update if there is a change
		mov al, prev_user2_posY
		cmp al, user2_posY
		jz label2 ; jump away if there is no change

		update2:
		clear_prev_location prev_user2_posX, prev_user2_posY

		; draw the second user
    mov cl,user2_posX
		mov dl,user2_posY
		mov al,SCREEN_ATTR
		mov ah,0ch
		int 10h

		label2:
    jmp again

    kill:
    MOV AH, 4CH         ; Function to exit program
    INT 21H             ; Call DOS interrupt to exit
MAIN ENDP
END MAIN