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
  
shownum macro
    local l1     
    local l2
     lea bx, s
     mov cx,10
     mov dx,0
     l1:
       mov dx,0
       div cx     
       add dl,30h
       mov [bx],dl
       inc bx 
       cmp ax,0
     jnz l1     
     mov byte ptr [bx],'$' 
     mov cx, bx
     lea dx,s
     sub cx,dx  
     dec bx
     l2:
         mov ah,2
         mov dl,[bx]
         int 21h
         dec bx
     loop l2      
endm shownum
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
        add  dx, FILE_NAME_LEN
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

    update_dir1

    mov dh, user1_current_dir
    mov dl, user1_current_frame
    call update_frame
    mov user1_current_frame, bl

ENDM

update_user2_pos MACRO
    ; I am considering up as positive y and down as negative y
    mov ax, 0101h ; ah -> x, al -> y

    up2:
    cmp user2_dir_bools, 00H
    jz right2
    dec word ptr user2_posY
    inc al

    right2:
    cmp user2_dir_bools[2], 00H
    jz down2
    inc word ptr user2_posX
    inc ah

    down2:
    cmp user2_dir_bools[4], 00H
    jz left2
    inc word ptr user2_posY
    dec al

    left2:
    cmp user2_dir_bools[6], 00H
    jz finish_update2
    dec word ptr user2_posX
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
    ; check for collision with the boundaries 1
    push              di
    mov               bl, BOUNDARY_COLOR1
    mov               al, current_frame
    mov               si, offset car
    call              handleCar
    cmp               bh, 1
    jz                collision

    ; check for collision with the boundaries 2
    pop               di
    push              di
    mov               bl, BOUNDARY_COLOR2
    mov               al, current_frame
    mov               si, offset car
    call              handleCar
    cmp               bh, 1
    jz                collision

    ; check for collision with the obstacles
    pop               di
    push              di
    mov               bl, OBSTACLE_COLOR
    mov               al, current_frame
    mov               si, offset car
    call              handleCar
    cmp               bh, 1
    jz                collision

    collision:
    pop               di
endm detectCollision

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

     ; Delay Function
                delayFunc
                call              ShowInGamePowerUps

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
                 jz                middle_jump1                            ; jump away if there is no change

     update1:    
     ; remove car and draw background
                 setStartPixel     user1_prev_posX, user1_prev_posY
                 mov               si, offset user1_current_background
                 mov               bx, 0
                 call              setBackground

     ; check for collision
                 setStartPixel     user1_posX, user1_posY
                 mov               bh, 0
                 detectCollision   user1_current_frame, car1
                 cmp               bh, 1
                 jz                user1_collision
                 jmp               user1_no_collision

     middle_jump1:
                 jmp               user2_check

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

     update2:    
     ; remove car and draw background
                 setStartPixel     user2_prev_posX, user2_prev_posY
                 mov               si, offset user2_current_background
                 mov               bx, 0
                 call              setBackground

     ; check for collision
                 setStartPixel     user2_posX, user2_posY
                 mov               bh, 0
                 detectCollision   user2_current_frame, car2
                 cmp               bh, 1
                 jz                user2_collision
                 jmp               user2_no_collision

     middle_jump2:
                 jmp               done_all

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