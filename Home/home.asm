;AUTHOR : Ahmed Aladdin

;---------------------------------------
.286
.MODEL SMALL
.STACK 32
;---------------------------------------
.DATA
include LogoData.inc
include userData.inc
;---------------------------------------
.code
include pm.inc
include infoM.inc

MAIN PROC FAR
    MOV AX,@DATA
    MOV DS,AX
    mov es, ax

    ; open graphics mode
    UltraGraphicsMode
    ; PaintScreen 06d, 0H, 0ffffh

    drawLogo

    showColoredMsg mes1, 0fH, mes1size, mes1y, mes1x
    MoveCursor nameInputX, nameInputY
    getSizedStrM name1, 15d
    checkFirstChar name1, nameWarning
    MoveCursor 34d, 25
    showmes name1

    
    ; wait for a key input to not close the screen
    MOV AH, 0
    INT 16h
    jmp success

    error_exit:
    mov ah, 9
    mov dx, offset errtext
    int 21h

    success:
    MOV AH,4CH
    INT 21H

MAIN ENDP
END MAIN

