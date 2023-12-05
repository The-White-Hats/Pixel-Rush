.286
.model small
.stack 64
.data
  helper db 1H
  divider db 3H
  random db 0
  msg db 'Random Value', '$'
.code
include pm.inc
;description
randomizer PROC
  pusha
  mov ah, 2Ch
  int 21H    ; puts the millseconds in dl
  mov al, dl ; contain hundreds of seconds

  mov ah, 0
  div helper ; divide ax by the helper which keep changing; puts the quotiant in al , and remainder in ah
  mov ah, 0  
  div divider
  mov random, ah ; puts the remainder in random

  inc helper    ; increment helper to insure random value every time
  cmp helper, 0ffh ; return helper to 1 if it's ffh to avoid dividing by zero
  jne dontreturn
  mov helper, 1h
  dontreturn:
  popa
  ret
randomizer ENDP

;description
main PROC far
  mov ax, @data
  mov ds, ax

  GraphicsMode

  MoveCursor 5d, 34d
  ShowMes msg

  re:
  call randomizer
  MoveCursor 11d, 36d
  xor ax,ax
  mov al, random
  PrintVarLenNum ax
  jmp re
  
  mov ah, 4ch
  int 21h
main ENDP
end main