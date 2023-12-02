
endlst macro  start     ; give it the start of the vertical cordinate (X-axis)
    ; newline
    mov ah,02h
    mov dl,10
    int 21h
      
    mov ah,3
  	mov bh,0h
  	int 10h
  	   
	mov ah,2
  	mov dl,start
  	int 10h 
  	
endm endlst
  
endl macro 
       mov ah,02h
    mov dl,10
    int 21h 
    mov ah,02h
    mov dl,13
    int 21h
endm endl  

clear macro 
    mov ah, 0       ; AH=0 is the function for setting video mode
    mov al, 13h       ; AL=3 sets 80x25 text mode (standard text mode)
    int 10h         ; BIOS interrupt for video services
endm clear 

clearline macro
    mov ah,9
    mov bh,0
    mov al, 0h
    mov cx,79
    mov bl,SCREEN_ATTR
    int 10h
endm cearline  

handel_firstuser_action macro

            cmp firstuser_lastaction,'w'          ; compare with ESC scan code  
			jnz checkS
			dec user1_posY
			jmp user1_finish
			
			checkS:
			cmp firstuser_lastaction,'s'         ; compare with F1 scan code 
			jnz checkA 
			inc user1_posY
			jmp user1_finish 
			
			checkA:
			cmp firstuser_lastaction,'a'        ; compare with F2 scan code 
			jnz checkD
			dec user1_posX
			jmp user1_finish 
			
			checkD:
			cmp firstuser_lastaction,'d'        ; compare with F2 scan code 
			jnz nochange1 
			inc user1_posX

            user1_finish:
			; mov cl,prev_user1_posX
			; mov dl,prev_user1_posY
			; mov al,0Fh
			; mov ah,0ch
			; int 10h

			nochange1:

endm handel_firstuser_action


handel_seconduser_action macro

          cmp seconduser_lastaction,48h          ; compare with upArrow scan code  
			jnz checkdownArrow
			dec user2_posY
			jmp user2_finish
			
			checkdownArrow:
			cmp seconduser_lastaction,50h         ; compare with downArrow scan code 
			jnz checkleftArrow 
			inc user2_posY
			jmp user2_finish 
			
			checkleftArrow:
			cmp seconduser_lastaction,4Bh        ; compare with leftArrow scan code 
			jnz checkrightArrow
			dec user2_posX
			jmp user2_finish 
			
			checkrightArrow:
			cmp seconduser_lastaction,4dh        ; compare with rightArrow scan code 
			jnz nochange2 
			inc user2_posX

            user2_finish: 

		    mov cl,prev_user2_posX
			mov dl,prev_user2_posY
			mov al,0Fh
			mov ah,0ch
			int 10h

            nochange2:

endm handel_seconduser_action


showmes macro str
    mov ah,09h
    lea dx,str
    int 21h  
endm showmes  
  
