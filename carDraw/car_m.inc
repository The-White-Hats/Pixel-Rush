
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
    up1:
    cmp user1_dir_bools, 00H
    jz right1
    dec word ptr user1_posY

    right1:
    cmp user1_dir_bools[2], 00H
    jz down1
    inc word ptr user1_posX

    down1:
    cmp user1_dir_bools[4], 00H
    jz left1
    inc word ptr user1_posY

    left1:
    cmp user1_dir_bools[6], 00H
    jz finish_update1
    dec word ptr user1_posX
    
    finish_update1:
ENDM