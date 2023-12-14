logoUL db 'logoUl.bin', 0
logoLL db 'logoLL.bin', 0
logoUR db 'logoUR.bin', 0
logoLR db 'logoLR.bin', 0
firstPN db 'P1N.bin', 0
secondPN db 'P2N.bin', 0
IPP db 'IP.bin', 0

buffer_size equ 15560           ; the array size which is gonna be filled with the img data from the .bin file
buffer db buffer_size dup(?)

errtext db "YOUR ERROR MESSAGE", 10, "$"

Xstart dw ?
Ystart dw ?
Xlimit dw ?
Ylimit dw ?

errorFlag db 0