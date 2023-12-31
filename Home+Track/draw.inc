putWallPaperM MACRO bgCR1, bgRR1, bgCS1, bgRS1
    mov bgCR, bgCR1
    mov bgRR, bgRR1
    mov bgCS, bgCS1
    mov bgRS, bgRS1
    readImageM wallpaper
    call putWallPaperP
ENDM

drawLogo MACRO
    readImageM logoUL
    draw 200, 20, 120, 120
    readImageM logoUR
    draw 320, 20, 120, 120
    readImageM logoLL
    draw 200, 140, 120, 120
    readImageM logoLR
    draw 320, 140, 120, 120
ENDM

drawnames MACRO
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
    call openFile_Home
    noError1:
    call readData
    noError2:
    call closeFile_Home
    ret
readImageP ENDP

;opens a file given the offset of the file name in dx
; you need first to move the offset of the file name to dx 
; lea dx, filename
openFile_Home PROC
    mov ah, 03Dh
    mov al, 0 ; open attribute: 0 - read-only, 1 - write-only, 2 -read&write
    int 21h
    ret
openFile_Home ENDP

; read the data form the opened file
readData PROC
    mov bx, AX
    mov ah, 03Fh
    mov cx, buffer_size ; number of bytes to read
    mov dx, offset buffer ; were to put read data
    int 21h
    ret
readData ENDP

closeFile_Home PROC
    mov ah, 3Eh         ; DOS function: close file
    INT 21H
    ret
closeFile_Home ENDP

; draws an image using interrupt
drawP PROC
    pusha
    lea si, buffer
    mov dx, Ystart
    mov ah, 0ch
    rowLoop:
    cmp dx, ScreenHeight
    jge  drawPFinish
    mov cx, Xstart
    columnLoop:
    mov al, [si]
    inc si
    cmp al, 250D
    je skip_drawP
    cmp cx, ScreenWidth
    jge skip_drawP
    int 10h
    skip_drawP:
    inc cx
    cmp cx, Xlimit
    jne columnLoop
    inc dx
    cmp dx, Ylimit
    jnz rowLoop
    drawPFinish:
    popa
    ret
drawP ENDP

UltraGraphicsMode MACRO 
  mov AX, 4F02h
  mov BX, 0101h  ; 640x480x256
  int 10h        ; BIOS interrupt for video services
ENDM UltraGraphicsMode

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

putWallPaperP PROC
    pusha
    mov al, bgCR
    mov cx, bgCS
    putWallPaperPColumnLoop:
    mov ah, bgRR
    mov dx, bgRS
    putWallPaperPRowLoop:
    draw cx, dx, bgSize, bgSize
    dec ah
    add dx, bgSize
    cmp ah, 0
    jnz putWallPaperPRowLoop
    dec al
    add cx, bgSize
    cmp al, 0
    jnz putWallPaperPColumnLoop
    popa
    ret
putWallPaperP ENDP
   