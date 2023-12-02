horizontalPart macro Xpos, Ypos, dir
    local drawLine
    local drawRoad
    local check1
    local check2
    local drawRed
    local drawWhite
    local drawDashes
    local alter
    local left
    local right
    local next
    local mid1
    local mid2

    cmp dir, 0 
    jnz leftCheck
    cmp Xpos + LINE_LENGTH, MAX_X
    jg start
    jmp ender
    leftCheck:
    cmp Xpos - LINE_LENGTH, MIN_X
    jl start
    jmp ender

    start:
    mov ah, 0ch
    mov si, LINE_WIDTH + 2*BOUNDARY_WIDTH
    mov dx, Ypos
    drawRoad:
            mov cx, Xpos
            mov di, LINE_LENGTH
            mov bl, BOUNDARY_LENGTH
            mov bh, 2*DASHEDLINE_LENGTH
            mov al, GRAY 
            cmp si, LINE_WIDTH + BOUNDARY_WIDTH ;check if upper bound
            jle check1
            mov al, RED
        check1:
            cmp si, 2 ;check if lower bound
            jge check2
            mov al, RED 
        check2:
            cmp si, LINE_WIDTH/2 +BOUNDARY_WIDTH+1 ;check if middle 
            jnz drawLine
            mov al , DASHESCOLOR ;white
        drawLine:
            cmp al, GRAY
            jnz drawRed
            int 10h
            jmp next
            drawRed:
                cmp al, RED
                jnz drawWhite
                int 10h
                dec bl
                cmp bl, 0
                jnz next
                mov bl, BOUNDARY_LENGTH
                mov al, WHITE
                jmp next
            mid1:
                jmp drawLine
            mid2:
                jmp drawRoad
            drawWhite:
                cmp al, WHITE
                jnz drawDashes
                int 10h
                dec bl
                cmp bl, 0
                jnz next
                mov bl, BOUNDARY_LENGTH
                mov al, RED
                jmp next
            drawDashes:
                cmp bh, DASHEDLINE_LENGTH 
                jg alter
                int 10h
                dec bh
                cmp bh, 0
                jnz next
                mov bh, 2*DASHEDLINE_LENGTH
                jmp next
                alter:
                    mov al, GRAY
                    int 10h
                    dec bh
                    mov al, DASHESCOLOR
            next:
            cmp dir, 0 ;check if right
            jnz left
            inc cx
            jmp right
            left:
                dec cx
            right:
            dec di
            cmp di, 0
            jnz mid1
        dec dx
        dec si
        cmp si, 0
        jnz mid2
    ender:
endm horizontalPart

deletePart macro posX1, posY1, posX2, posY2
mov ah, 0ch
mov al, GREEN
mov si, posY2
mov di, posX2
mov dx, posY1
delete1:
    mov cx, posX1
    delete:
        int 10h
        inc cx
        cmp cx, di
        jle delete
    inc dx
    cmp dx, si
    jle delete1
endm deletePart