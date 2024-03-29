.286
public chatRoom
extrn user1name:BYTE
extrn user2name:BYTE
.model small
.stack 64
.data
value db 0
sendval db 0
senX db 0
senY db 1
senP db 0
RecX db 40d
RecY db 1
recP db 0

pageScroll equ 12

.code
include ./inc/pm.inc

scrollR proc
pusha
mov ah, 6d       ; function 6
   mov al,1d        ; scroll by 1 line    
   mov bh, 7h       ; normal video attribute ; the color of the background and forground       
   mov ch,1       ; upper left Y
   mov cl,40d        ; upper left X
   mov dh,24d    ; lower right Y
   mov dl,79d      ; lower right X 
   int 10h    
	 dec recy
	 mov recX, 40d       
popa
ret
scrollR endp

scrollS proc
	pusha
mov ah, 6d          ; function 6
   mov al, 1D       ; scroll by 1 line    
   mov bh, 7h       ; normal video attribute ; the color of the background and forground         
   mov ch,1   	    ; upper left Y
   mov cl,0         ; upper left X
   mov dh,24        ; lower right Y
   mov dl,39        ; lower right X 
   int 10h    	
   dec senY
	 mov senX, 0	
popa
ret
scrolls ENDP



chatRoom proc far

	mov ax,@data
	mov ds,ax


	mov ah,0
	mov al,3
	int 10h
	;First Proc
	; -----------------------------------------------------Configure----------------------------------------------;
	;Set Divisor Latch Access Bit
	mov dx,3fbh 			; Line Control Register
	mov al,10000000b		;Set Divisor Latch Access Bit
	out dx,al			;Out it
	;Set LSB byte of the Baud Rate Divisor Latch register.
	mov dx,3f8h			
	mov al,0ch			
	out dx,al
	;Set MSB byte of the Baud Rate Divisor Latch register.
	mov dx,3f9h
	mov al,00h
	out dx,al
	;Set port configuration
	mov dx,3fbh
	mov al,00011011b
	; 0:Access to Receiver buffer, Transmitter buffer
	; 0:Set Break disabled
	; 011:Even Parity
	; 0:One Stop Bit
	; 11:8bits
	out dx,al

	; print the player one name
	lea dx, user1name
	mov ah, 09h
	int 21h
	showchar ':'
	MoveCursor 40, 0, 0
	lea dx, user2name
	mov ah, 09h
	int 21h
    showchar ':'

	;--------------------------------------------------------------recieve------------------------------------;
	checkreceive:

	;Check that Data Ready
		mov dx , 3FDH		; Line Status Register
		in al , dx 
		AND al , 1
		JZ CheckSendBridge

	;If Ready read the VALUE in Receive data register
		mov dx , 03F8H
		in al , dx 
		mov VALUE , al
		; check if it was enter
		cmp al, 10d
		jnz notEnterRe
		mov RecX, 40d
		inc RecY
		cmp RecY, 25d
		jnz dontscrollr1
		call scrollR
		dontscrollr1:
		JMP NEXT
		; check if it was esc
		notEnterRe:
		cmp al, 3dh
		jz midfinal
		;check if it was backspace
		cmp al, 8H
		jnz notBackspace3
		cmp recx, 40d
		jz dontDexRecx
		pusha
		MoveCursor RecX, RecY, recp		
		popa
		dec recX
		dec recX
		showchar 8h
		dontDexRecx:
		showchar 0
		showchar 8h
		jmp skipShowingTheBAckspace2
		notBackspace3:
		
		jmp dontCheckRecBridge
		CheckRecBridge: jmp checkreceive
		CheckSendBridge: jmp checksend
		dontCheckRecBridge:

		MoveCursor RecX, RecY, recp
		ShowChar VALUE
		skipShowingTheBAckspace2:
		inc RecX
		cmp RecX, 79d
		jnz NEXT
		mov RecX, 40d
		inc RecY
		NEXT:
		cmp RecY, 25d
		jnz dontscrollr2
		call scrollR
		dontscrollr2:

		jmp notmid
		midfinal: jmp endfinal
		notmid:

	;-------------------sender-------------------------;

	checksend:
	;Check that Transmitter Holding Register is Empty
	mov dx , 3FDH		; Line Status Register
	In al , dx 			;Read Line Status
	AND al , 00100000b
	jz CheckRecBridge

	mov ah,1
	int 16h
	jz CheckRecBridge

	mov ah,0
	int 16h

	; check if it was enter
	cmp ah,1ch
	jnz notEnter
	inc senY
	mov senX, 0
	mov sendval, 10d
	; check the end of my area
	cmp senY, 25d
	jnz dontscrolls
	call scrollS
	dontscrolls:
	jmp sendtobuffer

	; check if it was esc
	notEnter:
	cmp ah, 3dH
	jnz notEsc
	mov sendval, 3dh
	jmp sendtobuffer

	notEsc:
	; check if it was backspace
	cmp al, 8H
	jnz notBackspace2
	cmp senx, 0
	jz dontDexSenx
	dec senX
	dec senX
	showchar 8h
	dontDexSenx:
	showchar 0
	showchar 8h
	mov sendval, al
	jmp skipShowingTheBAckspace3
	notBackspace2:
	mov sendval, al

	; print the char of the sender
	MoveCursor senX, senY, senP
	ShowChar sendval
	; deal with the new cursor of the sender
	skipShowingTheBAckspace3:
	inc senX
	cmp senX, 39d
	jnz NEXT2
	mov senX, 0
	inc senY
	cmp senY, 25d
	jnz NEXT2
	call scrollS
	NEXT2:

	sendtobuffer:
	mov al, sendval
	mov dx, 03f8h
	out dx, al
	cmp sendval, 3dh
	jz endfinal

	jmp checkreceive
	;--------------------------------------------------;



	jmp checksend
	endfinal:
	mov value, 0
	mov sendval, 0
	mov senX, 0
	mov senY, 1
	mov senP, 0
	mov RecX, 40d
	mov RecY, 1
	mov recP, 0
	ret
chatRoom endp
end