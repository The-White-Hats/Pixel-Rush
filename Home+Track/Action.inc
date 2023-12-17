WinnerScreen PROC
    pusha
    lea si ,user1name
    mov cx,0
    getuser1namesize:
    inc cx
	inc si
    cmp [si],'a'
    jge check1please
	jmp endend2
	check1please:
	cmp [si],'z'
	jle getuser1namesize
	endend2:
    mov username1Size,cx

    lea si ,user2name
    mov cx,0
    getuser2namesize:
    inc cx
	inc si
    cmp [si],'a'
    jge check2please
	jmp endend1
	check2please:
	cmp [si],'z'
	jle getuser2namesize
	endend1:
    mov username2Size,cx

    ;clear


    showColoredMsg WinnerMessagePart1, 2bh, part1size, start_Y_ofpress-10, start_X_ofpress+25 ;* part1
    cmp IsUser1Win,1
    jnz user2isthewinner
    showColoredMsg user1name, 2bh,username1Size, start_Y_ofpress-10, start_X_ofpress+26+part1size ;* username
    jmp WinnerScreen_part2
    user2isthewinner:
    showColoredMsg user2name, 2bh,username2Size, start_Y_ofpress-10, start_X_ofpress+26+part1size ;* username
    WinnerScreen_part2:
    showColoredMsg WinnerMessagePart2, 2bh, part2size, start_Y_ofpress-10+2, start_X_ofpress+5+25 ;* part2
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

  
			cmp user_lastaction,01h          ; compare with ESC scan code  
			jnz checkF1
			showColoredMsg closeprogram, TEXT_COLOR, closeprogramsize, start_Y_ofpress, start_X_ofpress     
			jmp exit
			
			checkF1:
			cmp user_lastaction,3bh         ; compare with F1 scan code 
			jnz checkF2 
			showColoredMsg f1, TEXT_COLOR, f1size, start_Y_ofpress, start_X_ofpress 
			jmp finish 
			
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

			call WinnerScreen

			drawLogo

			MOV AH, 0
			INT 16h

			MOV AH, 0
			INT 16h

			drawButtons
             
			jmp finish 
			
			not_pressed:      ; esc,f1,f2 not pressed
			showColoredMsg none, TEXT_COLOR, nonesize, start_Y_ofpress, start_X_ofpress 
			
			finish: 
			
			jmp GetAction_again  ;jmp to again listen again for the next instruction
        
        exit:
        popa
    ret
GetAction ENDP