; Macro to draw a quarter circular sector
; Input: CX - Radius
;        DX - Starting point (X-coordinate in DH, Y-coordinate in DL)
;        AL - Direction (0 for clockwise, 1 for counterclockwise)
; Output: None

circularSector MACRO 
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX

    MOV AX, Radius          ; Copy radius to AX
    MOV BX, Radius          ; Copy radius to BX
    ADD AX, START_X         ; Add starting point to AX (X-coordinate)
    ADD BX, START_Y        ; Add starting point to BX (Y-coordinate)

    MOV CX, 0           ; Initialize loop counter

    ; Loop to draw quarter circular sector
    L1:
        CMP AL, 0       ; Check direction
        JE CLOCKWISE    ; If clockwise, jump to CLOCKWISE label

    COUNTERCLOCKWISE:
        MOV DL, CL      ; Copy loop counter to DL
        MUL DL          ; Multiply loop counter by itself
        DIV BH          ; Divide result by radius
        ADD AX, DX      ; Add starting point to AX (X-coordinate)
        ADD BX, AX      ; Add AX to BX (Y-coordinate)

        pusha
        mov ah, 0ch
        mov al, RED
        mov cx, ax
        mov dx, bx
        int 10h
        popa

        INC CL          ; Increment loop counter
        CMP CL, CH      ; Compare loop counter with radius
        JLE L1          ; If less than or equal to radius, jump to L1 label
        JMP EXIT        ; Jump to EXIT label

    CLOCKWISE:
        MOV DL, CL      ; Copy loop counter to DL
        MUL DL          ; Multiply loop counter by itself
        DIV BH          ; Divide result by radius
        ADD AX, DX      ; Add starting point to AX (X-coordinate)
        SUB BX, AX      ; Subtract AX from BX (Y-coordinate)

        pusha
        mov ah, 0ch
        mov al, RED
        mov cx, ax
        mov dx, bx
        int 10h
        popa

        INC CL          ; Increment loop counter
        CMP CL, CH      ; Compare loop counter with radius
        JLE L1          ; If less than or equal to radius, jump to L1 label

    EXIT:
    POP DX
    POP CX
    POP BX
    POP AX
ENDM
