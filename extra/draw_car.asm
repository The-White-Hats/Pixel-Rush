.MODEL SMALL
.STACK 64

;-----------------------

.DATA

    CAR_HEIGHT equ 10
    CAR_WIDTH equ 5

    SCREEN_WIDTH equ 320
    SCREEN_HEIGHT equ 200

    COS_ANGLE equ 985d ; 10 degrees
    SIN_ANGLE equ 174d ; 10 degrees
    centerx dw ?
    centery dw ?
    currentx dw ?
    currenty dw ?
    carPlaceX dw CAR_HEIGHT*CAR_WIDTH dup (?)
    carPlaceY dw CAR_HEIGHT*CAR_WIDTH dup (?)

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

        ; Initialize car
        call initCar
        mov cx, 20

        again:
            push cx

            ; clear car
            mov bp, 0Ah ; black color
            call draw

            pop cx
            cmp cx, 20
            push cx
            jz skip

            ; save new coordinates
            call save

            skip:

            ; Draw car
            mov bp, 4h ; red color
            call draw

            ; Wait for key
            mov ah,0
            int 16h

            pop cx

        loop again

        mov ah, 4Ch
        int 21h

    drawCar ENDP

;-----------------------

    initCar PROC

        mov cx, CAR_HEIGHT
        lea di, carPlaceX
        lea si, carPlaceY
        back_init:
            mov dx, CAR_WIDTH
            mov currentx, SCREEN_WIDTH/2 - CAR_WIDTH/2
            back2_init:
                mov ax, currentx
                mov [di], ax
                mov ax, currenty
                mov [si], ax
                add di, 2
                add si, 2
                inc currentx
                dec dx
                cmp dx, 0
                jnz back2_init
            inc currenty
            dec cx
            cmp cx, 0
            jnz back_init

        ret

    initCar ENDP

;-----------------------


    save PROC

        mov cx, CAR_HEIGHT
        lea di, carPlaceX
        lea si, carPlaceY
        back:
            mov dx, CAR_WIDTH
            back2:
                mov ax, [di]
                mov currentx, ax
                mov ax, [si]
                mov currenty, ax
                call getNewPoint
                mov [di], ax
                mov [si], bx
                add di, 2
                add si, 2
                dec dx
                cmp dx, 0
                jnz back2
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

        mov ax, di
        mov bx, si

        pop dx
        pop cx
        pop si
        pop di

        ret

    getNewPoint ENDP

;-----------------------

    draw Proc

        mov cx, CAR_HEIGHT
        lea di, carPlaceX
        lea si, carPlaceY
        back_draw:
            mov dx, CAR_WIDTH
            back2_draw:
                ; pixel => SCREEN_WIDTH*currenty + currentx
                push dx

                mov bx, SCREEN_WIDTH
                mov ax, [si]
                mul bx
                add ax, [di]
                mov bx, ax

                pop dx

                mov ax, bp
                mov es:[bx], al
                add di, 2
                add si, 2
                dec dx
                cmp dx, 0
                jnz back2_draw
            dec cx
            cmp cx, 0
            jnz back_draw

        ret

    draw ENDP

END drawCar