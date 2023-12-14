
;---------------------Procedures---------------------;

CheckDir PROC
    ; move user_dir_arr offset into si
    ; move user_dir_bools offset into di
    mov cx, 0H
    xor ah, ah
    check_user:
    ; check for the key arrow key
    cmp al, [si] 
    jnz notPressed
    mov [di], 1H
    notPressed:
    mov bl, [si]
    add bl, 80H
    cmp al, bl
    jnz notReleased
    mov [di], 0H
    notReleased:

    inc si
    add di, 2h
    inc cx
    cmp cx, 4H
    jnz check_user
    ret
CheckDir ENDP


;---------------------Macros---------------------;

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
    add dx, bp          ; add offset to dx
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
    mov     ax, ypos
    sub     ax, CAR_HEIGHT/2
    mov     cx, SCREEN_WIDTH
    mul     cx
    add     ax, xpos
    sub     ax, CAR_WIDTH/2

    mov     di, ax
    mov     bx, 0   ; to clear
    call    drawCar 
ENDM

setStartPixel MACRO
    ; location => SCREEN_WIDTH*(user1_posY - CAR_HEIGHT/2) + user1_posX - CAR_WIDTH/2

    mov     ax, user1_posY
    sub     ax, CAR_HEIGHT/2
    mov     cx, SCREEN_WIDTH
    mul     cx
    add     ax, user1_posX
    sub     ax, CAR_WIDTH/2

    mov     di, ax
ENDM

update_user1_pos MACRO
    ; I am considering up as positive y and down as negative y
    mov ax, 0101h ; ah -> x, al -> y

    up1:
    cmp user1_dir_bools, 00H
    jz right1
    dec word ptr user1_posY
    inc al

    right1:
    cmp user1_dir_bools[2], 00H
    jz down1
    inc word ptr user1_posX
    inc ah

    down1:
    cmp user1_dir_bools[4], 00H
    jz left1
    inc word ptr user1_posY
    dec al

    left1:
    cmp user1_dir_bools[6], 00H
    jz finish_update1
    dec word ptr user1_posX
    dec ah
    
    finish_update1:

    update_dir

    call update_frame

ENDM

update_dir MACRO
    ; I am making a table of all compinations of x and y to know and update the direction

    cmp ah, 0
    jz left_dir
    cmp ah, 2
    jz right_dir
    cmp al, 0
    jz down_dir
    cmp al, 2
    jz up_dir

    jmp done_update

    up_dir:
    mov current_dir, 0
    jmp done_update

    down_dir:
    mov current_dir, 4
    jmp done_update

    left_dir:
    mov current_dir, 5
    add current_dir, al
    jmp done_update

    right_dir:
    mov current_dir, 3
    sub current_dir, al

    done_update:
ENDM

add_to_frame MACRO
    ; (current_frame + bl) % 8

    mov ah, 0
    mov al, current_frame
    add al, bl
    mov cl, 8
    div cl
    mov bl, ah
ENDM