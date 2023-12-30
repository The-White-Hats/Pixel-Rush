;AUTHOR : Ahmed Aladdin

;---------------------------------------
.286
.MODEL SMALL
.STACK 32
;---------------------------------------
.DATA

filename1 db 'logo.bin', 0
filename2 db 'th.bin', 0
buffer_size equ 26624           ; the array size which is gonna be filled with the img data from the .bin file
buffer db buffer_size dup(?)

errtext db "YOUR ERROR MESSAGE", 10, "$"

Xstart dw ?
Ystart dw ?
Xlimit dw ?
Ylimit dw ?

errorFlag db 0
;---------------------------------------
.code
include pm.inc

MAIN PROC FAR
    MOV AX,@DATA
    MOV DS,AX

    ; open graphics mode
    mov ah,0
    mov al,13h
    int 10h

    readImageM filename2
    draw 100, 10, 173, 150

    ; wait for a key input to not close the screen
    MOV AH, 0
    INT 16h

    error_exit:
    mov ah, 9
    mov dx, offset errtext
    int 21h

    MOV AH,4CH
    INT 21H

MAIN ENDP
END MAIN

