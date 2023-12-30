.model small
.stack 64
.data
seed DW 1234H       ; Initial seed value
multiplier Dw 1d51H
increment Dw 8c45H
modulus DW 0F4bFh 
random_seed db 0, 10, 13, '$' ; 4 bytes of memory to store the random seed

.code   
main proc far
    mov ax, @data
    mov ds, ax 
    mov cx,5

  
    loop1:
    push cx
    call rndm
    
    mov ah,09h
    lea dx,random_seed
    int 21h 
    ; call delay interrupt to delay 211 ms
    
    pop cx
    loop loop1
    
    
    
    mov ah, 4Ch
    int 21h
main endp

rndm proc
    mov al, 0
    mov ah, 86h
    mov dx, 211
    mov cx, 7
    int 15h

    mov ah, 2Ch
    int 21h 
    mov ax, dx
    mov bl, 17
    mul bl
    mov bl, 137
    div bl
    mov al,ah
    mov ah, 17
    mov bx, ax

    
    mov al,dl
    mov bl, 5
    mul bl

    MOV BX, multiplier
    MUL BX               ; AX = seed * multiplier
    ADD AX, increment    ; AX = AX + increment
    MOV CX, modulus      ; CX = modulus (2^16)
    DIV CX               ; AX = AX / CX, DX = AX % CX (remainder)
    mov bh, 12
    div bh
    mov random_seed, ah
    

    ret
rndm endp


end main
