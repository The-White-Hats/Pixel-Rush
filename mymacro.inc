showmes macro str
    mov ah,09h
    lea dx,str
    int 21h  
endm showmes    

showchar macro str
    mov ah,02h
    mov dl,str
    int 21h  
endm showchar 

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

getstr macro str
    mov ah,0ah
    lea dx,str
    int 21h
endm getstr

shownum macro str
    local l1     
    local l2
     lea bx, str
     mov cx,10
     mov dx,0
     l1:
       mov dx,0
       div cx     
       add dl,30h
       mov [bx],dl
       inc bx 
       cmp ax,0
     jnz l1     
     mov [bx],'$' 
     mov cx, bx
     lea dx,str
     sub cx,dx  
     dec bx
     l2:
         mov ah,2
         mov dl,[bx]
         int 21h
         dec bx
         cmp dh,0
     loop l2      
endm shownum
  
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
			mov cl,prev_user1_posX
			mov dl,prev_user1_posY
			mov al,0Fh
			mov ah,0ch
			int 10h

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

handel_firstuser_action macro

    ; the up key for player one
    cmp al,'w'          ; compare with ESC scan code  
    jnz checkS          ; if this is not the key, go check the next one
    ; if this is the key do the following
    cmp user1_dir_bools, 00H    ; if it was not active, make it active and deactivate the down
    jnz notup1
    mov user1_dir_bools, 01H
    mov user1_dir_bools[2], 00H
    notup1:                     ; else if it was active before, make it inactive
    mov user1_dir_bools, 00H 
    ; finish the edit
    jmp user1_finish
    
    ; the down key for player one
    checkS:
    cmp al,'s'         ; compare with F1 scan code 
    jnz checkA         ; if this is not the key, go check the next one
    ; if this is the key do the following
    cmp user1_dir_bools[2], 00H    ; if it was not active, make it active and deactivate the up
    jnz notdown1
    mov user1_dir_bools[2], 01H
    mov user1_dir_bools[1], 00H
    notdown1:                     ; else if it was active before, make it inactive
    mov user1_dir_bools[2], 00H 
    ; finish the edit
    jmp user1_finish 
    
    ; the left key for player one
    checkA:
    cmp al,'a'        ; compare with F2 scan code 
    jnz checkD        ; if this is not the key, go check the next one
    ; if this is the key do the following
    cmp user1_dir_bools[3], 00H    ; if it was not active, make it active and deactivate the right
    jnz notleft1
    mov user1_dir_bools[3], 01H
    mov user1_dir_bools[1], 00H
    notleft1:                     ; else if it was active before, make it inactive
    mov user1_dir_bools[3], 00H 
    ; finish the edit
    jmp user1_finish 
    
    ; the right key for player one
    checkD:
    cmp al,'d'        ; compare with F2 scan code 
    jnz user1_finish 
    ; if this is the key do the following
    cmp user1_dir_bools[1], 00H    ; if it was not active, make it active and deactivate the left
    jnz notright1
    mov user1_dir_bools[1], 01H
    mov user1_dir_bools[3], 00H
    notright1:                     ; else if it was active before, make it inactive
    mov user1_dir_bools[1], 00H 
    ; finish the edit
    user1_finish:

endm handel_firstuser_action


handel_seconduser_action macro

    cmp al,48h          ; compare with upArrow scan code  
    jnz checkdownArrow
    ; if this is the key do the following
    cmp user2_dir_bools, 00H    ; if it was not active, make it active and deactivate the down
    jnz notup2
    mov user2_dir_bools, 01H
    mov user2_dir_bools[2], 00H
    notup2:                     ; else if it was active before, make it inactive
    mov user2_dir_bools, 00H 
    ; finish the edit
    jmp user2_finish
    
    checkdownArrow:
    cmp al,50h         ; compare with downArrow scan code 
    jnz checkleftArrow 
    ; if this is the key do the following
    cmp user2_dir_bools[2], 00H    ; if it was not active, make it active and deactivate the up
    jnz notdown2
    mov user2_dir_bools[2], 01H
    mov user2_dir_bools[1], 00H
    notdown2:                     ; else if it was active before, make it inactive
    mov user2_dir_bools[2], 00H 
    ; finish the edit
    jmp user2_finish
    
    checkleftArrow:
    cmp al,4Bh        ; compare with leftArrow scan code 
    jnz checkrightArrow
    ; if this is the key do the following
    cmp user2_dir_bools[3], 00H    ; if it was not active, make it active and deactivate the right
    jnz notleft2
    mov user2_dir_bools[3], 01H
    mov user2_dir_bools[1], 00H
    notleft2:                     ; else if it was active before, make it inactive
    mov user2_dir_bools[3], 00H 
    ; finish the edit
    jmp user2_finish  
    
    checkrightArrow:
    cmp al,4dh        ; compare with rightArrow scan code 
    jnz user2_finish 
    ; if this is the key do the following
    cmp user2_dir_bools[1], 00H    ; if it was not active, make it active and deactivate the left
    jnz notright2
    mov user2_dir_bools[1], 01H
    mov user2_dir_bools[3], 00H
    notright2:                     ; else if it was active before, make it inactive
    mov user2_dir_bools[1], 00H 
    ; finish the edit
    user2_finish:

endm handel_seconduser_action

update_user1_pos macro
    
    up1:
    cmp user1_dir_bools, 00H
    jz right1
    dec byte ptr user1_posY

    right1:
    cmp user1_dir_bools[1], 00H
    jz down1
    inc byte ptr user1_posX

    down1:
    cmp user1_dir_bools[2], 00H
    jz left1
    inc byte ptr user1_posY

    left1:
    cmp user1_dir_bools[2], 00H
    jz finish_update1
    dec byte ptr user1_posX
    
    finish_update1:

endm update_user1_pos

update_user2_pos macro
    
    up2:
    cmp user2_dir_bools, 00H
    jz right2
    dec byte ptr user2_posY

    right2:
    cmp user2_dir_bools[1], 00H
    jz down2
    inc byte ptr user2_posX

    down2:
    cmp user2_dir_bools[2], 00H
    jz left2
    inc byte ptr user2_posY

    left2:
    cmp user2_dir_bools[2], 00H
    jz finish_update2
    dec byte ptr user2_posX

    finish_update2:

endm update_user2_pos