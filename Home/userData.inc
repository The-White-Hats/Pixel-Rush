mes1 db "Player 1 Name:", '$'
mes1size equ $ - mes1 - 1
mes1X equ 40 - 3 - ($ - mes1 - 1)/2
mes1Y equ 21

mes12 db "Player 2 Name:", '$'

mes2 db "Initial points:", '$'
mes2size equ $ - mes2 - 1
mes2X equ mes1X
mes2Y equ mes1Y + 1

nameWarning db "A Plyer name must start with a letter",'$'
warningX equ 40 - ($ - nameWarning - 1)/2
warningY equ mes1Y + 2
warningSize equ $ - 1 - nameWarning

pointsWarning db "Points MUST be a number",'$'

name1 db 16 dup('$')
name1Size db ?

points1 dw 0

name2 db 16 dup('$')
name2Size db ?

points2 dw 0

temp dw 0

nameSize equ 15d
nameInputX equ mes1X + mes1size + 1
nameInputY equ mes1Y

pointsInputX equ mes2X + mes2size + 1
pointsInputY equ mes2Y