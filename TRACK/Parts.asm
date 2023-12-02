include hPart.inc
include circSect.inc
include draw.inc
.model small
.stack 64
.data
    ;*----------------------------------COLORS------------------------------------------------------;
         TEXT_COLOR equ 0    ; DefaultBackground text on white background
         SCREEN_ATTR equ 0F0h ; White background (high nibble) and DefaultBackground text (low nibble)
         DASHEDLINEATTR equ 0F0h ; 
         RED equ 04h
         LIGHT_GREEN equ 0Ah
         GREEN equ 02h
         LIGHT_GRAY equ 07h
         GRAY equ 08h
         WHITE equ 0Fh
         BLACK equ 00h
         DefaultBackground equ BLACK
         DASHESCOLOR equ LIGHT_GRAY
         ;*----------------------------------Positions-------------------------------------------------;      
         START_X dw 150
         START_Y dw 150
         END_X dw ?
         END_Y dw ?
         ;*----------------------------------BOUNDARY-------------------------------------------------;      
         MIN_X equ 0
         MAX_X equ 320
         MIN_Y equ 0
         MAX_Y equ 150 
         ;*----------------------------------Dimensions-------------------------------------------------;
         LINE_WIDTH equ 20
         LINE_LENGTH equ 60
         BOUNDARY_WIDTH equ 1
         BOUNDARY_LENGTH equ 4
         DASHEDLINE_LENGTH equ 6
          ;*----------------------------------Variables-------------------------------------------------;
		 posx dw 0
		 posy dw 0
		 toggleboundarycolor db 0
		 toggletrackmiddle db 0
		 boundaryflag db  0
		 middletrackflag db 0
         verticaldirection db 0 ;! 0 up 1 down

         cornerFlag db 1  ;! 0 means no corner, 1 means corner
         cornerType db 1 ;! 1 means up, 0 means down
         horizontalDirection db 1 ;! 1 left 0 right

.code
main proc far
    mov ax, @data
    mov ds, ax
  
    clear
    
    ; Remove Blinking from the screen and allowing to use 16 colors as background
        mov AX , 1003h
		mov BL ,00h  ; 00h background intensity enabled , 01h blink enabled
		mov BH , 00h ; to avoid problems on some adapters
        int 10h
		

	;---------------------------------------Screen Coloring------------------------------------------------;
		;mov ax ,0600h
;		mov bh,DefaultBackground
;		mov cx,0h
;		mov dx , 184fh
;     	int 10h
    ;-----------------------------------------------------------------------------------------------------;    
        mov START_X,210
    call  CheckHorizontalTrack
    
    mov horizontalDirection,1
    mov cornerFlag,0
    mov START_X,149
  
    ;call  CheckHorizontalTrack
    mov START_X,189
    mov START_Y,128
    call  CheckVerticalTrack 

    mov ah, 4ch        
    INT 21h         

main endp

CheckHorizontalTrack PROC 
	    mov bx,0
		mov cx, START_X
		mov dx, START_Y
        ;---------------------------check for bounadry conditions---------------------------------;
		    ;--------------------------------put the final position of dx and check with it---------------------------------;
	        cmp horizontalDirection ,1
				jnz down4
				sub cx,LINE_LENGTH ;dx
				jmp skip4
				down4:
				add cx,LINE_LENGTH ;dx
				skip4:

				cmp dx,MAX_Y
				jle Hcheck1
				jmp return1
            ;----------------------------------------------------------------------------------------------------------------;
			Hcheck1: 
				sub dx,LINE_WIDTH
				sub dx,BOUNDARY_WIDTH*2
				cmp dx,MIN_Y
				jge Hcheck2
				jmp return1

			Hcheck2:
				cmp cx,MAX_X ;dx
				jle Hcheck3
				jmp return1

			Hcheck3:
				cmp cx,MIN_X ;dx
				jge checkcolor1
				jmp return1

		checkcolor1:
		;-------------------------- horizontal line check = ---------------------------------;
        mov bx,0
        mov cx,START_X
		checkHorizontal1:
		    mov dx,START_Y


		    cmp bx,LINE_LENGTH
			jz checkVertical_pre1
			inc bx ; inc counter untill reach the length of the line

             ;-----------------------------update dx according to horizontalDirection -----------------------------------------;
			cmp horizontalDirection ,1
			jnz down5
			dec cx
			jmp skip5
			down5:
			inc cx
			skip5:
             ;----------------------------------------------------------------------------------------------------------------;
			
			mov ah,0dh
			int 10h
			cmp al,DefaultBackground  
            
		jz checkHorizontalparallel1 ;if not DefaultBackground return
		ret

		checkHorizontalparallel1:

			sub dx,LINE_WIDTH  
			sub dx,BOUNDARY_WIDTH*2 ;same y-axis but x-axis is increased by the width of the line and the boundary
            inc dx

			mov ah,0dh
			int 10h  
			cmp al,DefaultBackground
          
		jz checkHorizontal1  ;if not DefaultBackground return
		ret 
        
		;--------------------------vertical line check | | ---------------------------------;
		checkVertical_pre1:
		mov bx,0
        mov dx,START_Y
		checkVertical1:
            mov cx,LINE_WIDTH
            add cx,BOUNDARY_WIDTH*2
		    cmp bx,cx
			jz break1
			inc bx

            mov cx,START_X

			dec dx

			mov ah,0dh
			int 10h 
			cmp al,DefaultBackground
           
		jz checkVerticalparallel1
		ret

		checkVerticalparallel1:

			cmp horizontalDirection ,1
			jnz down6
			sub cx ,LINE_LENGTH
			jmp skip6
			down6:
			add cx ,LINE_LENGTH
			skip6:

			mov ah,0dh
			int 10h
			cmp al,DefaultBackground
		jz checkVertical1
         ret 
		 
		 return1:
		 ret
         break1:
		horizontalPart START_X, START_Y, horizontalDirection
        ret 
CheckHorizontalTrack ENDP

CheckVerticalTrack PROC
	    mov bx,0
		mov cx,START_X
		mov dx,START_Y
        ;---------------------------check for bounadry conditions---------------------------------;
		    ;--------------------------------put the final position of dx and check with it---------------------------------;
	        cmp verticaldirection ,0
				jnz down3
				sub dx,LINE_LENGTH
				jmp skip3
				down3:
				add dx,LINE_LENGTH
				skip3:

				cmp cx,MIN_X
				jge check1
				jmp return
            ;----------------------------------------------------------------------------------------------------------------;
			check1: 
				add cx,LINE_WIDTH
				add cx,BOUNDARY_WIDTH*2
				cmp cx,MAX_X
				jle check2
				jmp return

			check2:
				cmp dx,MIN_Y
				jge check3
				jmp return

			check3:
				cmp dx,MAX_Y
				jle checkcolor
				jmp return 

		checkcolor:
		;-------------------------- vertical line check | | ---------------------------------;
        mov bx,1
		mov dx,START_Y
		checkvertical:
		    mov cx,START_X

		    cmp bx,LINE_LENGTH
			jz checkhorizontal_pre
			inc bx ; inc counter untill reach the length of the line

             ;-----------------------------update dx according to verticaldirection -----------------------------------------;
			cmp verticaldirection ,0
			jnz down1
			dec dx 
			jmp skip1
			down1:
			inc dx
			skip1:
             ;----------------------------------------------------------------------------------------------------------------;
			
			mov ah,0dh
			int 10h
			cmp al,DefaultBackground
		jz checkverticalparallel ;if not DefaultBackground return
		ret

		checkverticalparallel:

			add cx,LINE_WIDTH  
			add cx,BOUNDARY_WIDTH*2 ;same y-axis but x-axis is increased by the width of the line and the boundary
            dec cx
			mov ah,0dh
			int 10h
			cmp al,DefaultBackground

		jz checkvertical  ;if not DefaultBackground return
		ret 
        
		;--------------------------horizontal line check = ---------------------------------;
		checkhorizontal_pre:
		mov bx,0
		mov cx,START_X
		checkhorizontal:

		    mov dx,START_Y
		    cmp bx,LINE_WIDTH
			jz break
			inc bx

			inc cx

			mov ah,0dh
			int 10
			cmp al,DefaultBackground

		jz checkhorizontalparallel
		ret

		checkhorizontalparallel:

			cmp verticaldirection ,0
			jnz down2
			sub dx ,LINE_LENGTH
            inc dx
			jmp skip2
			down2:
			add dx ,LINE_LENGTH
            dec dx
			skip2:

			mov ah,0dh
			int 10h
			cmp al,DefaultBackground
		jz checkhorizontal
         ret 
		 
		 return:
		 ret
         break:
		call GenerateVerticalTrack
        ret
CheckVerticalTrack ENDP

GenerateVerticalTrack PROC 

        mov cx,START_X
        mov dx,START_Y
        mov posx,cx
        mov posy,dx

		movehorizontal:      
			mov boundaryflag ,  0
			drawvertical:
			    mov cx,posx
				mov dx,posy
				
                ;------- if it is the first two lines then draw it with the red and white (boundary) else gray and lightgray with toggle-----;
				mov bx,START_X
				add bx,BOUNDARY_WIDTH
				cmp posx,bx
				jge roadcoloring
				
                ;-----------toggle the color of the boundary-----------------;
				boundarycoloring:

				cmp boundaryflag,0
				jnz whitebounadrycolor 
				mov al,RED
				jmp boundarytogglecheck
				whitebounadrycolor:
				mov al,WHITE
                boundarytogglecheck:

                ;---------Check if i finfished the boundary length then toggle the flag-----------------;
				inc toggleboundarycolor 
				cmp toggleboundarycolor,BOUNDARY_LENGTH

				jnz mid4 
				mov toggleboundarycolor,0
				xor boundaryflag,1
                jmp next

				roadcoloring:
                mov bx,START_X
				add bx,BOUNDARY_WIDTH
				add bx,LINE_WIDTH
                cmp posx,bx
                jge boundarycoloring 

                ;--------------- if it is not the middle track then jump to next ,equation  bx = START_X + BOUNDARY_WIDTH + LINE_WIDTH/2 ------------------------------;

				push ax
                mov ax,LINE_WIDTH
				mov bx,2
				div bl
				mov bx,0
				mov bl,al
				pop ax
		
				add bx,START_X
                add bx,BOUNDARY_WIDTH

                cmp posx,bx
                
				;-------------------------------------------------------------------------------------------------------------------------------------------------------;

				jnz normaltrackcoloring
                cmp middletrackflag,0   ;Checl if i will color white incase of middle track
				jnz normaltrackcoloring ; else normal coloring
				mov al,DASHESCOLOR
				jmp middeltrackcheck

				mid4:
				  jmp next
               
                mid1:  ;for Far jump resoloution
				jmp drawvertical
				

				normaltrackcoloring:
                mov al,GRAY

                middeltrackcheck:; if it is the middle track then check if i finished the dashed line length then toggle the flag
                push ax
                mov ax,LINE_WIDTH
				mov bx,2
				div bl
				mov bx,0
				mov bl,al
				pop ax
		
				add bx,START_X
                add bx,BOUNDARY_WIDTH
               
				cmp posx,bx; if it is not the middle track then jump to next ,bx = START_X + BOUNDARY_WIDTH + LINE_WIDTH/2
				jnz next

				inc toggletrackmiddle
				cmp toggletrackmiddle,DASHEDLINE_LENGTH
				jnz next
				mov toggletrackmiddle,0
				xor middletrackflag,1
                jmp next

                mid2:
				jmp movehorizontal

				next:
				mov ah,0ch
				int 10h
                
				;---------------------------------------change posy according to vertical direction if up dec and cmp with start - length if down inc and cmp with start+length ----------------------------------------------;
				cmp verticaldirection ,0 
				jnz down  
				dec posy
				mov bx,START_Y
				sub bx,LINE_LENGTH
				cmp posy,bx
				jmp skip
                down:
			    inc posy
				mov bx,START_Y
				add bx,LINE_LENGTH
				cmp posy,bx
				skip:
                ;-------------------------------------------------------------------------------------------------------------------------------------------------------;

            jnz mid1 ;jmp to mid1 then to drawvertical if less  than the length

            mov bx,START_Y
            mov posy,bx

            inc posx

			mov bx,LINE_WIDTH 
			add bx,BOUNDARY_WIDTH*2
			add bx,START_X

			cmp posx,bx ;bx = LINE_WIDTH  + START_X +BOUNDARY_WIDTH*2 ->total width of the line

		 jnz mid2  ;jmp to mid2 then to movehorizontal

		RET
	GenerateVerticalTrack endp


end main