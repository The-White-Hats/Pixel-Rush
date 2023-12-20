showmes macro str
    mov ah,09h
    lea dx,str
    int 21h  
endm showmes    

showchar macro str
    mov ah,02h
    mov dl,str
    int 21h  
endm showchar 

endlst macro  start     ; give it the start of the vertical cordinate (X-axis)
    ; newline
    mov ah,02h
    mov dl,10
    int 21h
      
    mov ah,3
  	mov bh,0h
  	int 10h
  	   
	mov ah,2
  	mov dl,start
  	int 10h 
  	
endm endlst
  
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

clearline macro
    mov ah,9
    mov bh,0
    mov al, 0h
    mov cx,79
    mov bl,SCREEN_ATTR
    int 10h
endm cearline

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

