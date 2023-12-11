;include hPart.inc
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
         START_X dw 150
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
         LINE_LENGTH equ 5
         BOUNDARY_WIDTH equ 1
         BOUNDARY_LENGTH equ 4
         DASHEDLINE_LENGTH equ 6
         cornerFlag db 1
         cornerType db 1 ;1 means up, 0 means down
         horizontalDirection db 0

.code
main proc far
    mov ax, @data
    mov ds, ax
    mov ah, 0
    mov al, 13h
    int 10h

    call horizontalPart
    ; mov START_X, 170
    ; call checkHorizontalTrack

    mov ah, 4ch        
    INT 21h         

main endp

CheckHorizontalTrack PROC
	    mov bx,0
		mov cx, START_X
		mov dx, START_Y
        ;---------------------------check for bounadry conditions---------------------------------;
		    ;--------------------------------put the final position of dx and check with it---------------------------------;
	        cmp horizontalDirection ,0
				jnz down3
				sub cx,LINE_LENGTH ;dx
				jmp skip3
				down3:
				add cx,LINE_LENGTH ;dx
				skip3:

				cmp dx,MAX_Y
				jle check1
				jmp return
            ;----------------------------------------------------------------------------------------------------------------;
			check1: 
				sub dx,LINE_WIDTH
				sub dx,BOUNDARY_WIDTH*2
				cmp dx,MIN_Y
				jge check2
				jmp return

			check2:
				cmp cx,MAX_X ;dx
				jle check3
				jmp return

			check3:
				cmp cx,MIN_X ;dx
				jge checkcolor
				jmp return 

		checkcolor:
		;-------------------------- horizontal line check = ---------------------------------;
        mov bx,0
        mov cx,START_X
		checkHorizontal:
		    mov dx,START_Y


		    cmp bx,LINE_WIDTH
			jz checkVertical_pre
			inc bx ; inc counter untill reach the length of the line

             ;-----------------------------update dx according to horizontalDirection -----------------------------------------;
			cmp horizontalDirection ,0
			jnz down1
			dec cx
			jmp skip1
			down1:
			inc cx
			skip1:
             ;----------------------------------------------------------------------------------------------------------------;
			
			mov ah,0dh
			int 10h
			cmp al,BLACK
		jz checkHorizontalparallel ;if not green return
		ret

		checkHorizontalparallel:

			add dx,LINE_WIDTH  
			add dx,BOUNDARY_WIDTH*2 ;same y-axis but x-axis is increased by the width of the line and the boundary

			mov ah,0dh
			int 10h
			cmp al,BLACK

		jz checkHorizontal  ;if not green return
		ret 
        
		;--------------------------vertical line check | | ---------------------------------;
		checkVertical_pre:
		mov bx,0
        mov dx,START_Y
		checkVertical:

		    mov cx,START_X
		    cmp bx,LINE_LENGTH
			jz break
			inc bx

			dec dx

			mov ah,0dh
			int 10h
			cmp al,BLACK

		jz checkVerticalparallel
		ret

		checkVerticalparallel:

			cmp horizontalDirection ,0
			jnz down2
			sub cx ,LINE_LENGTH
			jmp skip2
			down2:
			add cx ,LINE_LENGTH
			skip2:

			mov ah,0dh
			int 10h
			cmp al,BLACK
		jz checkVertical
         ret 
		 
		 return:
		 ret
         break:
		;horizontalPart START_X, START_Y, horizontalDirection
        ret
checkHorizontalTrack ENDP

horizontalPart PROC
    mov ah, 0ch
    mov si, LINE_WIDTH + 2*BOUNDARY_WIDTH ; Outer loop counter
    mov dx, START_Y
    drawRoad:
            mov cx, START_X
            mov di, LINE_LENGTH ; Inner loop counter
            mov bl, BOUNDARY_LENGTH
            mov bh, 2*DASHEDLINE_LENGTH
            mov al, GRAY 
            cmp si, LINE_WIDTH + BOUNDARY_WIDTH ;check if upper bound
            jle check123
            mov al, RED
        check123:
            cmp si, 2 ;check if lower bound
            jge check25
            mov al, RED 
        check25:
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
            cmp horizontalDirection, 0 ;check if right
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
    mov cx, START_X
    mov dx, START_Y
    mov si, BOUNDARY_WIDTH 
    mov bl, BOUNDARY_LENGTH
    mov al, RED
    boundaryOuter:
        mov dx, START_Y
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
    mov dx, START_Y
    jmp exec

    upOpened:
    mov dx, START_Y
    sub dx, LINE_WIDTH
    sub dx, BOUNDARY_WIDTH

    exec:
    mov si, BOUNDARY_WIDTH

    open1:
    
        mov di, LINE_WIDTH
        mov cx, START_X
        cmp horizontalDirection,1
        jnz addd
        dec cx
        jmp open11
        addd:
        inc cx
        open11:
        int 10h
        cmp horizontalDirection,0
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
	ret
 horizontalPart  ENDP


end main


