GetPlayersNames MACRO
    pusha
    paintAreaM 210d, 325d, 220d, 50d, 09H  ; paint input area in black
    paintAreaM 212d, 325d, 216d, 50d, 0H  ; paint input area in black

    readImageM firstPN
    draw 220, 305, 199, 15

    MoveCursor nameInputX, nameInputY
    getSizedStrM user1name, 15d
    checkFirstChar user1name, nameWarning
    popa

    ; second player
    pusha
    readImageM secondPN
    draw 220, 305, 199, 15                ; draw second player input msg
    paintAreaM 212d, 325d, 216d, 50d, 0H  ; paint input area in black

    MoveCursor nameInputX, nameInputY
    getSizedStrM user2name, 15d
    checkFirstChar user2name, nameWarning

    putWallPaperM 7, 2, 3*bgSize, 6*bgSize  ; recover the area with the wallpaper

    paintAreaM 0, 430d, 640d, 50d, 0H
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
ENDM showColoredMsg

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
    ; set the input size in ch and the buffer in di
    push di
    xor bh, bh
    getSizedStrLoop:
    mov ah, 0h          ; get the char
    int 16h
    
    cmp al, 0DH         ; pressed enter
    je getSizedStrEnd   
    cmp al, 8H          ; pressed backspace
    jne notBackspace
    showchar al
    showchar 0
    cmp bh, 0           ; handle pressing backspace when no previous input
    jz getSizedStrLoop
    dec di
    mov [di], byte ptr '$'
    dec bh
    showchar 8H
    jmp getSizedStrLoop
    notBackspace:
    inc bh
    cmp bh, ch
    jl getSizedStrSkipInc
    dec bh
    jmp getSizedStrLoop
    getSizedStrSkipInc:
    showchar al
    stosb
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
