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
	
	
	sendRecNames:
	mov ch, 17
	mov cl, 17

	lea si, user1name
	lea di, user2name

	mov dx , 3FDH		; Line Status Register
	in al , dx 
	AND al , 1
	jnz rec

	mov al, [si]
	mov dx, 03f8h
	out dx, al
	inc si
	dec ch

	mov dx, 3fdh
	chk: in al, dx
	and al, 1
	jz chk
	jmp rec

	recLoop:
		; test ch, cl
		; jz GetAction_again
		cmp cl, 0
		jnz continueRec
		cmp ch, 0
		jz GetAction_again
		continueRec:
		mov dx , 3FDH		; Line Status Register
		in al , dx 
		AND al , 1
		JZ checkStartUpchecksend1
		; get what I recieved
		rec:
		cmp cl, 0
		jz checkStartUpchecksend1
		mov dx , 03F8H
		in al , dx 
		mov [di] , al
		; showchar '1'
		inc di
		dec cl

		checkStartUpchecksend1:
		;Check that Transmitter Holding Register is Empty
		mov dx , 3FDH		; Line Status Register
		In al , dx 			;Read Line Status
		AND al , 00100000b
		jz recLoop
		cmp ch, 0
		jz recLoop
		mov al, [si]
		mov dx, 03f8h
		out dx, al
		inc si
		dec ch

		jmp recLoop


    GetAction_again:      
			;------------------------move the cursor to the pressed key message location ---------------------------------;

			checkreceive:
			;Check that Data Ready ; check that I recieved something
			mov dx , 3FDH		; Line Status Register
			in al , dx 
			AND al , 1
			JZ checksendBridg
			; get what I recieved
			mov dx , 03F8H
			in al , dx 
			mov RecievedValue , al
			;-----------------------------------check what was sent to me
			; first check if it was termination code
			cmp RecievedValue, 01h
			jnz not_exit
			jmp exit
			not_exit:
			; show something
			cmp RecievedValue, f1SC
			jnz not_f1SC
			
			mov ax, 0
			mov al, name2Size
			showColoredMsg user2name, TEXT_COLOR, ax, start_Y_ofpress, start_X_ofpress
			mov al, start_X_ofpress
			add al, name2Size
			inc al
			mov maloshlazma, al
			showColoredMsg f1RecievedMsg, TEXT_COLOR, f1RecievedMsgSize, start_Y_ofpress, maloshlazma
			not_f1SC:
			jmp bridg11
			checksendBridg:
				jmp checksend
			checkrecieveBridge:
				jmp checkreceive
			bridg11:
			cmp RecievedValue, f2SC
			jnz not_f2SC
			mov ax, 0
			mov al, name2Size
			showColoredMsg user2name, TEXT_COLOR, ax, start_Y_ofpress, start_X_ofpress
			mov al, start_X_ofpress
			add al, name2Size
			inc al
			mov maloshlazma, al
			showColoredMsg f2RecievedMsg, TEXT_COLOR, f2RecievedMsgSize, start_Y_ofpress, maloshlazma
			not_f2SC:
			; then check if it was a confirmation code to some invitation
			cmp RecievedValue, ConfirmationCode
			jnz checksend
			; if he agreed, then save my action that he agreed on
			mov sender, 0
			mov al, sendvalue
			mov AgreedValue, al
			jmp excuteoperation ; now excute the action that we agreed on
			;------------------------------------------------------------
			;-------------------------------------send data--------------
			checksend:
			;Check that Transmitter Holding Register is Empty
			mov dx , 3FDH		; Line Status Register
			In al , dx 			;Read Line Status
			AND al , 00100000b
			jz checkrecieveBridge
			; check if a key was pressed
			mov ah,1
			int 16h
			jz checkrecieveBridge ; jum if there wasn't a key press
			; get the pressed key
			mov ah,0
			int 16h
			; check that it is f1, f2 or esc
			cmp ah, 3bh ; f1
			jnz not_f1
			showColoredMsg f1, TEXT_COLOR, f1size, start_Y_ofpress-1, start_X_ofpress
			mov bx, 0
			mov bl, name2Size
			showColoredMsg user2name, TEXT_COLOR, bx, start_Y_ofpress-1, start_X_ofpress+30
			jmp handle_my_keys
			not_f1:
			cmp ah, 3ch ; f2
			jnz not_f2
			showColoredMsg f2, TEXT_COLOR, f2size, start_Y_ofpress-1, start_X_ofpress
			mov bx, 0
			mov bl, name2Size
			showColoredMsg user2name, TEXT_COLOR, bx, start_Y_ofpress-1, start_X_ofpress+30
			jmp handle_my_keys
			not_f2:
			cmp ah, 01h ; esc
			jz handle_my_keys
			jmp checkreceive ; if it is neither one of these, then jump
			handle_my_keys:
			cmp ah, RecievedValue
			jnz didnt_agree
			; if I agreed to the other player request, then do this
			mov SendValue, ConfirmationCode
			mov sender, 1
			jmp sendtobuffer
			didnt_agree:
			mov SendValue, ah
			sendtobuffer:
			mov al, sendvalue
			mov dx, 03f8h
			out dx, al
			cmp sendvalue, 01d ; check if I send a termination code
			jz exitMid
			cmp sendvalue, ConfirmationCode
			jnz goFinish1 ; if we didn't agree on something, then jump back to check recieve
			mov al, RecievedValue
			mov AgreedValue, al
			jmp skipPlz
			goFinish1:
			jmp goFinish
			skipPlz:
			;-----------------------------------------------excute the operatoin
			excuteoperation:

			showColoredMsg agreed, TEXT_COLOR, agreedSize, start_Y_ofpress, start_X_ofpress
			;------------------------------------------------------------
			

			;-------------------------------------------------------------------------------------------------------------;
			; cmp user_lastaction,01h          ; compare with ESC scan code  
			; jnz checkF1
			; showColoredMsg closeprogram, TEXT_COLOR, closeprogramsize, start_Y_ofpress, start_X_ofpress     
			; jmp exit
			

			; ;-------------------------------------------------------------------------------------------------------------;
			checkF1:
			cmp AgreedValue,3bh         ; compare with F1 scan code 
			; TODO: return this to normal
			; jnz mid_jump_to_checkF2 
			jnz goFinish12
			pusha
			call chatRoom
			popa
			jmp skipPlz2
			goFinish12:
			jmp checkF2
			skipPlz2:
			; restore all the settings
			MOV AX,@DATA
			MOV DS,AX
			mov es, ax
			UltraGraphicsMode
			putWallPaperM 14, 10, 0, 0

			jmp dont_mid_jump_to_checkF2
			mid_jump_to_checkF2: jmp checkF2
			exitMid:
			jmp exit
			dont_mid_jump_to_checkF2:

			drawLogo
			paintAreaM 0, 430d, 640d, 50d, 0H
			drawButtons
			goFinish:
			jmp finish
			
			

			;-------------------------------------------------------------------------------------------------------------;
			checkF2:
			cmp AgreedValue,3ch        ; compare with F2 scan code 
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
			;  mov sendvalue,0
			;  mov recievedvalue,0
			  mov agreedvalue,0
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

