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

; endl macro 
;        mov ah,02h
;     mov dl,10
;     int 21h 
;     mov ah,02h
;     mov dl,13
;     int 21h
; endm endl

; getstr macro str
;     mov ah,0ah
;     lea dx,str
;     int 21h
; endm getstr

; shownum macro str
;     local l1     
;     local l2
;      lea bx, str
;      mov cx,10
;      mov dx,0
;      l1:
;        mov dx,0
;        div cx     
;        add dl,30h
;        mov [bx],dl
;        inc bx 
;        cmp ax,0
;      jnz l1     
;      mov [bx],'$' 
;      mov cx, bx
;      lea dx,str
;      sub cx,dx  
;      dec bx
;      l2:
;          mov ah,2
;          mov dl,[bx]
;          int 21h
;          dec bx
;          cmp dh,0
;      loop l2      
; endm shownum
  
; clear macro 
;     mov ah, 0       ; AH=0 is the function for setting video mode
;     mov al, 3       ; AL=3 sets 80x25 text mode (standard text mode)
;     int 10h         ; BIOS interrupt for video services
; endm clear 

; moveup macro  
;     local skip
;     mov ah,3h
;     mov bh,0h
;     int 10h
;     mov ah,02h
;     inc dl
;     int 10h
; endm moveup 

; moveback macro  
;     local skip
;     mov ah,3h
;     mov bh,0h
;     int 10h
;     mov ah,02h
;     dec dl
;     int 10h
; endm moveback 

; change macro  
;    mov ah,9
;    mov bh,0
;    mov al,00h
;    mov cx,1
;    mov bl,0fah
;    int 10h
;    moveup
;    mov ah,9
;    mov bh,0
;    mov al,00h
;    mov cx,1
;    mov bl,0fah
;    int 10h
;    moveback
; endm change 

; reset macro  
;    mov ah,9
;    mov bh,0
;    mov al,44h
;    mov cx,0
;    mov bl,0F0h
;    int 10h
; endm reset 

; colorNamePosition macro ;changes the foreground color of each char place in the second line
;     local loop1
;     ;set cursor to second line char 8
;     mov ah, 02h
;     mov dh, 1
;     mov dl, 8
;     int 10h

;     mov bl,00h
;     mov ah,9 
;     mov bh,0 
;     mov al,00h
;     mov dx,16
;     loop1:
;     mov cx,1
;     int 10h
;     inc bh
;     dec dx
;     cmp dx,0
;     jnz loop1

;     mov ah, 02h
;     mov dh, 1
;     mov dl, 8
;     int 10h
; endm colorNamePosition

; checkIfAllNumbers macro buffer, error
;     local valid
;     local notValid
;     local finish
;     local invalid
;     invalid:
;     mov si, offset buffer+2
;     mov cx, 0
;     mov cl, buffer+1
;     cmp cl, 0
;     je notValid
;     inc cx
;     valid:
;         dec cx
;         cmp cx, 0
;         je finish
;         lodsb
;         cmp al, '0'
;         jl notValid
;         cmp al, '9'
;         jle valid
;     notValid:
;         ;move cursor to middle of screen
;         mov ah, 02h
;         mov dh, 12
;         mov dl, 40
;         int 10h
;         showMsgClr error, 0ch

;         mov dh, 5 ; line to be cleared
;         clearLineArea
;         mov ah, 02h
;         mov dh, 5
;         mov dl, 8
;         int 10h
;         getstr buffer
;         jmp invalid
;     finish:
;         mov ah, 02h
;         mov dh, 12
;         mov dl, 40
;         int 10h
;         clearLineArea
;         mov ah, 02h
;         mov dh, 3
;         mov dl, 8
;         int 10h
; endm checkIfAllNumbers


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

    cmp cx, 0
    je finish
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
        paintAreaM 320d, 336d, 140d, 16d, 01H  ; paint input area in black

        resetBuffer buffer, nameSize, '$'

        MoveCursor nameInputX, nameInputY       ; return the cursor
        getSizedStrM buffer, 15d
        jmp invalid
    finish:
endm checkFirstChar

; checkNameChars macro buffer, error
;     local valid
;     local notValid
;     local finish
;     local invalid
;     invalid:
;     mov si, offset buffer+2
;     mov cx, 0
;     mov cl, buffer+1
;     cmp cl, 0
;     je notValid
;     inc cx
;     valid:
;         dec cx
;         cmp cx, 0
;         je finish
;         lodsb
;         cmp al, 'A'
;         jl notValid
;         cmp al, 'Z'
;         jle valid
;         cmp al, 'a'
;         jl notValid
;         cmp al, 'z'
;         jle valid
;     notValid:
;         ;move cursor to middle of screen
;         mov ah, 02h
;         mov dh, 12
;         mov dl, 40
;         int 10h
;         showMsgClr error, 0ch

;         mov dh, 1 ; line to be cleared
;         clearLineArea
;         mov ah, 02h
;         mov dh, 1
;         mov dl, 4
;         int 10h
;         getstr buffer
;         jmp invalid
;     finish:
;         mov ah, 02h
;         mov dh, 12
;         mov dl, 40
;         int 10h
;         clearLineArea
;         mov ah, 02h
;         mov dh, 3
;         mov dl, 4
;         int 10h

    
; endm checkNameChars

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
ENDM

;get a string in a buffer with a limit on size
getSizedStr PROC
    ; set the input size in cx and the buffer in di
    push di
    xor bh, bh
    getSizedStrLoop:
    mov ah, 1h          ;get the char
    int 21h
    cmp al, 0DH         ;pressed enter
    je getSizedStrEnd   
    cmp al, 8H          ;pressed backspace
    jne notBackspace
    dec di
    mov [di], byte ptr '$'
    dec bh
    showchar 0
    showchar 8H
    jmp getSizedStrLoop
    notBackspace:
    stosb
    inc bh
    cmp bh, ch
    jnz getSizedStrLoop
    getSizedStrEnd:
    pop di
    add di, 16d
    mov [di], bh 
    ret
getSizedStr ENDP

resetBuffer MACRO name, bufferSize, value
    lea di, name
    mov cx, bufferSize
    mov al, value
    rep stosb
ENDM

ReadVarLenNum MACRO num
    local INPUT
    local NEXT
    ; the var must be a word to receive up to 65535
    MOV num, 0
    MOV BX, 10
    INPUT:
    MOV AH, 1
    INT 21H
    CMP AL, 13
    JE NEXT
    SUB AL, 30H
    MOV AH, 0
    Mov CX, AX
    MOV AX, num
    MUL BX
    ADD AX, CX
    MOv num,AX
    JMP INPUT
    NEXT:
ENDM