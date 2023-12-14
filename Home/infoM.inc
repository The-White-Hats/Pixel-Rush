GetPlayersNames MACRO
    pusha
    paintAreaM 210d, 325d, 220d, 50d, 09H  ; paint input area in black
    paintAreaM 212d, 325d, 216d, 50d, 0H  ; paint input area in black

    readImageM firstPN
    draw 220, 305, 199, 15

    ; showColoredMsg mes1, 0fH, mes1size, mes1y, mes1x
    MoveCursor nameInputX, nameInputY
    getSizedStrM name1, 15d
    checkFirstChar name1, nameWarning
    paintAreaM 201, 305, 238, 17, 2AH   ; clear 'player 1 name' logo
    popa

    ; second player
    pusha
    readImageM secondPN
    draw 220, 305, 199, 15
    paintAreaM 212d, 325d, 216d, 50d, 0H  ; paint input area in black

    ; showColoredMsg mes12, 0fH, mes1size, mes1y, mes1x
    MoveCursor nameInputX, nameInputY
    getSizedStrM name2, 15d
    checkFirstChar name2, nameWarning

    paintAreaM 210d, 325d, 220d, 50d, 2AH  ; clear the error area
    paintAreaM 201, 305, 238, 17, 2AH 
    paintAreaM 135, 464, 369, 11, 2AH 
    popa
ENDM

showmes macro str
    mov ah,09h
    lea dx,str
    int 21h  
endm showmes 

showColoredMsg MACRO str, color, strSize, row, column
    lea bp, str
    mov cx, strSize

    ; Print the string in red color at position (100, 50)
    mov ah, 13h
    mov al, 0 ; sub-function 0: use BL as color and do not update cursor
    mov bh, 0 ; page number
    mov bl, color ; red color
    mov dh, row ; row
    mov dl, column ; column
    int 10h
ENDM

showchar macro char
    mov ah,02h
    mov dl,char
    int 21h  
endm showchar 

checkFirstChar macro buffer, error
    local notValid
    local finish
    local invalid
    
    invalid:
    lea si, buffer
    mov cx, 0
    mov cl, [buffer+16] ; load buffer size
    cmp cl, 0           ; check if the buffer size is zero
    jz notValid

    lodsb
    cmp al, 'A'
    jl notValid
    cmp al, 'Z'
    jle finish
    cmp al, 'a'
    jl notValid
    cmp al, 'z'
    jle finish

    notValid:
        showColoredMsg nameWarning, 0ch, warningSize, warningY, warningX

        ; clearLineArea nameSize
        paintAreaM 250d, 335d, 140d, lineHeight, 0H  ; paint input area in black

        resetBuffer buffer, nameSize, '$'

        MoveCursor nameInputX, nameInputY       ; return the cursor
        getSizedStrM buffer, 15d
        jmp invalid
    finish:
endm checkFirstChar

clearLineArea macro width 
    mov ah,9        ; Display
    mov bh,0        ; Page 0
    mov al,00h      ; null
    mov cx, width   ; number of times to print
    mov bl,0A0h      ; color
    int 10h 
endm clearLineArea

MoveCursor MACRO x, y
    pusha
    mov dl, x
    mov dh, y
    mov ah, 2H
    int 10h 
    popa
ENDM

getSizedStrM MACRO buffer, bufferSize
    lea di, buffer
    mov ch, bufferSize
    call getSizedStr
    add di, bufferSize
    inc di
    mov [di], bh
ENDM

;get a string in a buffer with a limit on size
getSizedStr PROC
    ; set the input size in cx and the buffer in di
    push di
    xor bh, bh
    getSizedStrLoop:
    mov ah, 0h          ; get the char
    int 16h
    showchar al
    cmp al, 0DH         ; pressed enter
    je getSizedStrEnd   
    cmp al, 8H          ; pressed backspace
    jne notBackspace
    cmp bh, 0           ; handle pressing backspace when no previous input
    showchar 0
    jz getSizedStrLoop
    dec di
    mov [di], byte ptr '$'
    dec bh
    showchar 8H
    jmp getSizedStrLoop
    notBackspace:
    stosb
    inc bh
    cmp bh, ch
    jnz getSizedStrLoop
    getSizedStrEnd:
    pop di 
    ret
getSizedStr ENDP

resetBuffer MACRO name, bufferSize, value
    lea di, name
    mov cx, bufferSize
    mov al, value
    rep stosb
ENDM
