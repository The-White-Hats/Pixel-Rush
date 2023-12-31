;AUTHOR : Ahmed Aladdin

;---------------------------------------
.286
extrn GameMain:far
extrn chatRoom:far
public user1name,user2name
extrn IsUser1Win:BYTE
extrn IsUser2Win:BYTE
extrn user1score:BYTE
extrn user2score:BYTE
.MODEL SMALL
.STACK 32
;---------------------------------------
.DATA
                include ./inc/LogoData.inc
                include ./inc/userData.inc
                include ./inc/AcData.inc
    s           db      ?
                dt      ?
    scorestring db      ?
                dt      ?
     RecievedValue db ?
     SendValue db ?
     AgreedValue db ?
     ConfirmationCode equ 3AH

    ;---------------------------------------
.code
         include           ./inc/draw.inc
         include           ./inc/infoM.inc
         include           ./inc/Action.inc



MAIN PROC FAR
         MOV               AX,@DATA
         MOV               DS,AX
         mov               es, ax

    ; open graphics mode
         UltraGraphicsMode
    
         putWallPaperM     14, 10, 0, 0


         drawLogo
         drawnames

         GetPlayersNames

         drawButtons

         call              GetAction
         
         UltraGraphicsMode

         MOV               AH,4CH
         INT               21H

MAIN ENDP
END MAIN

