nameWarning db "Invalid first letter",'$'
warningX equ 40 - ($ - nameWarning - 1)/2
warningY equ 22
warningSize equ $ - 1 - nameWarning

name1 db 16 dup('$')
name1Size db ?

points1 db 0

name2 db 16 dup('$')
name2Size db ?

points2 db 0


nameSize equ 15d
nameInputX equ 40 - nameSize/2
nameInputY equ 21

lineHeight equ 16D