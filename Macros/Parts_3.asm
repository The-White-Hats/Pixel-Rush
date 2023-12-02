
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
         START_X dw 10
		 START_Y dw 150
		 ;----------------------------------BOUNDARY-------------------------------------------------;      
		 MIN_X equ 0
		 MAX_X equ 320
		 MIN_Y equ 0
		 MAX_Y equ 150 
         ;----------------------------------Dimentions-------------------------------------------------;
		 LINE_WIDTH dw 20
		 LINE_LENGTH dw 80
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
		    call GenerateVerticalTrack
			; l1:
			; jmp l1
			; xor Vertical_Track_Delete_Flag,1
			; mov cx,START_X
			; mov dx,START_Y
			; mov posx,cx
			; mov posy,dx
			;call GenerateVerticalTrack


        MOV AH, 4CH         ; Function to exit program
        INT 21H             ; Call DOS interrupt to exit

MAIN ENDP

GenerateVerticalTrack PROC 

		movehorizontal:      
			mov boundaryflag ,  0
			drawvertical:

			    mov cx,posx
				mov dx,posy

                cmp Vertical_Track_Delete_Flag,1
				jz mid31
              
				cmp posx,MAX_X
				jnz check1
				jmp delete
				check1:
				cmp posx,MIN_X
				jnz check2
				jmp delete
				check2:
				cmp posy,MIN_Y
				jnz checkcolor
				jmp delete
				checkcolor:
				mov ah,0dh
				int 10h
				cmp al,GREEN
				jz draw
                delete:
				mov bx,posx
                sub bx,START_X
				mov LINE_WIDTH,bx
				mov bx,posy
				sub bx,START_Y

				
				mov LINE_LENGTH,bx
                mov bx,START_X
				mov posx,bx
				mov bx,START_Y
				mov posy,bx

				mov Vertical_Track_Delete_Flag,1
                call GenerateVerticalTrack
				mov Vertical_Track_Delete_Flag,0
				RET
				draw:
				;-----------------------checkfor delete flag if true just draw with green---------------------;
				
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
				mid31:
				jmp mid3
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

                mid3:
				jmp  deleteverticaltrack
              

				roadcoloring:
                mov bx,START_X
				add bx,BOUNDARY_WIDTH
				add bx,LINE_WIDTH
                cmp posx,bx
                jge boundarycoloring 
                
				 push ax
                mov ax,LINE_WIDTH
				mov bx,2
				div bl
				mov bx,0
				mov bl,al
				pop ax
		
				add bx,START_X
                add bx,BOUNDARY_WIDTH

                cmp posx,bx; if it is not the middle track then jump to next
				jnz normaltrackcoloring
		

                cmp middletrackflag,0 ;Checl if i will color white incase of middle track
				jnz normaltrackcoloring ; else normal coloring
				mov al,WHITE
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

				cmp posx,bx; if it is not the middle track then jump to next
				jnz next

				inc toggletrackmiddle
				cmp toggletrackmiddle,DASHEDLINE_LENGTH
				jnz next
				mov toggletrackmiddle,0
				xor middletrackflag,1
                jmp next

                mid2:
				jmp movehorizontal
                
				deleteverticaltrack:
				mov al,GREEN

				next:

				mov ah,0ch
				int 10h

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

            jnz mid1

            mov bx,START_Y
            mov posy,bx

            inc posx

			
           mov bx,LINE_WIDTH
		   add bx,BOUNDARY_WIDTH*2
		   add bx,START_X

			cmp posx,bx

		 jnz mid2  ;jmp to again listen again for the next instruction

		RET
	GenerateVerticalTrack endp

END MAIN