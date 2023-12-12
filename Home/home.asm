;AUTHOR : Ahmed Aladdin

;---------------------------------------
.286
.MODEL SMALL
.STACK 32
;---------------------------------------
.DATA

include LogoData.inc
;---------------------------------------
.code
include pm.inc

MAIN PROC FAR
    MOV AX,@DATA
    MOV DS,AX

    ; open graphics mode
    mov AX, 4F02h
    mov BX, 0101h  ; 640x400x256
    int 10h

    drawLogo

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

