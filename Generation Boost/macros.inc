clear macro 
    mov ah, 0
    mov al, 13h
    int 10h
endm clear

colorScreen macro color
	mov ax, 0600h
	mov bh, color
	mov cx, 0h      ; upper left corner
	mov dx, 184fh   ; lower right corner
    int 10h
endm colorScreen

endlst macro  start     ; give it the start of the vertical cordinate (X-axis)
    ; newline
    mov ah,02h
    mov dl,10
    int 21h
      
    mov ah,3
  	mov bh,0h
  	int 10h
  	   
	mov ah,2
  	mov dl,start
  	int 10h 
  	
endm endlst
  
endl macro 
       mov ah,02h
    mov dl,10
    int 21h 
    mov ah,02h
    mov dl,13
    int 21h
endm endl  

clearline macro
    mov ah,9
    mov bh,0
    mov al,0h
    mov cx,79
    mov bl,SCREEN_ATTR
    int 10h
endm cearline   
  
; shownum macro
;     local l1     
;     local l2
;      lea bx, s
;      mov cx,10
;      mov dx,0
;      l1:
;        mov dx,0
;        div cx     
;        add dl,30h
;        mov [bx],dl
;        inc bx 
;        cmp ax,0
;      jnz l1     
;      mov byte ptr [bx],'$' 
;      mov cx, bx
;      lea dx,s
;      sub cx,dx  
;      dec bx
;      l2:
;          mov ah,2
;          mov dl,[bx]
;          int 21h
;          dec bx
;      loop l2      
; endm shownum
overrideInt9 macro
    ; Disable interrupts
                 cli
    ; Save the original interrupt vector for int 9h
                 mov                 ax, 3509h
                 int                 21h
                 mov                 origInt9Offset, bx
                 mov                 origInt9Segment, es

                 push                ds
                 mov                 ax, cs
                 mov                 ds, ax
    ; Change the interrupt vector for int 9h
                 mov                 ax, 2509h
                 lea                 dx, my_isr
                 int                 21h
    ; Re-enable interrupts
                 pop                 ds
                 sti
endm overrideInt9

restoreInt9 macro
    ; Restore the original interrupt vector for int 9h
             cli
             mov                 ax, origInt9Segment
             mov                 dx, origInt9Offset
    
             push                ds
             mov                 ds, ax

             mov                 ax, 2509h
             int                 21h
    ; Re-enable interrupts
             pop                 ds
             sti
endm resroreInt9

rmBlink macro
; Remove Blinking from the screen and allowing to use 16 colors as background
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

inputAllFiles macro
        mov  cx, 0
        mov  bp, 0
        mov  dx, offset carFile
    file_again:
        push dx
        push cx
        call inputFile
        pop  cx
        pop  dx
        add  dx, 2*FILE_NAME_LEN
        add  bp, BUFF_SIZE
        inc  cx
        cmp  cx, 8
        jnz  file_again
endm inputAllFiles

showMsg macro str
    mov ah, 09h
    lea dx, str
    int 21h  
endm showMsg

delayFunc macro
    mov    ax, 8600H   ; AH = 86h (Delay function), AL = 00h (not used)
    xor    cx, cx      ; CH = high order byte of delay count, CL = not used
    mov    dx, 090FFH  ; DL = low order byte of delay count, DH = not used
    int    15H         ; Call BIOS delay function
endm delayFunc

intialCarPosition macro
    mov     ax, START_CAR_X
    mov     user1_posX, ax
    mov     ax,START_CAR2_X
    mov     user2_posX, ax
    mov     ax, START_CAR_Y
    mov     user1_posY, ax
    mov     ax, START_CAR2_Y
    mov     user2_posY, ax


    mov     al, user1_current_frame
    mov     user1_current_dir, al
    mov     al, user2_current_frame
    mov     user2_current_dir, al

    ; save the background of first car
    setStartPixel user1_posX, user1_posY
    mov     si, offset user1_current_background
    mov     bx, 1
    call    setBackground

    ; draw the first car
    setStartPixel user1_posX, user1_posY
    mov           si, offset car1
    mov           al, user1_current_frame
    mov           bh, 1
    call          handleCar

    ; save the background of second car
    setStartPixel user2_posX, user2_posY
    mov     si, offset user2_current_background
    mov     bx, 1
    call    setBackground

    ; draw the second car
    setStartPixel user2_posX, user2_posY
    mov           si, offset car2
    mov           al, user2_current_frame
    mov           bh, 1
    call          handleCar
endm intialCarPosition

setStartPixel MACRO xpos, ypos
    ; location => SCREEN_WIDTH*(ypos - CAR_HEIGHT/2) + xpos - CAR_WIDTH/2
    
    mov     ax, ypos
    sub     ax, CAR_HEIGHT/2
    mov     cx, SCREEN_WIDTH
    mul     cx
    add     ax, xpos
    sub     ax, CAR_WIDTH/2
    mov     di, ax
ENDM

update_user1_pos MACRO
    ; I am considering up as positive y and down as negative y
    mov ax, 0101h ; ah -> x, al -> y
    mov bx, word ptr user1_speed

    up1:
    cmp user1_dir_bools, 00H
    jz right1
    sub word ptr user1_posY, bx
    inc al

    right1:
    cmp user1_dir_bools[2], 00H
    jz down1
    add word ptr user1_posX, bx
    inc ah

    down1:
    cmp user1_dir_bools[4], 00H
    jz left1
    add word ptr user1_posY, bx
    dec al

    left1:
    cmp user1_dir_bools[6], 00H
    jz finish_update1
    sub word ptr user1_posX, bx
    dec ah
    
    finish_update1:

    update_dir1

    mov dh, user1_current_dir
    mov dl, user1_current_frame
    call update_frame
    mov user1_current_frame, bl

ENDM

update_user2_pos MACRO
    ; I am considering up as positive y and down as negative y
    mov ax, 0101h ; ah -> x, al -> y
    mov bx, word ptr user2_speed

    up2:
    cmp user2_dir_bools, 00H
    jz right2
    sub word ptr user2_posY, bx
    inc al

    right2:
    cmp user2_dir_bools[2], 00H
    jz down2
    add word ptr user2_posX, bx
    inc ah

    down2:
    cmp user2_dir_bools[4], 00H
    jz left2
    add word ptr user2_posY, bx
    dec al

    left2:
    cmp user2_dir_bools[6], 00H
    jz finish_update2
    sub word ptr user2_posX, bx
    dec ah
    
    finish_update2:

    update_dir2

    mov dh, user2_current_dir
    mov dl, user2_current_frame
    call update_frame
    mov user2_current_frame, bl

ENDM

update_dir1 MACRO
    ; I am making a table of all compinations of x and y to know and update the direction

    cmp ah, 0
    jz left_dir1
    cmp ah, 2
    jz right_dir1
    cmp al, 0
    jz down_dir1
    cmp al, 2
    jz up_dir1

    jmp done_update1

    up_dir1:
    mov user1_current_dir, 0
    jmp done_update1

    down_dir1:
    mov user1_current_dir, 4
    jmp done_update1

    left_dir1:
    mov user1_current_dir, 5
    add user1_current_dir, al
    jmp done_update1

    right_dir1:
    mov user1_current_dir, 3
    sub user1_current_dir, al

    done_update1:
ENDM

update_dir2 MACRO
    ; I am making a table of all compinations of x and y to know and update the direction

    cmp ah, 0
    jz left_dir2
    cmp ah, 2
    jz right_dir2
    cmp al, 0
    jz down_dir2
    cmp al, 2
    jz up_dir2

    jmp done_update2

    up_dir2:
    mov user2_current_dir, 0
    jmp done_update2

    down_dir2:
    mov user2_current_dir, 4
    jmp done_update2

    left_dir2:
    mov user2_current_dir, 5
    add user2_current_dir, al
    jmp done_update2

    right_dir2:
    mov user2_current_dir, 3
    sub user2_current_dir, al

    done_update2:
ENDM

add_to_frame macro
    ; (current_frame + bl) % 8
    ; current_frame is in dl
    push ax
    push dx
    mov ah, 0
    mov al, dl
    add al, bl
    mov cl, 8
    div cl
    mov bl, ah
    pop dx
    pop ax
endm

detectCollision macro current_frame, car
    local collision
    local else
    local done_obstacle



    ; check for collision with the boundaries 1
    push              di
    mov               al, current_frame
    mov               si, offset car
    mov               bl, BOUNDARY_COLOR1
    call              handleCar
    cmp               bh, 1
    jz                collision

    pop               di  

    ; check for collision with the boundaries 2
    push              di
    mov               al, current_frame
    mov               si, offset car
    mov               bl, BOUNDARY_COLOR2
    call              handleCar
    cmp               bh, 1
    jz                collision

    pop               di

    ; check for collision with the obstacles
    push              di
    mov               al, current_frame
    mov               si, offset car
    mov               bl, OBSTACLE_COLOR
    call              handleCar

    cmp               user_obstacles_passed, 0
    jz                collision


    ; if (bh)
    ;     if (!isColliding)
    ;         mov isColliding, 1
    ; else
    ;     if (isColliding)
    ;         mov user_obstacles_passed, 0

    cmp               bh, 1
    jnz               else
    cmp               isColliding, 0
    jnz               done_obstacle
    mov               isColliding, 1
    jmp               done_obstacle

    else:
    cmp               isColliding, 1
    jnz               done_obstacle
    mov               user_obstacles_passed, 0

    done_obstacle:

    mov bh, 0
    collision:
    pop               di
endm detectCollision

detectPowerUp macro current_frame, car, stored_powerup
    local collision

    mov               cl, stored_powerup

    ; collision with speed power up
    push              di
    push              cx
    mov               al, current_frame
    mov               si, offset car
    mov               bl, POWERUP1_COLOR
    call              handleCar
    cmp               bh, 1
    mov               cl, 1
    jz                collision

    pop              cx
    pop               di
    
    ; collision with slow power up
    push              di
    push              cx
    mov               al, current_frame
    mov               si, offset car
    mov               bl, POWERUP2_COLOR
    call              handleCar
    cmp               bh, 1
    mov               cl, 2
    jz                collision

    pop              cx
    pop              di

    ; collision with generate-obstacle power up
    push              di
    push              cx
    mov               al, current_frame
    mov               si, offset car
    mov               bl, POWERUP3_COLOR
    call              handleCar
    cmp               bh, 1
    mov               cl, 3
    jz                collision

    pop              cx
    pop              di

    ; collision with enable-to-pass power up
    push              di
    push              cx
    mov               al, current_frame
    mov               si, offset car
    mov               bl, POWERUP4_COLOR
    call              handleCar
    cmp               bh, 1
    mov               cl, 4
    jz                collision

    pop              cx
    pop              di
    push             di
    push             cx
    collision:
    mov stored_powerup, cl
    mov si, di
    pop cx
    pop di
    mov di, si
    mov cl, stored_powerup
endm detectPowerUp

getPowerUpColor macro current_powerUp
    local speed_powerUp, slow_powerUp, generate_obstacle_powerUp, enable_to_pass_powerUp, done_powerUp
    cmp current_powerUp, 1
    jz speed_powerUp
    cmp current_powerUp, 2
    jz slow_powerUp
    cmp current_powerUp, 3
    jz generate_obstacle_powerUp
    cmp current_powerUp, 4
    jz enable_to_pass_powerUp
    jmp done_powerUp
    speed_powerUp:
    mov bl, POWERUP1_COLOR
    jmp done_powerUp
    slow_powerUp:
    mov bl, POWERUP2_COLOR
    jmp done_powerUp
    generate_obstacle_powerUp:
    mov bl, POWERUP3_COLOR
    jmp done_powerUp
    enable_to_pass_powerUp:
    mov bl, POWERUP4_COLOR
    done_powerUp:
endm getPowerUpColor

carMainLoop macro

    mov ah, 2Ch
	int 21H    ; puts the millseconds in dl
	mov PrevSec, dh ; contain hundreds of seconds

     again:      
                mov ax,0
                add al,IsUser1Win
                add al,IsUser2Win
                cmp ax,0

                jnz carMainLoop_extend
                jmp carMainLoop_bridge
                   carMainLoop_extend:
                   jmp skipallofthis
                carMainLoop_bridge:   

                call              UpdateUsersScore
                call              UpdatePowerUps

     ; Delay Function
                delayFunc
                call              ShowInGamePowerUps

                call UpdateSpeeds

     user1_check:
     ; update the location of user 1
     ; copy the current states into previous ones
                 mov               ax, user1_posX
                 mov               user1_prev_posX, ax
                 mov               ax, user1_posY
                 mov               user1_prev_posY, ax
                 mov               al, user1_current_frame
                 mov               user1_prev_frame, al
                 mov               al, user1_current_dir
                 mov               user1_prev_dir, al
                 update_user1_pos

     ; check if there is a change or not
                 mov               ax, user1_prev_posX
                 cmp               ax, user1_posX
                 jnz               update1                              ; jump to the update if there is a change
                 mov               ax, user1_prev_posY
                 cmp               ax, user1_posY
                 jz                middle_jump1 
                 jmp update1
     middle_jump1:
                 jmp                far ptr user2_check

     update1:    
     ; remove car and draw background
                 setStartPixel     user1_prev_posX, user1_prev_posY
                 mov               si, offset user1_current_background
                 mov               bx, 0
                 call              setBackground

     ; check for collision
                 call              carCollision
                 cmp bp, 1
                 jz                middle_jump111
                 jmp               no_middle_jump111
                 middle_jump111:
                 jmp               far ptr user1_collision

                 no_middle_jump111:
                 setStartPixel     user1_posX, user1_posY
                 mov               bh, 0


                 mov al, user1_isColliding
                 mov isColliding, al
                 mov al, user1_obstacles_passed
                 mov user_obstacles_passed, al

                 detectCollision   user1_current_frame, car1

                 mov al, user_obstacles_passed
                 mov user1_obstacles_passed, al
                 mov al, isColliding
                 mov user1_isColliding, al


                 cmp               bh, 1

                 jz                middle_jump11
                 jmp               no_middle_jump11
                 middle_jump11:
                 jmp               far ptr user1_collision

                 no_middle_jump11:     
                 detectPowerUp     user1_current_frame, car1, user1_stored_powerup
                 mov               user1_stored_powerup, cl
                 cmp               bh, 1
                 jz                user1_powerUp

                 jmp               user1_no_collision


    user1_powerUp:
                 getPowerUpColor   user1_stored_powerup
                 call              clearPowerUp

    user1_no_collision:
     ; save the background
                 setStartPixel     user1_posX, user1_posY
                 mov               si, offset user1_current_background
                 mov               bx, 1
                 call              setBackground
     ; draw the car
                 setStartPixel     user1_posX, user1_posY
                 mov               si, offset car1
                 mov               al, user1_current_frame
                 mov               bh, 1
                 call              handleCar

                 jmp               user2_check

     user1_collision:  
     ; get previous postions
                 mov               ax, user1_prev_posX
                 mov               user1_posX, ax
                 mov               ax, user1_prev_posY
                 mov               user1_posY, ax
                 mov               al, user1_prev_frame
                 mov               user1_current_frame, al
                 mov               al, user1_prev_dir
                 mov               user1_current_dir, al
     ; draw the car
                 setStartPixel     user1_posX, user1_posY
                 mov               si, offset car1
                 mov               al, user1_current_frame
                 mov               bh, 1
                 call              handleCar

     user2_check:
     ; update the location of user 2
     ; copy the current states into previous ones
                 mov               ax, user2_posX
                 mov               user2_prev_posX, ax
                 mov               ax, user2_posY
                 mov               user2_prev_posY, ax
                 mov               al, user2_current_frame
                 mov               user2_prev_frame, al
                 mov               al, user2_current_dir
                 mov               user2_prev_dir, al
                 update_user2_pos

     ; check if there is a change or not
                 mov               ax, user2_prev_posX
                 cmp               ax, user2_posX
                 jnz               update2                              ; jump to the update if there is a change
                 mov               ax, user2_prev_posY
                 cmp               ax, user2_posY
                 jz                middle_jump2                            ; jump away if there is no change
                 jmp update2
     middle_jump2:
                 jmp far ptr           done_all

     update2:    
     ; remove car and draw background
                 setStartPixel     user2_prev_posX, user2_prev_posY
                 mov               si, offset user2_current_background
                 mov               bx, 0
                 call              setBackground

     ; check for collision
                 call              carCollision
                 cmp bp, 1
                 jz                middle_jump222
                 jmp               no_middle_jump222
                 middle_jump222:
                 jmp               far ptr user2_collision

                 no_middle_jump222:
                 setStartPixel     user2_posX, user2_posY
                 mov               bh, 0


                 mov al, user2_isColliding
                 mov isColliding, al
                 mov al, user2_obstacles_passed
                 mov user_obstacles_passed, al

                 detectCollision   user2_current_frame, car2

                 mov al, user_obstacles_passed
                 mov user2_obstacles_passed, al
                 mov al, isColliding
                 mov user2_isColliding, al
                 
                 
                 cmp               bh, 1
                 jz                middle_jump22
                 jmp               no_middle_jump22
                 middle_jump22:
                 jmp               far ptr user2_collision

                 no_middle_jump22:
                 detectPowerUp     user2_current_frame, car2, user2_stored_powerup
                 mov               user2_stored_powerup, cl
                 cmp               bh, 1
                 jz                user2_powerUp
                 jmp               user2_no_collision


    user2_powerUp:
                 getPowerUpColor   user2_stored_powerup
                 call              clearPowerUp

     user2_no_collision:
     ; save the background
                 setStartPixel     user2_posX, user2_posY
                 mov               si, offset user2_current_background
                 mov               bx, 1
                 call              setBackground
     ; draw the car
                setStartPixel      user2_posX, user2_posY
                 mov               si, offset car2
                 mov               al, user2_current_frame
                 mov               bh, 1
                 call              handleCar

                 jmp               done_all

     user2_collision:  
     ; get previous postions
                 mov               ax, user2_prev_posX
                 mov               user2_posX, ax
                 mov               ax, user2_prev_posY
                 mov               user2_posY, ax
                 mov               al, user2_prev_frame
                 mov               user2_current_frame, al
                 mov               al, user2_prev_dir
                 mov               user2_current_dir, al
     ; draw the car
                 setStartPixel     user2_posX, user2_posY
                 mov               si, offset car2
                 mov               al, user2_current_frame
                 mov               bh, 1
                 call              handleCar
     done_all:   
                call Timer
               ; check for winner         
                    
                call CheckForGameFinish    
                
                skipallofthis:
                cmp               killSignal, 0H
                jnz               kill

               

                jmp               again

     kill:       
     ; restore the original int 9h
     mov killSignal, 0H
                 restoreInt9

endm carMainLoop