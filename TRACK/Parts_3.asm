
; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt
         include draw.inc
		.MODEL SMALL
		.STACK 64
		.DATA   
		;----------------------------------COLORS------------------------------------------------------;
		 TEXT_COLOR equ 0    ; Black text on white background
         SCREEN_ATTR equ 0FCh ; White background (high nibble) and black text (low nibble)
         DASHEDLINEATTR equ 0F0h ; 
		 WHITE equ 0Fh
         RED equ 04h
		 LIGHT_GREEN equ 0Ah
		 GREEN equ 02h
		 LIGHT_GRAY equ 07h
		 GRAY equ 08h
         ;----------------------------------Positions-------------------------------------------------;      
         START_X dw 0
		 START_Y dw 30
		 START_X2 dw 30
		 START_Y2 dw 75
		 ;----------------------------------BOUNDARY-------------------------------------------------;      
		 MIN_X equ 0
		 MAX_X equ 320
		 MIN_Y equ 0
		 MAX_Y equ 150 
         ;----------------------------------Dimentions-------------------------------------------------;
		 LINE_WIDTH dw 20
		 LINE_LENGTH dw 60
		 BOUNDARY_WIDTH equ 1
		 BOUNDARY_LENGTH equ 4
		 DASHEDLINE_LENGTH equ 6
	     ;----------------------------------Variables-------------------------------------------------;
		 posx dw 0
		 posy dw 0
		 toggleboundarycolor db 0
		 toggletrackmiddle db 0
		 boundaryflag db  0
		 middletrackflag db 0
         verticaldirection db 0 ; 0 up 1 down
		 Vertical_Track_Delete_Flag db 0

     	.CODE


MAIN 	PROC FAR
     	MOV AX , @DATA
      	MOV DS , AX  

      	clear
        
		; Remove Blinking from the screen and allowing to use 16 colors as background
        mov AX , 1003h
		mov BL ,00h  ; 00h background intensity enabled , 01h blink enabled
		mov BH , 00h ; to avoid problems on some adapters
        int 10h
		

		;---------------------------------------Screen Coloring------------------------------------------------
		; mov ax ,0600h
		; mov bh,GREEN
		; mov cx,0h
		; mov dx , 184fh
     	; int 10h
		   xor cx,cx
			xor dx,dx
		    mov cl,0 ;x axis
			mov dl,0 ;y axis
			mov al,GREEN
			mov ah,0ch
		back:	int 10h
		  inc cx
		  cmp cx,320
		  jnz back
		  mov cx,0
		  inc dl
		  cmp dl,200
		  jnz back
        ;------------------------------------Vertical Line-------------------------------------------;  
			mov cx,START_X
			mov dx,START_Y
			mov posx,cx
			mov posy,dx
		   ; call  GenerateVerticalTrack 
			; mov cx,START_X2
			; mov dx,START_Y2
			; mov START_X,cx
			; mov START_Y,dx
			; mov posx,cx
			; mov posy,dx
		    call  CheckVerticalTrack 
			
        MOV AH, 4CH         ; Function to exit program
        INT 21H             ; Call DOS interrupt to exit

MAIN ENDP


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
        mov bx,0
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
			cmp al,GREEN
		jz checkverticalparallel ;if not green return
		ret

		checkverticalparallel:

			add cx,LINE_WIDTH  
			add cx,BOUNDARY_WIDTH*2 ;same y-axis but x-axis is increased by the width of the line and the boundary

			mov ah,0dh
			int 10h
			cmp al,GREEN

		jz checkvertical  ;if not green return
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
			int 10h
			cmp al,GREEN

		jz checkhorizontalparallel
		ret

		checkhorizontalparallel:

			cmp verticaldirection ,0
			jnz down2
			sub dx ,LINE_LENGTH
			jmp skip2
			down2:
			add dx ,LINE_LENGTH
			skip2:

			mov ah,0dh
			int 10h
			cmp al,GREEN
		jz checkhorizontal
         ret 
		 
		 return:
		 ret
         break:
		call GenerateVerticalTrack
        ret
CheckVerticalTrack ENDP

GenerateVerticalTrack PROC 
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
				mov al,LIGHT_GRAY
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

END MAIN