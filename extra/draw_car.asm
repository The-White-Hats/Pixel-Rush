PUBLIC drawCar

.MODEL SMALL
.STACK 64

;-----------------------

.DATA

    CAR_HEIGHT equ 20h
    CAR_WIDTH equ 10h

    SCREEN_WIDTH equ 320
    SCREEN_HEIGHT equ 200

    ANGLE equ 10h
    carPlace db (CAR_HEIGHT*CAR_WIDTH) dup(?)

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
        mov di, SCREEN_WIDTH*(SCREEN_HEIGHT/2 - CAR_HEIGHT/2) + (SCREEN_WIDTH/2) - CAR_WIDTH/2

        ; save coordinates
        call save

        ; Draw car
        call draw

    drawCar ENDP

;-----------------------

    save PROC

        mov cx, CAR_HEIGHT
        lea bx, carPlace
        back:
            mov dx, CAR_WIDTH
            back2:
                mov [bx], di
                inc bx
                inc di
                dec dx
                cmp dx, 0
                jnz back2
            add di, SCREEN_WIDTH - CAR_WIDTH
        loop back

    save ENDP

;-----------------------

    draw Proc

        mov cx, CAR_HEIGHT
        lea bx, carPlace
        back_draw:
            mov dx, CAR_WIDTH
            back2_draw:
                mov al, 04h
                mov di, [bx]
                mov es:[di], al
                inc bx
                dec dx
                cmp dx, 0
                jnz back2_draw
        loop back

    draw ENDP

END drawCar