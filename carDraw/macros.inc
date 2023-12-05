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

clear_prev_location MACRO xpos, ypos
  mov cx, xpos
  mov dx, ypos
  mov al,0Fh
  mov ah,0ch
  int 10h   
ENDM

check_user1_dir MACRO

    lea si, user1_dir_arr
    lea di, user1_dir_bools
    mov cx, 4H
    xor ah, ah
    check_user1:

        ; check arrow keys
        cmp al, byte ptr [si]
        jnz notPressed
        mov byte ptr [di], 1H
        notPressed:
        mov bl, byte ptr [si]
        add bl, 80H
        cmp al, bl
        jnz notReleased
        mov byte ptr [di], 0H
        notReleased:

        inc si
        inc si
        inc di

    loop check_user1

ENDM


update_user1_pos MACRO
    up1:
    cmp user1_dir_bools, 00H
    jz right1
    dec word ptr user1_posY

    right1:
    cmp user1_dir_bools[1], 00H
    jz down1
    inc word ptr user1_posX

    down1:
    cmp user1_dir_bools[2], 00H
    jz left1
    inc word ptr user1_posY

    left1:
    cmp user1_dir_bools[3], 00H
    jz finish_update1
    dec word ptr user1_posX
    
    finish_update1:
ENDM