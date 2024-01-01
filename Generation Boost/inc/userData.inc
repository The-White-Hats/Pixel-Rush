user1name db 16 dup('$')
name1Size db ?

points1 db 0

user2name db 16 dup('$')
name2Size db ?

nameWarning db "Invalid first letter",'$'
warningX equ 40 - ($ - nameWarning - 1)/2
warningY equ 22
warningSize equ $ - 1 - nameWarning


username1Size dw 0
username2Size dw 0

WinnerMessagePart1 db 'Congratulations$'
part1size equ $-WinnerMessagePart1-1
WinnerMessagePart2 db 'YOU WON!$'
part2size equ $-WinnerMessagePart2-1

BothLostMes db 'You Both Have Lost$'
BothLostMes1size equ $-BothLostMes-1

score db 'Score: $'
scoreSize equ $-score-1


points2 db 0


nameSize equ 15d
nameInputX equ 40 - nameSize/2
nameInputY equ 21

lineHeight equ 16D

goaltime db ?
waittime db 2

f1SC equ 3bh
f2SC equ 3ch

f1RecievedMsg db "sent you a chat invitation, to accept plz press F1        $"
f1RecievedMsgSize equ $-f1RecievedMsg-1
f2RecievedMsg db "sent you a game invitation, to accept plz press F2        $"
f2RecievedMsgSize equ $-f2RecievedMsg-1

maloshlazma db 0