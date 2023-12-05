clear macro 
    mov ah, 0
    mov al, 13h
    int 10h
endm clear

rmBlink macro
    mov ax, 1003h
    mov bl, 00h     ; 00h background intensity enabled , 01h blink enabled
    mov bh, 00h     ; to avoid problems on some adapters
    int 10h
endm rmBlink

; put filename in dx
openFile macro
    mov ah, 03Dh
    mov al, 0       ; open attribute: 0 - read-only, 1 - write-only, 2 -read&write
    int 21h
endm openFile

readFile macro BUFF_SIZE, car
    mov bx, ax
    mov ah, 03Fh
    mov cx, BUFF_SIZE   ; number of bytes to read
    lea dx, car         ; were to put read data
    int 21h
endm readFile

closeFile macro
    mov ah, 3Eh
    int 21h
endm closeFile

showMsg macro str
    mov ah, 09h
    lea dx, str
    int 21h  
endm showMsg 