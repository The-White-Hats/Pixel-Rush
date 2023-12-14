mes1 db "Player 1 Name:", '$'
mes1size equ $ - mes1 - 1
mes1X equ 40 - 8 - ($ - mes1 - 1)/2
mes1Y equ 21

mes12 db "Player 2 Name:", '$'

nameWarning db "Invalid first letter",'$'
warningX equ 40 - ($ - nameWarning - 1)/2
warningY equ mes1Y + 1
warningSize equ $ - 1 - nameWarning

name1 db 16 dup('$')
name1Size db ?

points1 db 0

name2 db 16 dup('$')
name2Size db ?

points2 db 0


nameSize equ 15d
nameInputX equ 40 - nameSize/2
nameInputY equ mes1Y

lineHeight equ 16D

; pointsSize equ 3d
; pointsInputX equ mes2X + mes2size + 1
; pointsInputY equ mes2Y