
; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt
include car_macro.inc
.386
.MODEL SMALL
.STACK 64
.DATA   
    TEXT_COLOR equ 0    ; Black text on white background
    SCREEN_ATTR equ 020h ; White background (high nibble) and black text (low nibble)
    DASHEDLINEATTR equ 0F0h ; 

    user1_posX db 10 ; Position X
    user1_posY db 10 ; Position Y
    user1_dir_bools db 4 dup(0) ; up, right, down, left
    flag1 db 0

    user2_posX db 30 ; Position X
    user2_posY db 10 ; Position Y         
    user2_dir_bools db 4 dup(0) ; up, right, down, left
    flag2 db 0
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

    in al, 60H ; put the scan code of the pressed or unpressed










    ; clear the previous location
		mov cl,user1_posX
		mov dl,user1_posY
		mov al,0Fh
		mov ah,0ch
		int 10h

    ; update the location
    update_user1_pos

		; draw the first user        
		mov cl,user1_posX 
		mov dl,user1_posY
		mov al,SCREEN_ATTR
		mov ah,0ch
		int 10h

    ; clear the previous location
		mov cl,user2_posX
		mov dl,user2_posY
		mov al,0Fh
		mov ah,0ch
		int 10h

    ; update the second user location
    update_user2_pos

		; draw the second user
    mov cl,user2_posX
		mov dl,user2_posY
		mov al,SCREEN_ATTR
		mov ah,0ch
		int 10h


		;------------------------------------------------checking key pressing--------------------------------------      
    
    
    jmp again

    exit:
    MOV AH, 4CH         ; Function to exit program
    INT 21H             ; Call DOS interrupt to exit

MAIN ENDP
END MAIN