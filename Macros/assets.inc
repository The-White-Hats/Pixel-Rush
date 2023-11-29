showmes macro str
    mov ah,09h
    lea dx,str
    int 21h  
endm showmes  

showMsgClr macro str, clr
    local loop1
    mov cx,0
    lea bx,str
    loop1:
     mov ax,[bx]
     inc bx
     inc cx
     cmp ax,'$'
     jnz loop1
    dec cx
    mov ah,9 ;Display
    mov bh,0 ;Page 0
    mov al,00h ;
    mov cx,80 ;5 times
    mov bl,clr ;Green (A) on white(F) background

    int 10h 
    mov ah,09h
    lea dx,str
    int 21h  
endm showMsgClr 


showchar macro str
    mov ah,02h
    mov dl,str
    int 21h  
endm showchar 

endl macro 
       mov ah,02h
    mov dl,10
    int 21h 
    mov ah,02h
    mov dl,13
    int 21h
endm endl

getstr macro str
    mov ah,0ah
    lea dx,str
    int 21h
endm getstr
shownum macro str
    local l1     
    local l2
     lea bx, str
     mov cx,10
     mov dx,0
     l1:
       mov dx,0
       div cx     
       add dl,30h
       mov [bx],dl
       inc bx 
       cmp ax,0
     jnz l1     
     mov [bx],'$' 
     mov cx, bx
     lea dx,str
     sub cx,dx  
     dec bx
     l2:
         mov ah,2
         mov dl,[bx]
         int 21h
         dec bx
         cmp dh,0
     loop l2      
endm shownum
  
clear macro 
    mov ah, 0       ; AH=0 is the function for setting video mode
    mov al, 3       ; AL=3 sets 80x25 text mode (standard text mode)
    int 10h         ; BIOS interrupt for video services
endm clear 

moveup macro  
    local skip
    mov ah,3h
    mov bh,0h
    int 10h
    mov ah,02h
    inc dl
    int 10h
endm moveup 
moveback macro  
    local skip
    mov ah,3h
    mov bh,0h
    int 10h
    mov ah,02h
    dec dl
    int 10h
endm moveback 

change macro  
   mov ah,9
   mov bh,0
   mov al,00h
   mov cx,1
   mov bl,0fah
   int 10h
   moveup
   mov ah,9
   mov bh,0
   mov al,00h
   mov cx,1
   mov bl,0fah
   int 10h
   moveback
endm change 

reset macro  
   mov ah,9
   mov bh,0
   mov al,44h
   mov cx,0
   mov bl,0F0h
   int 10h
endm reset 

checkFirstChar macro buffer, error
    local valid
    local notValid
    local finish
    local invalid
    invalid:
    mov si, offset buffer+2
    mov cx, 0
    mov cl, buffer+1
    cmp cl, 0
    jz notValid
    inc cx
    valid:
        dec cx
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
        ;move cursor to middle of screen
        mov ah, 02h
        mov dh, 12
        mov dl, 40
        int 10h
        showMsgClr error, 0ch

        mov dh, 1 ; line to be cleared
        clearLine
        mov ah, 02h
        mov dh, 1
        mov dl, 8
        int 10h
        getstr buffer
        jmp invalid
    finish:
        mov ah, 02h
        mov dh, 12
        mov dl, 40
        int 10h
        clearLine
        mov ah, 02h
        mov dh, 3
        mov dl, 4
        int 10h
endm checkFirstChar

checkNameChars macro buffer, error
    local valid
    local notValid
    local finish
    local invalid
    invalid:
    mov si, offset buffer+2
    mov cx, 0
    mov cl, buffer+1
    cmp cl, 0
    je notValid
    inc cx
    valid:
        dec cx
        cmp cx, 0
        je finish
        lodsb
        cmp al, 'A'
        jl notValid
        cmp al, 'Z'
        jle valid
        cmp al, 'a'
        jl notValid
        cmp al, 'z'
        jle valid
    notValid:
        ;move cursor to middle of screen
        mov ah, 02h
        mov dh, 12
        mov dl, 40
        int 10h
        showMsgClr error, 0ch

        mov dh, 1 ; line to be cleared
        clearLine
        mov ah, 02h
        mov dh, 1
        mov dl, 4
        int 10h
        getstr buffer
        jmp invalid
    finish:
        mov ah, 02h
        mov dh, 12
        mov dl, 40
        int 10h
        clearLine
        mov ah, 02h
        mov dh, 3
        mov dl, 4
        int 10h

    
endm checkNameChars

clearLine macro 
    ; local loop1
    mov ah, 02h
    mov dl, 0
    int 10h
    ; mov cx, 80
    ; loop1:
    ;     mov ah, 02h
    ;     mov dl, 20h
    ;     int 21h
    ;     loop loop1
    mov ah,9 ;Display
mov bh,0 ;Page 0
mov al,00h ;Letter D
mov cx,80 ;5 times
mov bl,0fh ;Green (A) on white(F) background
int 10h 
endm clearLine
    

