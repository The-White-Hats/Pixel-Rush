.model small
.stack 64
.data
seed dw 1234H       ; Initial seed value
random db ?         ; Random number

.code   
main proc far
    mov ax, @data
    mov ds, ax 
    mov cx,5

  
    loop1:
    push cx
    call rndm
    
    mov ah,09h
    lea dx,random
    int 21h 
    ; call delay interrupt to delay 211 ms
    
    pop cx
    loop loop1
    
    
    
    mov ah, 4Ch
    int 21h
main endp

rndm proc
    mov ah, 2Ch
    int 21h
    add seed, dx
    mov ax, seed
    mov bx, 32749
    mul bx
    add ax, 32541
    mov seed, ax

    ; Convert the result to a number between 0 and 11
    mov bh, 12
    div bh      ; Divide DX:AX by BL
    mov random, ah

    ret
rndm endp



end main
