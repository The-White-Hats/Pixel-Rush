
.MODEL SMALL
.STACK 64
;------------------------------------------

.DATA
filename db 'Car_Blue.bin', 0
buffer_size equ 500h
buffer db buffer_size dup(?)

errtext db "Something Went Wrong!!!", 10, "$"

IMAGE_HEIGHT equ 50h
IMAGE_WIDTH equ 30h

SCREEN_WIDTH equ 320
SCREEN_HEIGHT equ 200

;------------------------------------------

.CODE
readFile PROC
    mov ax,@DATA
    mov DS,ax

    mov ah, 03Dh
    mov al, 0               ; open attribute: 0 - read-only, 1 - write-only, 2 -read&write
    mov dx, offset filename ; ASCII filename to open
    int 21h

    jc error_exit           ; Jump if carry flag set (error)

    mov bx, AX
    mov ah, 03Fh
    mov cx, buffer_size     ; number of bytes to read
    mov dx, offset buffer   ; were to put read data
    int 21h


    ; Check for errors
    jc error_exit       ; Jump if carry flag set (error)

    mov ah, 3Eh         ; DOS function: close file
    INT 21H

    MOV DI,320/2 - IMAGE_WIDTH/2 ;STARTING PIXEL
    CALL drawCar

    MOV ah, 0
    INT 16h
    jmp exit

    error_exit:
    mov ah, 9
    mov dx, offset errtext
    int 21h

    exit:
    MOV ah,4ch
    INT 21h
readFile ENDP

drawCar PROC

    ; Set video mode
    mov ah,0
    mov al,13h
    int 10h

    push di ; save starting position

    ; Draw car
    mov ax,0A000h
    mov es,ax
    mov di,0
    mov cx, 64000
    mov al, 0Ah

    rep stosb ; Fill screen with color

    pop di

    mov si, offset buffer
    mov dx, IMAGE_HEIGHT

    REPEAT:
    mov cx, IMAGE_WIDTH

    DRAW_PIXELS:
        ; Check if the byte at [SI] is 250 TO SKIP IT
        mov ah, BYTE PTR [SI]
        CMP BYTE PTR [SI], 250
        JE SKIP_DRAW

        ; Draw the pixel
        MOVSB
        JMP DECC

        SKIP_DRAW:
        INC DI
        INC SI

        DECC:
        DEC CX

        JNZ DRAW_PIXELS

    ADD DI, SCREEN_WIDTH - IMAGE_WIDTH
    DEC DX
    JNZ REPEAT

    RET

drawCar ENDP

END readFile