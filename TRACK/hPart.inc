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
    local mid3
    local ender
    local boundaryOuter
    local boundary
    


    mov ah, 0ch
    mov si, LINE_WIDTH + 2*BOUNDARY_WIDTH ; Outer loop counter
    mov dx, Ypos
    drawRoad:
            mov cx, Xpos
            mov di, LINE_LENGTH ; Inner loop counter
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
            mov al , DASHESCOLOR ;anything but white or gray or red
        drawLine:
            cmp al, GRAY ;check if normal road
            jnz drawRed
            int 10h
            jmp next
            drawRed:
                cmp al, RED ;check if red boundary
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
                cmp al, WHITE ;check if white boundary
                jnz drawDashes
                int 10h
                dec bl
                cmp bl, 0
                jnz next
                mov bl, BOUNDARY_LENGTH
                mov al, RED
                jmp next
            drawDashes:
                cmp bh, DASHEDLINE_LENGTH ;check if normal road or dashed line
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
    cmp cornerFlag, 1
    jnz mid3
    mov cx, Xpos
    mov dx, Ypos
    mov si, BOUNDARY_WIDTH 
    mov bl, BOUNDARY_LENGTH
    mov al, RED
    boundaryOuter:
        mov dx, Ypos
        sub dx, LINE_WIDTH
        dec dx
        mov di, LINE_WIDTH + 2*BOUNDARY_WIDTH 
        boundary:
        drawRed2:
            cmp al, RED ;check if red boundary
            jnz drawWhite2
            int 10h
            dec bl
            cmp bl, 0
            jnz boundaryNext
            mov bl, BOUNDARY_LENGTH
            mov al, WHITE
            jmp boundaryNext
        drawWhite2:
            cmp al, WHITE ;check if white boundary
            jnz drawDashes
            int 10h
            dec bl
            cmp bl, 0
            jnz boundaryNext
            mov bl, BOUNDARY_LENGTH
            mov al, RED
            jmp boundaryNext
            mid3:
            jmp final
    boundaryNext:
        dec di
        inc dx
        cmp di, 0
        jnz boundary
    dec si
    inc cx
    cmp si, 0
    jnz boundaryOuter

    ;open a way 
    mov al, GRAY
    cmp cornerType, 0
    jnz upOpened
    mov dx, Ypos
    jmp exec

    upOpened:
    mov dx, Ypos
    sub dx, LINE_WIDTH
    sub dx, BOUNDARY_WIDTH

    exec:
    mov si, BOUNDARY_WIDTH

    open1:
    
        mov di, LINE_WIDTH
        mov cx, Xpos
        cmp dir,1
        jnz addd
        dec cx
        jmp open11
        addd:
        inc cx
        open11:
        int 10h
        cmp dir,0
        jz goRight
        dec cx
        jmp loopConds
        goRight:
        inc cx
        loopConds:
        dec di
        cmp di,0
        jnz open11
    inc dx
    dec si
    cmp si,0
    jnz open1

    final:
endm horizontalPart
