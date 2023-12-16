RED equ 04h
LIGHT_GREEN equ 0Ah
GREEN equ 02h
LIGHT_GRAY equ 07h
GRAY equ 08h
WHITE equ 0Fh
BLACK equ 00h
CYAN equ 03h
LIGHT_CYAN equ 0bh	
BROWN equ 06h
BLUE equ 01h 
LIGHT_BLUE equ 09h
PURPLE equ 05h
YELLOW equ 0Eh
ORANGE equ 0Ch
LIGHT_ORANGE equ 27h
EXTRA_LIGHT_GRAY equ 2ch
TEXT_COLOR equ YELLOW

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