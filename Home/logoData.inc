logoUL db 'logoUl.bin', 0
logoLL db 'logoLL.bin', 0
logoUR db 'logoUR.bin', 0
logoLR db 'logoLR.bin', 0
hackers db 'hackers.bin', 0
firstPN db 'P1N.bin', 0
secondPN db 'P2N.bin', 0
F1button db 'F1B.bin',  0
F2button db 'F2B.bin',  0
Exitbutton db 'ExitB.bin',  0
wallpaper db 'w1.bin', 0

BackgroundColor equ 2AH
ScreenWidth equ 640d
ScreenHeight equ 480d

buffer_size equ 15560           ; the array size which is gonna be filled with the img data from the .bin file
buffer db buffer_size dup(?)

Xstart dw ?
Ystart dw ?
Xlimit dw ?
Ylimit dw ?

; bg = background, C = column, R = row | repeat, S = start
bgCR db ?
bgRR db ?
bgCS dw ?
bgRS dw ?
bgSize equ 48

errorFlag db 0