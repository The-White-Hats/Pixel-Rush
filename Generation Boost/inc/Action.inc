PrintVarLenNum MACRO num
 local l1
 local l2
 mov ax,0
  MOv Al, num
  MOv DX, 0
  MOV BX, 10
  MOv CX, 0
  L1:
  DIV BX
  PUSH DX
  MOV DX, 0
  INC CX
  CMP AX, 0
  JNE L1
  MOV AH, 2
  L2:
  POP DX
  ADD DX, 48
  INT 21H
  LOOP L2
ENDM

WinnerScreen PROC
    pusha
	paintAreaM 168d, 323d, 304d, 54d, GOLD  ; paint input area in black
    paintAreaM 170d, 325d, 300d, 50d, 0H  ; paint input area in black
    mov ax,0
	add al,IsUser1Win
	add al,IsUser2Win
	cmp ax,0
	jz bothlost

    showColoredMsg WinnerMessagePart1, GOLD, part1size, 21,25 ;* part1
    cmp IsUser1Win,1
    jnz user2isthewinner
	mov ax,0
	mov al,name1Size
    showColoredMsg user1name, GOLD,ax, 21, 26+part1size ;* username
    jmp WinnerScreen_part2
    user2isthewinner:
	mov ax,0
	mov al,name2Size
    showColoredMsg user2name, GOLD,ax,21, 26+part1size ;* username
    WinnerScreen_part2:
    showColoredMsg WinnerMessagePart2, GOLD, part2size, 22, 35 ;* part2
    jmp WinnerScreen_drawscores
	bothlost:
   
    showColoredMsg BothLostMes, RED, BothLostMes1size, 22,31 ;* part1

    WinnerScreen_drawscores:

	call Wait_Sec

	paintAreaM 170d, 325d, 300d, 50d, 0H  ; paint input area in black
    
	mov ax,0
	mov al,name1Size
    showColoredMsg user1name, GOLD,ax, 21, 25 ;* username

	showColoredMsg score, GOLD,scoreSize, 21, 40 ;* username

    popa
	MoveCursor 40+scoreSize,21
	PrintVarLenNum user1score
	pusha
    
	mov ax,0
	mov al,name2Size
    showColoredMsg user2name, GOLD,ax,22, 25 ;* username

	showColoredMsg score, GOLD,scoreSize, 22, 40 ;* username

	popa
	MoveCursor 40+scoreSize,22
	PrintVarLenNum user2score
	pusha

   	call Wait_Sec

    popa
    ret
WinnerScreen ENDP

GetAction PROC
    pusha
    GetAction_again:      
			;------------------------move the cursor to the pressed key message location ---------------------------------;


			MOV AH, 00H         ;get the pressed key scan code and put it in ah        
			INT 16H    
			mov user_lastaction,ah  ; save the pressed key scan code in user_lastaction variable

			;-------------------------------------------------------------------------------------------------------------;
			cmp user_lastaction,01h          ; compare with ESC scan code  
			jnz checkF1
			showColoredMsg closeprogram, TEXT_COLOR, closeprogramsize, start_Y_ofpress, start_X_ofpress     
			jmp exit
			

			;-------------------------------------------------------------------------------------------------------------;
			checkF1:
			cmp user_lastaction,3bh         ; compare with F1 scan code 
			jnz mid_jump_to_checkF2 
			showColoredMsg f1, TEXT_COLOR, f1size, start_Y_ofpress, start_X_ofpress 
			pusha
			call chatRoom
			popa
			; restore all the settings
			MOV AX,@DATA
			MOV DS,AX
			mov es, ax
			UltraGraphicsMode
			putWallPaperM 14, 10, 0, 0

			jmp dont_mid_jump_to_checkF2
			mid_jump_to_checkF2: jmp checkF2
			dont_mid_jump_to_checkF2:

			drawLogo
			paintAreaM 0, 430d, 640d, 50d, 0H
			drawButtons

			jmp finish 
			

			;-------------------------------------------------------------------------------------------------------------;
			checkF2:
			cmp user_lastaction,3ch        ; compare with F2 scan code 
			jnz tonot_pressed
			jmp GetAction_bridge
			    tonot_pressed:
                jmp not_pressed
            GetAction_bridge:
			showColoredMsg f2, TEXT_COLOR, f2size, start_Y_ofpress, start_X_ofpress 
			pusha
			call GameMain
			popa
			MOV AX,@DATA
			MOV DS,AX
			mov es, ax
			UltraGraphicsMode
            
			putWallPaperM 14, 10, 0, 0

			drawLogo
			call WinnerScreen

			mov user1score, 0
			mov user2score, 0
			mov IsUser1Win, 0H
			mov IsUser2Win, 0H
			
			
			putWallPaperM 7, 2, 3*bgSize, 6*bgSize  ; recover the area with the wallpaper
			paintAreaM 0, 430d, 640d, 50d, 0H
			drawButtons
             
			jmp finish 
			

			;-------------------------------------------------------------------------------------------------------------;
			not_pressed:      ; esc,f1,f2 not pressed
			showColoredMsg none, TEXT_COLOR, nonesize, start_Y_ofpress, start_X_ofpress 
			
			finish: 
			jmp GetAction_again  ;jmp to again listen again for the next instruction
        
			exit:
			popa
    ret
GetAction ENDP


Wait_Sec PROC
	pusha
	mov ah, 2Ch
	int 21H    ; puts the millseconds in dl
	add dh,waittime
	mov al, dh ; contain hundreds of seconds
	mov bl,60
	mov ah,0
	xor dx,dx
	div bl
    mov goaltime,ah

    wait_loop:
	mov ah, 2Ch
	int 21H    ; puts the millseconds in dl
	mov al, dh ; contain hundreds of seconds
	mov bl,60
	mov ah,0
	xor dx,dx
	div bl
	cmp ah,goaltime
	jnz wait_loop
	popa
	ret
Wait_Sec ENDP

