include hPart.inc
include circSect.inc
.model small
.stack 64
.data
    ;----------------------------------COLORS------------------------------------------------------;
         TEXT_COLOR equ 0    ; Black text on white background
         SCREEN_ATTR equ 0F0h ; White background (high nibble) and black text (low nibble)
         DASHEDLINEATTR equ 0F0h ; 
         RED equ 04h
         LIGHT_GREEN equ 0Ah
         GREEN equ 02h
         LIGHT_GRAY equ 07h
         GRAY equ 08h
         WHITE equ 0Fh
         BLACK equ 00h
         DASHESCOLOR equ 07h
         ;----------------------------------Positions-------------------------------------------------;      
         START_X dw 70
         START_Y dw 150
         END_X dw ?
         END_Y dw ?
         ;----------------------------------BOUNDARY-------------------------------------------------;      
         MIN_X equ 0
         MAX_X equ 320
         MIN_Y equ 0
         MAX_Y equ 150 
         ;----------------------------------Dimentions-------------------------------------------------;
         LINE_WIDTH equ 20
         LINE_LENGTH equ 60
         BOUNDARY_WIDTH equ 1
         BOUNDARY_LENGTH equ 4
         DASHEDLINE_LENGTH equ 6
         RADIUS dw 20
         p0x dw 0
         p1x dw 0
         p2x dw 0
         p0y dw 0
         p1y dw 0
         p2y dw 0
         direction db 1

.code
main proc far
    mov ax, @data
    mov ds, ax
    mov ah, 0
    mov al, 13h
    int 10h
    ;draw two horizontal roads with different directions
    
    horizontalPart START_X, START_Y, direction

    mov ax, START_X
    add ax, LINE_LENGTH
    mov END_X, ax
    mov ax, START_Y
    add ax, LINE_WIDTH
    add ax, 2*BOUNDARY_WIDTH
    mov END_Y, ax
    ;deletePart START_X, START_Y, END_X, END_Y
    mov ah, 4ch        
    INT 21h         

main endp
end main


