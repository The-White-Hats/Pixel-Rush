PUBLIC drawCar

.MODEL SMALL
.STACK 64

;-----------------------

.DATA

    CAR_HEIGHT equ 16
    CAR_WIDTH equ 8

    SCREEN_WIDTH equ 320
    SCREEN_HEIGHT equ 200

    COS_ANGLE equ 985d ; 10 degrees
    SIN_ANGLE equ 174d ; 10 degrees
    centerx dw ?
    centery dw ?
    currentx dw ?
    currenty dw ?
    oldCarPlace db 2*CAR_HEIGHT*CAR_WIDTH dup (?)
    newCarPlace db 2*CAR_HEIGHT*CAR_WIDTH dup (?)

;-----------------------

.CODE
    drawCar PROC FAR

        ; Set video mode
        mov ah,0
        mov al,13h
        int 10h

        ; Draw background
        mov ax,0A000h
        mov es,ax
        mov di,0
        mov cx, 64000
        mov al, 0Ah

        rep stosb ; Fill screen with color

        ; starting position
        mov centerx, SCREEN_WIDTH/2
        mov centery, SCREEN_HEIGHT/2
        mov currentx, SCREEN_WIDTH/2 - CAR_WIDTH/2
        mov currenty, SCREEN_HEIGHT/2 - CAR_HEIGHT/2
        ; pixel => SCREEN_WIDTH*centery + centerx
        mov di, SCREEN_WIDTH
        mov ax, centery
        mul di
        add ax, centerx
        mov di, ax

        ; save coordinates
        call save

        ; Draw car
        call draw

        ; Wait for key
        mov ah,0
        int 16h

        mov ah, 4Ch
        int 21h

    drawCar ENDP

;-----------------------

    save PROC

        mov cx, CAR_HEIGHT
        lea si, oldCarPlace
        back:
            mov dx, CAR_WIDTH
            mov currentx, SCREEN_WIDTH/2 - CAR_WIDTH/2
            back2:
                call getNewPoint
                mov [si], ax
                inc si
                inc si
                inc currentx
                dec dx
                cmp dx, 0
                jnz back2
            inc currenty
            dec cx
            cmp cx, 0
            jnz back

        ret

    save ENDP

;-----------------------

    getNewPoint Proc

        push di
        push si
        push cx
        push dx

        ; x_rotated (di) = (x_original - center_x) * cos(angle) + center_x - (y_original - center_y) * sin(angle)

        mov ax, currentx
        sub ax, centerx
        mov cx, COS_ANGLE
        Imul cx
        mov cx, 1000d ; because cosine is fraction
        Idiv cx
        add ax, centerx
        mov di, ax
        mov ax, currenty
        sub ax, centery
        mov cx, SIN_ANGLE
        Imul cx
        mov cx, 1000d ; because cosine is fraction
        Idiv cx
        sub di, ax

        ; y_rotated (si) = (x_original - center_x) * sin(angle) + center_y + (y_original - center_y) * cos(angle)

        mov ax, currentx
        sub ax, centerx
        mov cx, SIN_ANGLE
        Imul cx
        mov cx, 1000d ; because cosine is fraction
        Idiv cx
        add ax, centery
        mov si, ax
        mov ax, currenty
        sub ax, centery
        mov cx, COS_ANGLE
        Imul cx
        mov cx, 1000d ; because cosine is fraction
        Idiv cx
        add si, ax

        ; pixel => SCREEN_WIDTH*currenty + currentx
        mov bx, SCREEN_WIDTH
        mov ax, si
        mul bx
        add ax, di

        pop dx
        pop cx
        pop si
        pop di

        ret

    getNewPoint ENDP

;-----------------------

    draw Proc

        mov cx, CAR_HEIGHT
        lea si, oldCarPlace
        back_draw:
            mov dx, CAR_WIDTH
            back2_draw:
                mov al, 04h
                mov di, [si]
                mov es:[di], al
                inc si
                inc si
                dec dx
                cmp dx, 0
                jnz back2_draw
            dec cx
            cmp cx, 0
            jnz back_draw

        ret

    draw ENDP

END drawCar