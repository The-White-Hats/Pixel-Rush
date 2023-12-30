.286
.model small
.stack 64
.data
value db 0
sendval db 0
SenderX db 0
SenderY db 13
RecX db 0
RecY db 0
pageScroll equ 12

.code
include pm.inc
scroll proc
pusha
mov ah,6       ; function 6
   mov al,1        ; scroll by 1 line    
   mov bh,7       ; normal video attribute         
   mov ch,0       ; upper left Y
   mov cl,0        ; upper left X
   mov dh,12     ; lower right Y
   mov dl,79      ; lower right X 
   int 10h           
popa
scroll endp
main proc far

mov ax,@data
mov ds,ax


mov ah,0
mov al,3
int 10h
;First Proc

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



;----------------------recieve
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
	cmp al, 10d
	jnz notEnterRe
	mov RecX, 0
	inc RecY
	JMP NEXT
	notEnterRe:
	cmp al, 27d
	jz midfinal

	
	mov bh, 0
	MoveCursor RecX, RecY

	ShowChar VALUE
	inc RecX
	cmp RecX, 79d
	jnz NEXT
	mov RecX, 0
	inc RecY
	;cmp RecY, 25d
	NEXT:

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



mov ah,0
int 16h


cmp ah,1ch
jnz notEnter
inc SenderY
mov sendval, 10d
jmp sendtobuffer

midfinal: jmp endfinal

notEnter:
cmp value,27d
jnz notEsc
mov sendval, 27d
jmp sendtobuffer

notEsc:
mov sendval, al

; print the char of the sender
mov bh, 0
MoveCursor SenderX, SenderY
ShowChar sendval
; deal with the new cursor of the sender
inc SenderX
cmp SenderX, 79d
jnz NEXT2
mov SenderX, 0
inc SenderY
NEXT2:

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
mov ah, 4ch
int 21h
main endp
end main