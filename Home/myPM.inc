; This file include all the macros that I might need and I know up till now
; It includes:
;   ShowMes str
;   ReadVarLenNum num
;   PrintVarLenNum num
;   MoveCursor x, y
;   ShowChar char
;   Endl
;   GetStr str
;   TextMode
;   GraphicsMode
;   UltraGraphicsMode
;   RemoveBlinking
;   PaintScreen Color, LeftTopCorner, RightBottomCorner ; it can be used to clear a certain part of the screan
;   MoveCursorUp times
;   MoveCursorRight times
;   MoveCursorDown times
;   MoveCursorLeft times

ShowMes MACRO str
  mov ah,09h
  lea dx,str
  int 21h  
ENDM 

ReadVarLenNum MACRO num
  ; the var must be a word to receive up to 65535
  MOV num, 0
  MOV BX, 10
  .INPUT:
  MOV AH, 1
  INT 21H
  CMP AL, 13
  JE .NEXT
  SUB AL, 30H
  MOV AH, 0
  Mov CX, AX
  MOV AX, num
  MUL BX
  ADD AX, CX
  MOv num,AX
  JMP .INPUT
  .NEXT:
ENDM

; This proc print a number with variable lenght up to 65535
PrintVarLenNum MACRO num
  MOv AX, num
  MOv DX, 0
  MOV BX, 10
  MOv CX, 0
  .L1:
  DIV BX
  PUSH DX
  MOV DX, 0
  INC CX
  CMP AX, 0
  JNE .L1
  MOV AH, 2
  .L2:
  POP DX
  ADD DX, 48
  INT 21H
  LOOP .L2
ENDM

MoveCursor MACRO x, y
  mov dl, x
  mov dh, y
  mov ah, 2H
  int 10h  
ENDM

ShowChar MACRO char
  mov ah,02h
  mov dl,char
  int 21h  
ENDM 

Endl MACRO 
  mov ah,02h
  mov dl,10
  int 21h 
  mov ah,02h
  mov dl,13
  int 21h
ENDM

GetStr MACRO str
  mov ah,0ah
  lea dx,str
  int 21h
ENDM

TextMode MACRO 
  mov ah, 0       ; AH=0 is the function for setting video mode
  mov al, 3       ; AL=3 sets 80x25 text mode (standard text mode)
  int 10h         ; BIOS interrupt for video services
ENDM

GraphicsMode MACRO 
  mov ah, 0       ; AH=0 is the function for setting video mode
  mov al, 13H     ; AL=13H sets 320x200 video graphics mode
  int 10h         ; BIOS interrupt for video services
ENDM

UltraGraphicsMode MACRO 
  mov AX, 4F02h
  mov BX, 0101h  ; 640x400x256
  int 10h        ; BIOS interrupt for video services
ENDM

RemoveBlinking MACRO
  mov AX , 1003h
  mov BL ,00h  ; 00h background intensity enabled , 01h blink enabled
  mov BH , 00h ; to avoid problems on some adapters
  int 10h  
ENDM

; this could be used to clear a certain part of the screen from text without scrolling
PaintScreen MACRO Color, LeftTopCorner, RightBottomCorner
  mov ax ,0600h
  mov bh, Color
  mov cx, LeftTopCorner
  mov dx , RightBottomCorner
  int 10h
ENDM

MoveCursorUp MACRO times
  mov cx, times
  .repeat:
  mov ah,3h
  mov bh,0h
  int 10h
  mov ah,02h
  dec dh
  int 10h
  loop .repeat
ENDM

MoveCursorRight MACRO times
  mov cx, times  
  .repeat:
  mov ah,3h
  mov bh,0h
  int 10h
  mov ah,02h
  inc dl
  int 10h
  loop .repeat
ENDM 

MoveCursorDown MACRO times
  mov cx, times
  .repeat:
  mov ah,3h
  mov bh,0h
  int 10h
  mov ah,02h
  inc dh
  int 10h
  loop .repeat
ENDM

MoveCursorLeft MACRO times
  local repeat
  mov cx, times  
  repeat:
  mov ah,3h
  mov bh,0h
  int 10h
  mov ah,02h
  dec dl
  int 10h
  loop repeat
ENDM 