;include hPart.inc
;include circSect.inc
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
         dt ?
		 START_X dw 150
         START_Y dw 150
		 dt ?
         END_X dw ?
         END_Y dw ?
         ;*----------------------------------BOUNDARY-------------------------------------------------;      
         MIN_X equ 0
         MAX_X equ 320
         MIN_Y equ 0
         MAX_Y equ 150 
         ;*----------------------------------Dimensions-------------------------------------------------;
         LINE_WIDTH equ 20
         LINE_LENGTH equ 20
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
        ;*----------------------------------Track Directions Generation Variables-------------------------------------------------;
        
        MAX_PARTS equ 5

		WRONGTHRESHOLD equ 20

		prev_start_x dw ?
		prev_start_y dw ?
        
        x_max_new dw ?
        x_min_new dw ?

        x_max_old dw ?
        x_min_old dw ?

        y_max_new dw ?
        y_min_new dw ?

        y_max_old dw ?
        y_min_old dw ?

		dt ?

        Directions dw MAX_PARTS+1 dup(0)
        
		dt ?

        ClosedArea dw (MAX_PARTS+1)*4 dup(0) ;! Xmin, Xmax, Ymin, Ymax
        dt ?
		PrevStart  dw (MAX_PARTS+1)*2 dup(0) ;! START_X,START_Y

        SingleCheckFlag db 1 ;? check for single part in track

		TrackCheckFlag db 1 ;? check for all track parts with the new part

		TotalParts dw 0 ;? check for all track parts so far

		WrongCounter dw 0 ;? count number of consecutive wrong parts

		startoffsetdirection dw 0 
		startoffsetclosedarea dw 0 
		startoffsetprevstart dw 0 

        ;*----------------------------------Track Generation Variables------------------------------------------------
        CurrentCase dw 0 
        
        ;*----------------------------------Random Proc Variables------------------------------------------------
        seed DW 1234H       ; Initial seed value
		multiplier Dw 1d51H
		increment Dw 8c45H
		modulus DW 0F4bFh 
		random_part db 0 ; 4 bytes of memory to store the random seed

		str db ?
		str2 db 'yes$'
		
		;------------------------------------------- CASES -------------------------------------------; 
		lastRandom db 0
        currentRandom db 0
		validationFlag db 0

		casse0 db 0,5,7
		casse1 db 8,10,1
		casse2 db 2,6,11
		casse3 db 3,4,9
		casse4 db 1,8,10
		casse5 db 2,6,11
		casse6 db 1,9,11
		casse7 db 3,4,9
		casse8 db 2,6,11
		casse9 db 0,5,7
		casse10 db 3,4,9
		casse11 db 0,5,7

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

    ; mov START_X,210
    ; call  CheckHorizontalTrack
    
    ; mov horizontalDirection,1
    ; mov cornerFlag,0
    ; mov START_X,149
  
    ; ;call  CheckHorizontalTrack
    ; mov START_X,189
    ; mov START_Y,128
    ; call  CheckVerticalTrack 

	call GenerateTrackDirections

	clear
    
	lea si,Directions
	mov cx,MAX_PARTS
	l1321:
		mov ax,[si]	
		push cx
		shownum str
		endl
		pop  cx
		add si,2
       	; jmp ja55sl
        ;      GenerateTrackDir_mid688:
		; 	 jmp  l1321
		; ja55sl:
		; mov ax,[si]	
		; push cx
		; shownum str
		; endl
		; pop  cx
		;add si,2

	loop l1321

    call Draw

    mov ah, 4ch        
    INT 21h         

main endp


SelectType PROC
	 cmp CurrentCase , 0 
		jnz casee1
		;*-------------------------------Case0--------------------------------;
            mov verticaldirection,0
			call  GenerateVerticalTrack
			jmp Done
        ;*--------------------------------------------------------------------;
		casee1:
		cmp CurrentCase , 1
		jnz casee2
		;*-------------------------------Case1--------------------------------;
            mov verticaldirection,1
			call  GenerateVerticalTrack 
			jmp Done
        ;*--------------------------------------------------------------------;
        casee2:
		cmp CurrentCase , 2
		jnz casee3
		;*-------------------------------Case2--------------------------------;
            mov horizontalDirection,0
			mov cornerFlag,0
			call  GenerateHorizontalTrack 
			jmp Done
        ;*--------------------------------------------------------------------;
		casee3:
		cmp CurrentCase , 3
		jnz casee4
		;*-------------------------------Case3--------------------------------;
            mov horizontalDirection,1
			mov cornerFlag,0
			call  GenerateHorizontalTrack 
			jmp Done
        ;*--------------------------------------------------------------------;
		casee4:
		cmp CurrentCase , 4
		jz exe4
		cmp CurrentCase , 5
		jnz casee6
		;*-------------------------------Case4/5--------------------------------;
         exe4:	
			mov horizontalDirection,0
			mov cornerFlag,1
			mov cornerType,0
			call  GenerateHorizontalTrack 
			jmp Done
		;*--------------------------------------------------------------------;
		casee6:
		cmp CurrentCase , 6
		jz exe6
		cmp CurrentCase , 7
		jnz casee8
		;*-------------------------------Case6/7--------------------------------;
		 exe6:
			mov horizontalDirection,1
			mov cornerFlag,1
			mov cornerType,0
			call  GenerateHorizontalTrack 
			jmp Done	
		;*--------------------------------------------------------------------;
		casee8:
		cmp CurrentCase , 8
		jz exe8
		cmp CurrentCase , 9
		jnz casee10
		;*-------------------------------Case8/9--------------------------------;
         exe8:	
			mov horizontalDirection,0
			mov cornerFlag,1
			mov cornerType,1
			call  GenerateHorizontalTrack 
			jmp Done
		;*--------------------------------------------------------------------;
		;*-------------------------------Case10/11--------------------------------;
        casee10:
		    mov horizontalDirection,1
			mov cornerFlag,1
			mov cornerType,1
			call  GenerateHorizontalTrack 
	    Done:
		ret
SelectType ENDP

Draw PROC
	lea si,Directions
    lea bx,PrevStart
	
	mov ax,[bx]
	mov START_X,ax
	mov ax,[bx+2]
	mov START_Y,ax
    add bx,4

    mov cx,MAX_PARTS
	iterate:

	   mov ax,[si]
	   mov CurrentCase ,ax

       push bx
	   push cx
	   push si
	   call SelectType
	   pop si
	   pop cx
	   pop bx

	   mov ax,[bx]
	   mov START_X,ax
	   mov ax,[bx+2]
	   mov START_Y,ax
	   add bx,4

       add si,2 
       mov dx,[si]

	   cmp dx,4d
	   jnz nextCase
	   		inc START_X
			jmp ClearToGo
	   nextCase:
	   cmp dx,6d
	   jnz nextCase2
	   		dec START_X
			jmp ClearToGo
	   nextCase2:
       cmp dx,7d
	   jnz nextCase3
			mov ax,START_X
			add ax,LINE_WIDTH
			add ax,BOUNDARY_WIDTH*2
			dec ax
			mov START_X,ax
			jmp ClearToGo
			iterate_mid:
			jmp iterate
       nextCase3:
	   cmp dx,8d
	   jnz nextCase4
			mov ax,START_Y
			add ax,LINE_WIDTH
			add ax,BOUNDARY_WIDTH*2
			dec ax
			mov START_Y,ax
			jmp ClearToGo
	   nextCase4:
	   cmp dx,9d
	   jnz nextCase5
			inc START_X
			jmp ClearToGo
	   nextCase5:
        cmp dx,10d
	   jnz nextCase6
	        mov ax,0
			
	        mov ax,START_X
			add ax,LINE_WIDTH
			add ax,BOUNDARY_WIDTH*2
			dec ax
			mov START_X,ax
            
			mov ax,START_Y
			add ax,LINE_WIDTH
			add ax,BOUNDARY_WIDTH*2
			dec ax

			mov START_Y,ax	
			jmp ClearToGo
		nextCase6:	
        cmp dx,11d
	    jnz ClearToGo
		  dec START_X

        ClearToGo: 
		dec cx
		cmp cx,0 
		jg iterate_mid 

	ret
Draw ENDP

GenerateTrackDirections PROC 
	mov cx,0
	lea si,Directions
	lea di,ClosedArea
    lea bx,PrevStart

	lea ax,Directions
    mov startoffsetdirection ,ax

	lea ax,ClosedArea
	mov	startoffsetclosedarea ,ax

	lea ax,PrevStart
	mov	startoffsetprevstart ,ax

	mov ax,START_X
	mov [bx],ax
	mov ax,START_Y
	mov [bx+2],ax
	add bx,4

	GenerateTrackDir_loop:
        
		mov ax,START_X
        mov prev_start_x,ax
		mov ax,START_Y
		mov prev_start_y,ax

        push bx
    	call rndm
		pop bx
        
		cmp si,startoffsetdirection
		jz movelastrandom0
		mov ax,[si-2]
		mov lastRandom,al
		jmp skipmove

		movelastrandom0:
		mov lastRandom,0

		skipmove:
		mov ax,0
        mov al,random_part
		mov currentRandom,al

        push bx
        call checkCases
        pop bx

		cmp validationFlag,1
		jz friends
        jmp GenerateTrackDir_loop
		friends:

  
        cmp random_part , 0 
		jnz case1
		;*-------------------------------Case0--------------------------------;
			mov ax,START_X
			mov x_min_new,ax

			add ax,LINE_WIDTH
			add ax,BOUNDARY_WIDTH*2 
			dec ax
			mov x_max_new,ax

			mov ax,START_Y
			mov y_max_new,ax

			sub ax,LINE_LENGTH

			;! updating
			mov START_Y,ax

			inc ax
			mov y_min_new,ax

            
			jmp GenerateTrackDir_FlagCheck
        ;*--------------------------------------------------------------------;
		case1:
		cmp random_part , 1
		jnz case2
		;*-------------------------------Case1--------------------------------;
         	mov ax,START_X
			mov x_min_new,ax

			add ax,LINE_WIDTH
			add ax,BOUNDARY_WIDTH*2 
			dec ax
			mov x_max_new,ax

			mov ax,START_Y
			mov y_min_new,ax

			add ax,LINE_LENGTH

			;! updating
			mov START_Y,ax

            dec ax
			mov y_max_new,ax


			jmp GenerateTrackDir_FlagCheck    
        ;*--------------------------------------------------------------------;
        case2:
		cmp random_part , 2
		jnz case3
		;*-------------------------------Case2--------------------------------;
         	mov ax,START_X
			mov x_min_new,ax

			add ax,LINE_LENGTH

			;! updating
			mov START_X,ax

			dec ax
			mov x_max_new,ax

			mov ax,START_Y
			mov y_max_new,ax

			sub ax,LINE_WIDTH
			sub ax,BOUNDARY_WIDTH*2
			inc ax
			mov y_min_new,ax

			jmp GenerateTrackDir_FlagCheck
        ;*--------------------------------------------------------------------;
		case3:
		cmp random_part , 3
		jnz case4
		;*-------------------------------Case3--------------------------------;
            mov ax,START_X
			mov x_max_new,ax

			sub ax,LINE_LENGTH

	        ;! updating
			mov START_X,ax

            inc ax
			mov x_min_new,ax

			mov ax,START_Y
			mov y_max_new,ax

			sub ax,LINE_WIDTH
			sub ax,BOUNDARY_WIDTH*2
			inc ax
			mov y_min_new,ax

			jmp GenerateTrackDir_FlagCheck
        ;*--------------------------------------------------------------------;
		case4:
		cmp random_part , 4
		jnz case5
		;*-------------------------------Case4--------------------------------;
         	mov ax,[di-2]
			mov y_max_new,ax

			mov ax,[di-4]
			mov y_min_new,ax

			mov ax,[di-6]
			mov x_max_new,ax

			mov ax,[di-8]
			mov x_min_new,ax

			;! updating
			inc START_X
			inc START_Y

			jmp SkipValidation
		;*--------------------------------------------------------------------;
		case5:
		cmp random_part , 5
		jnz case6
		;*-------------------------------Case5--------------------------------;
         	mov ax,START_X
			mov x_min_new,ax

			add ax,LINE_LENGTH

			;! updating
			mov START_X,ax

			dec ax
			mov x_max_new,ax

			mov ax,START_Y
			mov y_max_new,ax

			sub ax,LINE_WIDTH
			sub ax,BOUNDARY_WIDTH*2
			inc ax
			mov y_min_new,ax

			jmp GenerateTrackDir_FlagCheck
		;*--------------------------------------------------------------------;
		case6:
		cmp random_part , 6
		jnz case7
		;*-------------------------------Case6--------------------------------;
			mov ax,[di-2]
			mov y_max_new,ax

			mov ax,[di-4]
			mov y_min_new,ax

			mov ax,[di-6]
			mov x_max_new,ax

			mov ax,[di-8]
			mov x_min_new,ax
			
			;! updating
			mov ax,START_X 
			sub ax,LINE_WIDTH
			sub ax,BOUNDARY_WIDTH*2
			mov ax,START_X

			inc START_Y

			jmp SkipValidation

			 GenerateTrackDir_mid2:
			jmp GenerateTrackDir_loop
		;*--------------------------------------------------------------------;
		case7:
		cmp random_part , 7
		jnz case8
		;*-------------------------------Case7--------------------------------;
            mov ax,START_X
			add ax,LINE_WIDTH
			add ax,BOUNDARY_WIDTH*2
			dec ax
			mov x_max_new,ax
               

			sub ax,LINE_LENGTH

			;! updating
			mov START_X,ax

			inc  ax
			mov x_min_new,ax

			mov ax,START_Y
			mov y_max_new,ax

			sub ax,LINE_WIDTH
			sub ax,BOUNDARY_WIDTH*2
			inc ax
			mov y_min_new,ax

		 	jmp GenerateTrackDir_FlagCheck   
		;*--------------------------------------------------------------------;
		case8:
		cmp random_part , 8
		jnz case9
		;*-------------------------------Case8--------------------------------;
         	mov ax,START_X
			mov x_min_new,ax

			add ax,LINE_LENGTH

            ;! updating
			mov START_X,ax

			dec ax
			mov x_max_new,ax

			mov ax,START_Y
			mov y_min_new,ax

			add ax,LINE_WIDTH
			add ax,BOUNDARY_WIDTH*2

            ;! updating
			mov START_Y,ax

			dec ax
			mov y_max_new,ax

			jmp GenerateTrackDir_FlagCheck
		;*--------------------------------------------------------------------;
		case9:
		cmp random_part , 9
		jnz case10
		;*-------------------------------Case9--------------------------------;
            mov ax,[di-2]
			mov y_max_new,ax

			mov ax,[di-4]
			mov y_min_new,ax

			mov ax,[di-6]
			mov x_max_new,ax

			mov ax,[di-8]
			mov x_min_new,ax

			;! updating
			inc START_X

			mov ax,START_Y
			sub ax,BOUNDARY_WIDTH*2					
			sub ax,LINE_WIDTH

			jmp SkipValidation

		;*--------------------------------------------------------------------;
		case10:
		cmp random_part , 10
		jnz case11
		;*-------------------------------Case10--------------------------------;
         	mov ax,START_X
			add ax,LINE_WIDTH
			add ax,BOUNDARY_WIDTH*2
			dec ax
			mov x_max_new,ax


			sub ax,LINE_LENGTH

			;! updating
			mov START_X,ax

            inc ax
			mov x_min_new,ax

			mov ax,START_Y
			mov y_min_new,ax

			add ax,LINE_WIDTH
			add ax,BOUNDARY_WIDTH*2
			dec ax

			;! updating
			mov START_Y,ax	

			mov y_max_new,ax

			jmp GenerateTrackDir_FlagCheck

		;*---------------------------------------------------------------------;
		case11:
		cmp random_part , 11 
		jnz GenerateTrackDir_FlagCheck
		;*-------------------------------Case11--------------------------------;
			mov ax,[di-2]
			mov y_max_new,ax

			mov ax,[di-4]
			mov y_min_new,ax

			mov ax,[di-6]
			mov x_max_new,ax

			mov ax,[di-8]
			mov x_min_new,ax
			
			;! updating
			mov ax,START_X

			sub ax,BOUNDARY_WIDTH*2
			sub ax,LINE_WIDTH

			mov START_X,ax
            

			mov ax,START_Y
			sub ax,BOUNDARY_WIDTH*2					
			sub ax,LINE_WIDTH
            mov START_Y,ax
			
			jmp SkipValidation
			
		    GenerateTrackDir_mid:
			jmp GenerateTrackDir_mid2
		;*---------------------------------------------------------------------;
	    GenerateTrackDir_FlagCheck:
        
		push si
        call ValidateTrack ;? call ValidateTrack with new x,y
        pop si

		cmp TrackCheckFlag,1 ;? check if it is valid
        jnz GenerateTrackDir_wrong

            SkipValidation:

			mov ax,x_min_new
			mov [di],ax
			mov ax,x_max_new
			mov [di+2],ax
			mov ax,y_min_new
			mov [di+4],ax
			mov ax,y_max_new
			mov [di+6],ax
			add di,8

            
            mov ax,0
			mov al,random_part
			mov [si],ax
			add si,2

          
			mov ax,START_X
			mov [bx],ax
			mov ax,START_Y
			mov [bx+2],ax
			add bx,4

			inc TotalParts
		    

			jmp GenerateTrackDir_still
			
            GenerateTrackDir_mid3:
			jmp GenerateTrackDir_mid
        ;*----------------Wrong Direction-----------------;
        GenerateTrackDir_wrong:

			mov ax,prev_start_x
			mov START_X,ax
			mov ax,prev_start_y
			mov START_Y,ax

			inc WrongCounter

			jmp jadksl
             GenerateTrackDir_mid6:
			 jmp  GenerateTrackDir_mid3
			jadksl:


			cmp WrongCounter,WRONGTHRESHOLD
			jnz GenerateTrackDir_still

				mov WrongCounter,0
				
				cmp si,startoffsetdirection
				jz GenerateTrackDir_still

				sub si,2

				cmp di,startoffsetclosedarea
				jz GenerateTrackDir_still
				sub di,8
				
				
				cmp bx,startoffsetprevstart
				jz GenerateTrackDir_still 

				sub bx,2
				mov ax,[bx]
				mov START_Y,ax
				
				cmp bx,startoffsetprevstart
				jz GenerateTrackDir_still
				
				sub bx,2
				mov ax,[bx]
				mov START_X,ax

				dec TotalParts

        	GenerateTrackDir_still:
        ;*-----------------------------------------------;

		cmp TotalParts,MAX_PARTS
	jnz GenerateTrackDir_mid6
	ret
GenerateTrackDirections ENDP	
ValidateTrack PROC ;! Change Value of ax and si and dependent on the new values of x,y. they should be updated before calling this procedure
      
	  mov cx,START_X
	  mov dx,START_Y
	;---------------------------check for bounadry conditions---------------------------------;
			cmp cx,MIN_X
			jge ValidateTrack_check1
			jmp wrong
		;----------------------------------------------------------------------------------------------------------------;
		ValidateTrack_check1: 
			cmp cx,MAX_X
			jle ValidateTrack_check2
			jmp wrong

		ValidateTrack_check2:
			cmp dx,MIN_Y
			jge ValidateTrack_check3
			jmp wrong

		ValidateTrack_check3:
			cmp dx,MAX_Y
			jle CheckIntersection
			jmp wrong 

	CheckIntersection:

   lea si,ClosedArea
   mov cx,0
   for1:

	mov ax,[si]
	mov x_min_old,ax

	mov ax,[si+2]
	mov x_max_old,ax

	mov ax,[si+4]
	mov y_min_old,ax

	mov ax,[si+6]
	mov y_max_old,ax
	
    call Validator
	cmp SingleCheckFlag,0
	jz wrong 
     
	add si,8
	inc cx

    cmp cx,TotalParts  
	jle for1

    mov TrackCheckFlag,1
	ret 
   
    wrong:
    mov TrackCheckFlag,0
	ret
ValidateTrack ENDP

Validator PROC ;! Change Value of ax only and dependent on the old and new values
   mov ax,x_min_old
  cmp x_max_new,ax
  jl passed 
  
   mov ax,x_max_old
  cmp x_min_new,ax
  jg passed 

   mov ax,y_max_old
  cmp y_min_new,ax
  jg passed 

   mov ax,y_min_old
  cmp y_max_new,ax
  jl passed 
  
  mov SingleCheckFlag , 0 
  ret 
  
  passed:
  mov SingleCheckFlag , 1
  ret
Validator ENDP

;! ------------ all next Proc changes the values of all registeres--------------------------;

GenerateVerticalTrack PROC ;! Dependent on the START_X and START_Y  and does not change them

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

GenerateHorizontalTrack PROC
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
            jmp next_horizontal
            drawRed:
                cmp al, RED ;check if red boundary
                jnz drawWhite
                int 10h
                dec bl
                cmp bl, 0
                jnz next_horizontal
                mov bl, BOUNDARY_LENGTH
                mov al, WHITE
                jmp next_horizontal
            mid1_horizontal:
                jmp drawLine
            mid2_horizontal:
                jmp drawRoad
            drawWhite:
                cmp al, WHITE ;check if white boundary
                jnz drawDashes
                int 10h
                dec bl
                cmp bl, 0
                jnz next_horizontal
                mov bl, BOUNDARY_LENGTH
                mov al, RED
                jmp next_horizontal
            drawDashes:
                cmp bh, DASHEDLINE_LENGTH ;check if normal road or dashed line
                jg alter
                int 10h
                dec bh
                cmp bh, 0
                jnz next_horizontal
                mov bh, 2*DASHEDLINE_LENGTH
                jmp next_horizontal
                alter:
                    mov al, GRAY
                    int 10h
                    dec bh
                    mov al, DASHESCOLOR
            next_horizontal:
            cmp horizontalDirection, 0 ;check if right
            jnz left
            inc cx
            jmp right
            left:
                dec cx
            right:
            dec di
            cmp di, 0
            jnz mid1_horizontal
        dec dx
        dec si
        cmp si, 0
        jnz mid2_horizontal
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
 GenerateHorizontalTrack  ENDP

rndm proc 
    
    mov cx, 1

outer_loop: 
    push cx
    mov ah, 2Ch
    int 21h 
    pop cx
    mov ax, dx
    mov bl, 3
    mul bl
    mov bl, 137
    div bl
    mov al,ah
    mov ah, 17
    mov bx, ax


inner_loop: 
    push cx
    push bx
    mov ah, 2Ch
    int 21h    
    
    mov al,dl
    mov bl, 5
    mul bl

    MOV BX, multiplier
    MUL BX               ; AX = seed * multiplier
    ADD AX, increment    ; AX = AX + increment
    MOV CX, modulus      ; CX = modulus (2^16)
    DIV CX               ; AX = AX / CX, DX = AX % CX (remainder)
    mov bh, 12
    div bh
    mov random_part, ah
        
    pop bx
    pop cx

    dec bx 
    cmp bx, 0
    jnz inner_loop

    dec cx 
    cmp cx, 0
    jnz outer_loop

    ret
rndm endp

checkCases proc
    mov validationFlag,0
    mov al, lastRandom
    mov bl, 3
    mul bl
    mov bh, 0
    mov bl, al
    mov al, casse0[bx]
    mov cx,3
    loop1:
        mov al, casse0[bx]
        cmp al, currentRandom
        je valid
        inc bx
        loop loop1
    jmp endCheckCases
    valid:
        mov validationFlag, 1
    endCheckCases:
    ret
checkCases endp

end main