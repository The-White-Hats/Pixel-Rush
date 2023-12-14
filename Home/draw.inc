drawLogo MACRO
    readImageM logoUL
    draw 200, 20, 120, 120
    readImageM logoUR
    draw 320, 20, 120, 120
    readImageM logoLL
    draw 200, 140, 120, 120
    readImageM logoLR
    draw 320, 140, 120, 120
    readImageM hackers
    draw 135, 464, 369, 11
ENDM

drawButtons MACRO
    pusha
    readImageM F1button
    draw 165, 305, 60, 34
    readImageM F2button
    draw 290, 305, 60, 34
    readImageM Exitbutton
    draw 415, 305, 60, 34
    popa
ENDM

; draw the image present in the buffer ; you give it the starting coordinates and width and height
draw MACRO x, y, width, height
    ; set the upper left corer and the borders of drawing
    mov Xstart, x
    mov Ystart, y
    mov Xlimit, x
    add Xlimit, width
    mov Ylimit, y
    add Ylimit, height
    ; call the drawing proc
    call drawP
ENDM

; set the offset of the file ; call readImagP ; check for error
readImageM MACRO filename
    lea dx, filename
    call readImageP
ENDM

; call all the necessary procedures to read an image
readImageP PROC
    call openFile
    noError1:
    call readData
    noError2:
    call closeFile
    ret
readImageP ENDP

;opens a file given the offset of the file name in dx
; you need first to move the offset of the file name to dx 
; lea dx, filename
openFile PROC
    mov ah, 03Dh
    mov al, 0 ; open attribute: 0 - read-only, 1 - write-only, 2 -read&write
    int 21h
    ret
openFile ENDP

; read the data form the opened file
readData PROC
    mov bx, AX
    mov ah, 03Fh
    mov cx, buffer_size ; number of bytes to read
    mov dx, offset buffer ; were to put read data
    int 21h
    ret
readData ENDP

closeFile PROC
    mov ah, 3Eh         ; DOS function: close file
    INT 21H
    ret
closeFile ENDP

; draws an image using interrupt
drawP PROC
    lea si, buffer
    mov dx, Ystart
    mov ah, 0ch
    rowLoop:
    mov cx, Xstart
    columnLoop:
    mov al, [si]
    inc si
    cmp al, 250D
    je skip
    int 10h
    skip:
    inc cx
    cmp cx, Xlimit
    jne columnLoop
    inc dx
    cmp dx, Ylimit
    jnz rowLoop
    ret
drawP ENDP

UltraGraphicsMode MACRO 
  mov AX, 4F02h
  mov BX, 0101h  ; 640x480x256
  int 10h        ; BIOS interrupt for video services
ENDM

paintAreaM MACRO x, y, width, height, color
    mov Xstart, x
    mov Ystart, y
    mov Xlimit, x
    add Xlimit, width
    mov Ylimit, y
    add Ylimit, height
    mov al, color

    call paintAreaP
ENDM

paintAreaP PROC
    mov dx, Ystart
    mov ah, 0ch
    paintAreaRowLoop:
    mov cx, Xstart
    paintAreaColumnLoop:
    int 10h
    inc cx
    cmp cx, Xlimit
    jne paintAreaColumnLoop
    inc dx
    cmp dx, Ylimit
    jnz paintAreaRowLoop
    ret
paintAreaP ENDP