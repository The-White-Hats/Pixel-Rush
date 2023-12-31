.286
public chatRoom
.model small
.stack 64
.data
value db 0
sendval db 0
senX db 0
senY db 0
senP db 0
RecX db 40d
RecY db 0
recP db 0

pageScroll equ 12

.code
include ./inc/pm.inc

scrollR proc
pusha
mov ah, 6d       ; function 6
   mov al,1d        ; scroll by 1 line    
   mov bh, 7h       ; normal video attribute ; the color of the background and forground       
   mov ch,0       ; upper left Y
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
   mov ch,0   	    ; upper left Y
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



	;--------------------------------------------------------------recieve------------------------------------;
	checkreceive:

	;Check that Data Ready
		mov dx , 3FDH		; Line Status Register
		in al , dx 
		AND al , 1
		JZ checksend

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
		cmp al, 27d
		jz midfinal

		

	;-------------------sender-------------------------;

	checksend:
	;Check that Transmitter Holding Register is Empty
	mov dx , 3FDH		; Line Status Register
	In al , dx 			;Read Line Status
	AND al , 00100000b
	jz checkreceive

	mov ah,1
	int 16h
	jz checkreceive

	jmp notmid
	midfinal: jmp endfinal
	notmid:

	mov ah,0
	int 16h

	

	sendtobuffer:
	mov al, sendval
	mov dx, 03f8h
	out dx, al
	cmp sendval, 27d
	jz endfinal

	jmp checkreceive
	;--------------------------------------------------;



	jmp checksend
	endfinal:
	mov value, 0
	mov sendval, 0
	mov senX, 0
	mov senY, 0
	mov senP, 0
	mov RecX, 40d
	mov RecY, 0
	mov recP, 0
	ret
chatRoom endp
end