; filename: dots.asm


;  Controls:
;
;  Up/Down Arrows  -  Move Purple Dot 
;  W/S Keys        -  Move Cyan Dot
;  Esc             -  Exit


IDEAL
MODEL small
STACK 100h

DATASEG

; postion of cyan dot
xCyanDot dw 107
yCyanDot dw 100

; position of purple dot
xPurpleDot dw 214
yPurpleDot dw 100

; keyboard scan codes we'll need
KeyEsc    equ 01h
KeyW      equ 11h
KeyS      equ 1Fh
UpArrow   equ 48h
DownArrow equ 50h

KeyList db 128 dup (0)

proc onKeyEvent  ; custom handler for int 09h
    push ax bx
    in   al, 60h
    mov  ah, 0
    mov  bx, ax
    and  bx, 127           ; 7-bit scancode goes to BX ; bh will all be zeros
    shl  ax, 1             ; 1-bit pressed/released goes to AH ; ah will tell if it's pressed or released ; zero = pressed
    xor  ah, 1             ; -> AH=1 Pressed, AH=0 Released
    mov  [KeyList + bx], ah
    mov  al, 20h           ; The non specific EOI (End Of Interrupt)
    out  20h, al
    pop  bx ax
    iret
endp

CODESEG

proc sleepSomeTime
    mov cx, 0
    mov dx, 20000  ; 20ms
    mov ah, 86h
    int 15h  ; param is cx:dx (in microseconds)
    ret
endp

proc drawPurpleDot
    mov al, 5
    mov cx, [xPurpleDot]
    mov dx, [yPurpleDot]
    mov bh, 0h
    mov ah, 0ch
    int 10h

    ret
endp

proc coverPurpleDot
    mov al, 0
    mov cx, [xPurpleDot]
    mov dx, [yPurpleDot]
    mov bh, 0h
    mov ah, 0ch
    int 10h

    ret
endp

proc drawCyanDot
    mov al, 3
    mov cx, [xCyanDot]
    mov dx, [yCyanDot]
    mov bh, 0h
    mov ah, 0ch
    int 10h

    ret
endp

proc coverCyanDot
    mov al, 0
    mov cx, [xCyanDot]
    mov dx, [yCyanDot]
    mov bh, 0h
    mov ah, 0ch
    int 10h

    ret
endp

proc if_Up_isPressedMoveDot
    cmp [byte KeyList + UpArrow], 1
    jne handleUp_end
    
    call coverPurpleDot
    dec [yPurpleDot]
    call drawPurpleDot

    handleUp_end:
    ret
endp

proc if_Down_isPressedMoveDot
    cmp [byte KeyList + DownArrow], 1
    jne handleDown_end
    
    call coverPurpleDot
    inc [yPurpleDot]
    call drawPurpleDot
    
    handleDown_end:
    ret
endp

proc if_W_isPressedMoveDot
    cmp [byte KeyList + KeyW], 1
    jne handleW_end
    
    call coverCyanDot
    dec [yCyanDot]
    call drawCyanDot

    handleW_end:
    ret
endp

proc if_S_isPressedMoveDot
    cmp [byte KeyList + KeyS], 1
    jne handleS_end
    
    call coverCyanDot
    inc [yCyanDot]
    call drawCyanDot

    handleS_end:
    ret
endp

proc main
    call drawPurpleDot
    call drawCyanDot
    
    mainLoop:
        call sleepSomeTime

        call if_Up_isPressedMoveDot
        call if_Down_isPressedMoveDot
        call if_W_isPressedMoveDot
        call if_S_isPressedMoveDot

    ; if Esc is not pressed, jump back to mainLoop
    cmp [byte KeyList + KeyEsc], 1
    jne mainLoop
    
    ret
endp

start:
    mov ax, @data
    mov ds, ax

; enter graphic mode
    mov ax, 13h
    int 10h

; get the address of the existing int09h handler
    mov ax, 3509h ; Get Interrupt Vector
    int  21h ; -> ES:BX
    push es bx

; replace the existing int09h handler with ours
    mov dx, offset onKeyEvent
    mov ax, 2509h
    int 21h

call main

; return to text mode
    mov ah, 0
    mov al, 2
    int 10h

; restore the original int09h handler
    pop dx ds
    mov ax, 2509h
    int 21h

exit:
    mov ax, 4c00h
    int 21h

end start