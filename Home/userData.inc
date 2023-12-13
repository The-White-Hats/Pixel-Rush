mes1 db "Enter First Player username", '$'
mes1size equ $ - mes1 - 1
mes1X equ 40 - ($ - mes1 - 1)/2
mes1Y equ 22

mes2 db "Initial points:", 10, 13, 9, '$'

nameWarning db "Enter a valid username",'$'
warningX equ 40 - ($ - nameWarning - 1)/2
warningY equ mes1Y + 2
warningSize equ $ - 1 - nameWarning

pointsWarning db "Points MUST be a number",'$'
name1 db 15, 0, 16 dup('$')
points1 db 15, 0, 15 dup('$')
name2 db 15, 0, 16 dup('$')
points2 db 15, 0, 15 dup('$')
nameSize equ 15d
nameInputX equ 35D
nameInputY equ mes1Y + 1