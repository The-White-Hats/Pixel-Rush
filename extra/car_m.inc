clear macro 
    mov ah, 0       ; AH=0 is the function for setting video mode
    mov al, 13h       ; AL=3 sets 80x25 text mode (standard text mode)
    int 10h         ; BIOS interrupt for video services
endm clear 

check_user1_dir MACRO
    lea si, user1_dir_arr
    lea di, user1_dir_bools
    mov cx, 0H
    xor ah, ah
    check_user1:
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
    inc di
    inc cx
    cmp cx, 4H
    jnz check_user1
ENDM

check_user2_dir MACRO
    lea si, user2_dir_arr
    lea di, user2_dir_bools
    mov cx, 0H
    xor ah, ah
    check_user2:
    ; check for the up arrow key
    cmp al, [si]
    jnz notPressed2
    mov [di], 1H
    notPressed2:
    mov bl, [si]
    add bl, 80H
    cmp al, bl
    jnz notReleased2
    mov [di], 0H
    notReleased2:

    inc si
    inc di
    inc cx
    cmp cx, 4H
    jnz check_user2
ENDM

clear_prev_location MACRO xpos, ypos
  mov cl, xpos
  mov dl, ypos
  mov al,0Fh
  mov ah,0ch
  int 10h   
ENDM

update_user1_pos MACRO
    up1:
    cmp user1_dir_bools, 00H
    jz right1
    dec byte ptr user1_posY

    right1:
    cmp user1_dir_bools[1], 00H
    jz down1
    inc byte ptr user1_posX

    down1:
    cmp user1_dir_bools[2], 00H
    jz left1
    inc byte ptr user1_posY

    left1:
    cmp user1_dir_bools[3], 00H
    jz finish_update1
    dec byte ptr user1_posX
    
    finish_update1:
ENDM

update_user2_pos MACRO
    up2:
    cmp user2_dir_bools, 00H
    jz right2
    dec byte ptr user2_posY

    right2:
    cmp user2_dir_bools[1], 00H
    jz down2
    inc byte ptr user2_posX

    down2:
    cmp user2_dir_bools[2], 00H
    jz left2
    inc byte ptr user2_posY

    left2:
    cmp user2_dir_bools[3], 00H
    jz finish_update2
    dec byte ptr user2_posX

    finish_update2:
ENDM