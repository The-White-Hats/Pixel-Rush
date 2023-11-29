include usernames.inc
.model small
.stack 64
.data
    mes1 db "Please enter your name:", 10, 13, 9, '$'
    mes2 db "Initial points:", 10, 13, 9, '$'
    warning db "First character MUST be a letter",'$'
    name1 db 15, 0, 15 dup('$')
    points1 db 15, 0, 15 dup('$')
    name2 db 15, 0, 15 dup('$')
    points2 db 15, 0, 15 dup('$')
.code
main proc far
    mov ax, @data
    mov ds, ax
    showmes mes1
    getstr name1
    checkFirstChar name1, warning
    endl
    showmes mes2
    getstr points1
    clear
    showmes mes1
    getstr name2
    checkFirstChar name2, warning
    endl
    showmes mes2
    getstr points2
main endp
end main
