.MODEL SMALL
.STACK 64

;-----------------------

.DATA

    CAR_HEIGHT equ 120
    CAR_WIDTH equ 85

    SCREEN_WIDTH equ 320
    SCREEN_HEIGHT equ 200

    COS_ANGLE equ 985d ; 10 degrees
    SIN_ANGLE equ 174d ; 10 degrees
    
    centerx equ SCREEN_WIDTH/2
    centery equ SCREEN_HEIGHT/2

    currentx db ?
    currenty db ?
    carPlaceX db CAR_HEIGHT*CAR_WIDTH dup (?)
    carPlaceY db CAR_HEIGHT*CAR_WIDTH dup (?)

;-----------------------

.CODE

    drawCar PROC FAR
        mov ax, @data
        mov ds, ax

        ; Set video mode
        mov ah,0
        mov al,13h
        int 10h

        ; Draw background
        mov ax,0A000h
        mov es,ax

        ; starting position
        mov currentx, centerx - CAR_WIDTH/2
        mov currenty, centery - CAR_HEIGHT/2

        ; Initialize car

        mov cx, CAR_HEIGHT
        lea di, carPlaceX
        lea si, carPlaceY
        back_init:
            mov dx, CAR_WIDTH
            mov currentx, centerx - CAR_WIDTH/2
            back2_init:
                mov ah, 0
                mov al, currentx
                mov [di], al
                mov al, currenty
                mov [si], al
                inc si
                inc di
                mov al, currentx
                inc al
                mov currentx, al
                dec dx
                cmp dx, 0
                jnz back2_init
            mov al, currenty
            inc al
            mov currenty, al
            dec cx
            cmp cx, 0
            jnz back_init



        mov cx, 20
        again:
            push cx

            ; clear car
            mov bp, 00h ; black color
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

        mov ah, 4ch
        int 21h

    drawCar ENDP

;-----------------------

    save PROC

        mov cx, CAR_HEIGHT
        lea di, carPlaceX
        lea si, carPlaceY
        back:
            mov dx, CAR_WIDTH
            back2:
                mov al, [di]
                mov currentx, al
                mov al, [si]
                mov currenty, al
                call getNewPoint
                mov [di], al
                mov [si], bl
                inc si
                inc di
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

        mov ah, 0
        mov al, currentx
        sub al, centerx
        mov cx, COS_ANGLE
        Imul cx
        mov cx, 1000d ; because cosine is fraction
        Idiv cx
        add al, centerx
        mov di, ax
        mov ah, 0
        mov al, currenty
        sub al, centery
        mov cx, SIN_ANGLE
        Imul cx
        mov cx, 1000d ; because cosine is fraction
        Idiv cx
        sub di, ax

        ; y_rotated (si) = (x_original - center_x) * sin(angle) + center_y + (y_original - center_y) * cos(angle)

        mov ah, 0
        mov al, currentx
        sub al, centerx
        mov cx, SIN_ANGLE
        Imul cx
        mov cx, 1000d ; because cosine is fraction
        Idiv cx
        add al, centery
        mov si, ax
        mov ah, 0
        mov al, currenty
        sub al, centery
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
                mov ah, 0
                mov al, [si]
                mov ah, 0
                mul bx
                mov bh, 0
                mov bl, [di]
                add bx, ax

                pop dx

                mov ax, bp
                mov es:[bx], al
                inc si
                inc di
                dec dx
                cmp dx, 0
                jnz back2_draw
            dec cx
            cmp cx, 0
            jnz back_draw

        ret

    draw ENDP

END drawCar