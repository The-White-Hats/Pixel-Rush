; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt
.MODEL SMALL
.STACK 64
.DATA   
    TEXT_COLOR equ 0    ; Black text on white background
    SCREEN_ATTR equ 020h ; White background (high nibble) and black text (low nibble)
    DASHEDLINEATTR equ 0F0h ; 

    user1_posX dw 10 ; Position X
    user1_posY dw 10 ; Position Y
    user1_dir_bools db 7 dup(0) ; up, right, down, left
			db 0 ; margin
		user1_dir_arr db 48h, 4dh, 50H, 4BH
    prev_user1_posX dw 0
		prev_user1_posY dw 0

    user2_posX dw 30 ; Position X
    user2_posY dw 10 ; Position Y
    user2_dir_bools db 7 dup(0) ; up, right, down, left
			dw 0 ; margin
		user2_dir_arr db 11h, 20h, 1fH, 1eH
    prev_user2_posX dw 0
		prev_user2_posY dw 0
			db 0
		killSignal db 0
			dw 0
		origInt9Offset dw 0
    origInt9Segment dw 0
.CODE

include car_m.inc

my_isr PROC
		in al, 60H ; put the scan code of the pressed or unpressed

		cmp al, 1h ; pressing the esc key
    jz midKill

		lea si, user1_dir_arr
		lea di, user1_dir_bools
		call CheckDir

		lea si, user2_dir_arr
		lea di, user2_dir_bools
		call CheckDir

		jmp dontKill

		midKill:
		mov al, 0ffH
		mov killSignal, al		          ; Call DOS interrupt to exit

		dontKill:
		mov  al, 20h           ; The non specific EOI (End Of Interrupt)
    out  20h, al
    iret
my_isr endp 

MAIN 	PROC FAR
    MOV AX , @DATA
    MOV DS , AX  
    clear

		; ---------------------------------------override int 9h----------------------------------------------;

    ; Disable interrupts
    CLI
		; Save the original interrupt vector for int 9h
    mov ax, 3509h
    int 21h
    mov origInt9Offset, bx
    mov origInt9Segment, es

		push ds
		mov ax, cs
		mov ds, ax
    ; Change the interrupt vector for int 9h
    mov ax, 2509h
		lea dx, my_isr
		int 21h
    ; Re-enable interrupts
		pop ds
    STI

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
		mov cx,user1_posX 
		mov dx,user1_posY
		mov al,SCREEN_ATTR
		mov ah,0ch
		int 10h

    ; draw the second user
    mov cx,user2_posX
		mov dx,user2_posY
		int 10h

    ; ------------------this loop is like while(true) until the user press esc to exit the program---------;
		again:

		mov ax, 8600H ; AH = 86h (Delay function), AL = 00h (not used)
		xor cx, cx ; CH = high order byte of delay count, CL = not used
		mov dx, 0F0FFH ; DL = low order byte of delay count, DH = not used
		int 15H ; Call BIOS delay function

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
		jz label1 ; jump away if there is no change

		update1:
		clear_prev_location prev_user1_posX, prev_user1_posY
		; draw the first user        
		mov cx,user1_posX 
		mov dx,user1_posY
		mov al,SCREEN_ATTR
		mov ah,0ch
		int 10h

		label1:
    ; update the second user location
		; copy the current postions into prev_postions
		mov ax, user2_posX
		mov prev_user2_posX, ax
		mov ax, user2_posY
		mov prev_user2_posY, ax
    update_user2_pos

		; check if there is a change or not
		mov ax, prev_user2_posX
		cmp ax, user2_posX
		jnz update2 ; jump to the update if there is a change
		mov ax, prev_user2_posY
		cmp ax, user2_posY
		jz label2 ; jump away if there is no change

		update2:
		clear_prev_location prev_user2_posX, prev_user2_posY

		; draw the second user
    mov cx,user2_posX
		mov dx,user2_posY
		mov al,SCREEN_ATTR
		mov ah,0ch
		int 10h
		
		label2:

		cmp killSignal, 0H
		jnz kill

    jmp again

		kill:
		; Restore the original interrupt vector for int 9h
    CLI
    mov ax, origInt9Segment
    mov dx, origInt9Offset
    
    push ds
    mov ds, ax

    mov ax, 2509h
    int 21h
    ; Re-enable interrupts
    pop ds
    STI

    MOV AH, 4CH         ; Function to exit program
    INT 21H
MAIN ENDP
END MAIN