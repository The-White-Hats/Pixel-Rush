
; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt
include mymacro.inc
	.MODEL SMALL
	.STACK 64
	.DATA   
		 	TEXT_COLOR equ 0    ; Black text on white background
      SCREEN_ATTR equ 020h ; White background (high nibble) and black text (low nibble)
      DASHEDLINEATTR equ 0F0h ; 

			user1_posX db 10 ; Position X
			user1_posY db 10 ; Position Y 
			prev_user1_posX db 10 ; previous Position X
			prev_user1_posY db 10 ; previous Position Y 

			user2_posX db 30 ; Position X
			user2_posY db 10 ; Position Y 
			prev_user2_posX db 30 ;  previous Position X
			prev_user2_posY db 10 ; previous Position Y         
	

			firstuser_lastaction db ?
			seconduser_lastaction db ?

      str db ?
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

    ; this loop is like while(true) until the user press esc to exit the program
		again:      
		;------------------------move the cursor to the pressed key message location ---------------------------------;

		; draw the first user        
		mov cl,user1_posX 
		mov dl,user1_posY
		mov al,SCREEN_ATTR
		mov ah,0ch
		int 10h
        
			 
  	mov prev_user1_posX,cl
		mov prev_user1_posY,dl

		; draw the second user
    mov cl,user2_posX
		mov dl,user2_posY
		mov al,SCREEN_ATTR
		mov ah,0ch
		int 10h

    mov prev_user2_posX,cl
		mov prev_user2_posY,dl

		;------------------------------------------------checking key pressing--------------------------------------      
    MOV AH, 01H   ; Function 01H of INT 16H - Check if a key is pressed
    INT 16H       ; Check keyboard status
            
		JZ NoKeyPress

		; wait for pressing a key
		MOV AH, 00H     ; Function 02H of INT 16H - Check keystroke 
		INT 16H 
					
		mov firstuser_lastaction,al

		mov seconduser_lastaction,ah
		jmp pressed
							
		NoKeyPress:
		mov firstuser_lastaction,0
		mov seconduser_lastaction,0	

		pressed:
					
		handel_firstuser_action
		
		handel_seconduser_action

				
		jmp again  ;jmp to again listen again for the next instruction
        

    exit:
    MOV AH, 4CH         ; Function to exit program
    INT 21H             ; Call DOS interrupt to exit

MAIN ENDP
END MAIN