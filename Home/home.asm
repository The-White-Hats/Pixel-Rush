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
include draw.inc
include infoM.inc

MAIN PROC FAR
    MOV AX,@DATA
    MOV DS,AX
    mov es, ax

    ; open graphics mode
    UltraGraphicsMode
    paintAreaM 0, 0, 640, 480, BackgroundColor  ; paint the screen in orange
    drawLogo

    GetPlayersNames

    drawButtons

    
    ; wait for a key input to not close the screen
    MOV AH, 0
    INT 16h

    UltraGraphicsMode

    MOV AH,4CH
    INT 21H

MAIN ENDP
END MAIN

