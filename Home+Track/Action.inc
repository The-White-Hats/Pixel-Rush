;description
GetAction PROC
    pusha
    GetAction_again:      
			;------------------------move the cursor to the pressed key message location ---------------------------------;

 

			MOV AH, 00H         ;get the pressed key scan code and put it in ah        
			INT 16H  
            
			mov user_lastaction,ah  ; save the pressed key scan code in user_lastaction variable

  
			cmp user_lastaction,01h          ; compare with ESC scan code  
			jnz checkF1
			showColoredMsg closeprogram, TEXT_COLOR, closeprogramsize, start_X_ofpress, start_Y_ofpress     
			jmp exit
			
			checkF1:
			cmp user_lastaction,3bh         ; compare with F1 scan code 
			jnz checkF2 
			showColoredMsg f1, TEXT_COLOR, f1size, start_X_ofpress, start_Y_ofpress 
			jmp finish 
			
			checkF2:
			cmp user_lastaction,3ch        ; compare with F2 scan code 
			jnz not_pressed
			showColoredMsg f2, TEXT_COLOR, f2size, start_X_ofpress, start_Y_ofpress 
			call GameMain
			jmp finish 
			
			not_pressed:      ; esc,f1,f2 not pressed
			showColoredMsg none, TEXT_COLOR, nonesize, start_X_ofpress, start_Y_ofpress 
			
			finish: 
			
			jmp GetAction_again  ;jmp to again listen again for the next instruction
        
        exit:
        popa
    ret
GetAction ENDP