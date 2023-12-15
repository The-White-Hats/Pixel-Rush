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
     mov [bx],'$' 
     mov cx, bx
     lea dx,s
     sub cx,dx  
     dec bx
     l2:
         mov ah,2
         mov dl,[bx]
         int 21h
         dec bx
         cmp dh,0
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
    mov     ax, START_CAR_Y
    mov     user1_posY, ax

    ; save the background
    setStartPixel user1_posX, user1_posY
    mov     bx, 1
    call    setBackground

    ; draw the car
    call    drawCar
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

add_to_frame macro
    ; (current_frame + bl) % 8

    mov ah, 0
    mov al, current_frame
    add al, bl
    mov cl, 8
    div cl
    mov bl, ah
endm

carMainLoop macro

    again:
    ; Delay Function
             delayFunc

    ; update the location
    ; copy the current postions into prev_postions
             mov                 ax, user1_posX
             mov                 prev_user1_posX, ax
             mov                 ax, user1_posY
             mov                 prev_user1_posY, ax
             update_user1_pos

    ; check if there is a change or not
             mov                 ax, prev_user1_posX
             cmp                 ax, user1_posX
             jnz                 update1                             ; jump to the update if there is a change
             mov                 ax, prev_user1_posY
             cmp                 ax, user1_posY
             jz                  done_all                            ; jump away if there is no change

    update1: 
    ; remove car and draw background
             setStartPixel prev_user1_posX, prev_user1_posY
             mov                bx, 0
             call               setBackground

    ; save the background
             setStartPixel user1_posX, user1_posY
             mov                bx, 1
             call               setBackground
    ; draw the car
             call               drawCar

    done_all:
             cmp                 killSignal, 0H
             jnz                 kill
 
             jmp                 again

    kill:    
    ; restore the original int 9h
             restoreInt9

endm carMainLoop