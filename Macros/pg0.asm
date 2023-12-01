include assets.inc
.model small
.stack 64
.data
    mes1 db "Please enter your name:", 10, 13, 9, '$'
    mes2 db "Initial points:", 10, 13, 9, '$'
    nameWarning db "First character MUST be a letter",'$'
    pointsWarning db "Points MUST be a number",'$'
    name1 db 15, 0, 15 dup('$')
    points1 db 15, 0, 15 dup('$')
    name2 db 15, 0, 15 dup('$')
    points2 db 15, 0, 15 dup('$')
.code
main proc far
    mov ax, @data
    mov ds, ax
    clear
    showmes mes1
    getstr name1
    checkFirstChar name1, nameWarning
    endl
    showmes mes2
    getstr points1
    checkIfAllNumbers points1, pointsWarning
    clear
    showmes mes1
    getstr name2
    checkFirstChar name2, nameWarning
    endl
    showmes mes2
    getstr points2
    checkIfAllNumbers points2, pointsWarning

main endp
end main
