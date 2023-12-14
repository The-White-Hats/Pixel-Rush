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

    paintAreaM 0, 0, 640, 480, 2AH  ; paint the screen in orange

    drawLogo

    pusha
    paintAreaM 212d, 325d, 216d, 50d, 0H  ; paint input area in black

    readImageM firstPN
    draw 220, 305, 199, 15

    ; showColoredMsg mes1, 0fH, mes1size, mes1y, mes1x
    MoveCursor nameInputX, nameInputY
    getSizedStrM name1, 15d
    checkFirstChar name1, nameWarning
    paintAreaM 201, 305, 238, 17, 2AH   ; clear 'player 1 name' logo
    popa

    ; second player
    pusha
    readImageM secondPN
    draw 220, 305, 199, 15
    paintAreaM 212d, 325d, 216d, 50d, 0H  ; paint input area in black

    ; showColoredMsg mes12, 0fH, mes1size, mes1y, mes1x
    MoveCursor nameInputX, nameInputY
    getSizedStrM name2, 15d
    checkFirstChar name2, nameWarning

    paintAreaM 210d, 325d, 220d, 50d, 2AH  ; clear the error area
    paintAreaM 201, 305, 238, 17, 2AH 
    paintAreaM 135, 464, 369, 11, 2AH 
    popa

    ; MoveCursor 34d, 25d
    ; showmes name1
    ; showchar ' '
    ; showmes name2

    
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

