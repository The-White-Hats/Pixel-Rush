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

    paintAreaM 160d, 320d, 320d, 79d, 0H  ; paint input area in black
    ; readImageM firstPN
    ; draw 234, 335, 116, 19

    showColoredMsg mes1, 0fH, mes1size, mes1y, mes1x
    MoveCursor nameInputX, nameInputY
    getSizedStrM name1, 15d
    checkFirstChar name1, nameWarning

    ; MoveCursor warningX, warningY
    ; clearLineArea 160D

    paintAreaM 160d, 370d, 320d, 17d, 02H  ; clear the error area
    
    showColoredMsg mes2, 0fH, mes2size, mes2y, mes2x
    MoveCursor pointsInputX, pointsInputY
    ReadVarLenNum points1

    ; MoveCursor mes1x, mes1y
    ; clearLineArea 160D

    ; second player
    paintAreaM 160d, 320d, 320d, 79d, 0H  ; paint input area in black

    showColoredMsg mes12, 0fH, mes1size, mes1y, mes1x
    MoveCursor nameInputX, nameInputY
    getSizedStrM name2, 15d
    checkFirstChar name2, nameWarning

    paintAreaM 160d, 370d, 320d, 17d, 02H  ; clear the error area
    
    showColoredMsg mes2, 0fH, mes2size, mes2y, mes2x
    MoveCursor pointsInputX, pointsInputY
    ReadVarLenNum points2
    

    MoveCursor 34d, 25
    showmes name1
    showchar ' '
    showmes name2

    
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

