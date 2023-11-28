include usernames.inc
.model small
.stack 64
.data
    mes1 db "Please enter your name:", 10, 13, 9, '$'
    mes2 db "Initial points:", 10, 13, 9, '$'
    warning db "Please enter only letters!",'$'
    name1 db 15, ?, 15 dup('$')
    points1 db 15, ?, 15 dup('$')
    name2 db 15, ?, 15 dup('$')
    points2 db 15, ?, 15 dup('$')
.code
main proc far
    mov ax, @data
    mov ds, ax
    getUserName mes1, mes2, name1, points1, warning
    clear
    getUserName mes1, mes2, name2, points2, warning
main endp
end main
