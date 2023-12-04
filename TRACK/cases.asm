.model small
.stack 100h
.data
    lastRandom db 0
    currentRandom db 6
    validationFlag db 0 ,'$'
    ;------------------------------------------- CASES -------------------------------------------;
    case0 db 0,5,7
    case1 db 8,10,1
    case2 db 2,6,11
    case3 db 3,4,9
    case4 db 1,8,10
    case5 db 2,6,11
    case6 db 1,9,11
    case7 db 3,4,9
    case8 db 2,6,11
    case9 db 0,5,7
    case10 db 3,4,9
    case11 db 0,5,7

.code
main proc far
    mov ax,@data
    mov ds,ax
    call checkCases
    add validationFlag, 30h
    mov ah, 09h
    mov dx, offset validationFlag
    int 21h

    mov ah, 4ch
    int 21h

main endp

checkCases proc
    mov al, lastRandom
    mov bl, 3
    mul bl
    mov bh, 0
    mov bl, al
    mov al, case0[bx]
    mov cx,3
    loop1:
        mov al, case0[bx]
        cmp al, currentRandom
        je valid
        inc bx
        loop loop1
    jmp endCheckCases
    valid:
        mov validationFlag, 1
    endCheckCases:
    ret
checkCases endp


end main
