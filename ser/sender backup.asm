.286
.model small
.stack 64
.data
value db 0
xs db 0
ys db 0
xr db 0
yr db 12
.code
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
jnz enter1
mov value,'#'
jmp enterr
enter1:
mov value,al

enterr:
cmp value,'#'
jnz cont1
inc ys
mov xs,0
mov ah,2
		mov dh,ys
		mov dl,xs
		int 10h
jmp check3

cont1:
		mov ah,2
		mov dh,ys
		mov dl,xs
		int 10h

		mov ah,9
		mov bh,0
		mov al,value
		mov cx,1
		mov bl,15
	
		int 10h



inc xs


check3:
;If empty put the VALUE in Transmit data register
  		mov dx , 3F8H		; Transmit data register
  		mov  al,VALUE
  		out dx , al 


cmp value,27
jz endfinal
;--------------------------------------------------;

;Recieve

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

		
cmp value,27
jz endfinal		

cmp value,'#'
jnz cont
inc yr
mov xr,0
mov ah,2
		mov dh,yr
		mov dl,xr
		int 10h
jmp checksend

cont:
		mov ah,2
		mov dh,yr
		mov dl,xr
		int 10h

		mov ah,9
		mov bh,0
		mov al,value
		mov cx,1
		mov bl,15
	
		int 10h



inc xr
jmp checksend
endfinal:
main endp
end main