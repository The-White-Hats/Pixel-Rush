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
	pusha
	mov saveObstaclesCounter,0
	mov saveCompilationPowerupsCounter,0
	mov saveCompilationPowerupsClrsCounter,0
	mov saveInGamePowerupsCounter,0
	mov saveInGamePowerupsClrsCounter,0
	mov loadCompilationPowerupsClrsCounter,0

	lea si,Directions
    lea bx,PrevStart
	
	mov ax,[bx]
	mov START_X,ax
	mov ax,[bx+2]
	mov START_Y,ax
    add bx,4
    ;*--------------------------------------Check that the last direction is horizontal or vertical------;
    add si, 2*MAX_PARTS-2
    mov ax,0
    modify_maxparts:
	  cmp  [si],3d
	  jle Draw_break
	  inc ax
	  sub si,2 
    loop modify_maxparts 
    Draw_break:
    ;*--------------------------------------------------------------------------------------------------;
    lea si,Directions
    mov cx,MAX_PARTS
	sub cx,ax
	mov TotalParts,cx
	iterate:

	   mov ax,[si]
	   mov CurrentCase ,ax

	    cmp CurrentCase,4d
	   jnz nextCase
	   		inc START_X
			jmp ClearToGo
	   nextCase:
	   cmp CurrentCase,6d
	   jnz nextCase2
	   		dec START_X
			jmp ClearToGo
	   nextCase2:
       cmp CurrentCase,7d
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
	   cmp CurrentCase,8d
	   jnz nextCase4
			mov ax,START_Y
			add ax,LINE_WIDTH
			add ax,BOUNDARY_WIDTH*2
			dec ax
			mov START_Y,ax
			jmp ClearToGo
	   nextCase4:
	   cmp CurrentCase,9d
	   jnz nextCase5
			inc START_X
			jmp ClearToGo
	   nextCase5:
        cmp CurrentCase,10d
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
        cmp CurrentCase,11d
	    jnz ClearToGo
		  dec START_X

        ClearToGo: 

	   call SelectType
	 
	   mov ax,[bx]
	   mov START_X,ax
	   mov ax,[bx+2]
	   mov START_Y,ax
	   add bx,4

       add si,2 

		dec cx
		cmp cx,0 
		jg iterate_mid 
    ;*--------------------------------------Draw Obstacles----------------------------------------------;
	pusha 
	lea si, obstacles
	mov cx, 0
	finalDrawObstaclesLoop:
	call DrawObstacle
	add si, 4
	add cx, 4
	cmp cx, saveObstaclesCounter
	jl finalDrawObstaclesLoop
	popa
   ;*--------------------------------------Draw Powerups----------------------------------------------;
	pusha 
	lea si, compilationPowerups
	finalDrawPowerupsLoop:
	call DrawPower
	add si, 4
	add loadCompilationPowerupsClrsCounter, 1
	mov ax, loadCompilationPowerupsClrsCounter
	cmp ax, saveCompilationPowerupsClrsCounter
	jl finalDrawPowerupsLoop
	popa
popa
	ret
Draw ENDP

GenerateTrackDirections PROC 
	Restart:

    call RandomStart

    ; mov ah, 2Ch
	; int 21H    ; puts the millseconds in dl
	; mov al, dl ; contain hundreds of seconds
	; mov starttime,al

    mov helper,1

	lea si,Directions
	lea di,ClosedArea
    lea bx,PrevStart
	lea ax,PartWrongs
	mov PartWrongsOffset,ax

    
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
    
    mov TotalParts,0

	GenerateTrackDir_loop:
	;    pusha
	; 	mov ah, 2Ch
	; 	int 21H    ; puts the millseconds in dl
	; 	mov al, dl ; contain hundreds of seconds
	; 	sub al,starttime
	; 	cmp al,99
	; 	jge Restart
    ;    popa

        mov ax,START_X
		mov prev_start_x,ax
		mov ax,START_Y
		mov prev_start_y,ax
      
		cmp si,startoffsetdirection
		jz resetlastrandom
		mov ax,[si-2]
		mov lastRandom,al
		jmp skipmove

		resetlastrandom:
		mov al,START_DIR
		mov lastRandom,al

		skipmove:  
		
    	call specifiedrandom



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

			add ax,HORIZONTAL_LINE_LENGTH

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

			sub ax,HORIZONTAL_LINE_LENGTH

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

			add ax,HORIZONTAL_LINE_LENGTH

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
			mov START_X,ax

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
               

			sub ax,HORIZONTAL_LINE_LENGTH

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

			add ax,HORIZONTAL_LINE_LENGTH

            ;! updating
			mov START_X,ax

			dec ax
			mov x_max_new,ax

			mov ax,START_Y
			mov y_min_new,ax

			add ax,LINE_WIDTH
			add ax,BOUNDARY_WIDTH*2
			dec ax

            ;! updating
			mov START_Y,ax
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
            mov START_Y,ax
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

			sub ax,HORIZONTAL_LINE_LENGTH

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
        
        call ValidateTrack ;? call ValidateTrack with new x,y

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
            
			add PartWrongsOffset,2
		
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

			cmp lastRandom,4
			jge dont_try

            inc WrongCounter

			jmp jadksl
             GenerateTrackDir_mid6:
			 jmp  GenerateTrackDir_mid3
			jadksl:

			cmp WrongCounter,WRONGTHRESHOLD
			jnz GenerateTrackDir_still
			    dont_try:

				mov WrongCounter,0
	
				cmp TotalParts,0
				jz GenerateTrackDir_still

			
				sub si,2

				sub di,8

                sub bx,4

				mov ax,[bx-2]
				mov START_Y,ax
	
				mov ax,[bx-4]
				mov START_X,ax

				dec TotalParts
                
				push bx
				mov bx,PartWrongsOffset
				mov ax,0
				mov [bx],ax
				sub PartWrongsOffset,2
				mov ax,[bx-2]
				inc ax
				mov [bx-2],ax
				pop bx
				cmp ax,WRONGTHRESHOLD
				jz dont_try
        	GenerateTrackDir_still:
        ;*-----------------------------------------------;

		cmp TotalParts,MAX_PARTS
	jnz GenerateTrackDir_mid6
	ret
GenerateTrackDirections ENDP	

ValidateTrack PROC ;! Change Value of ax and si and dependent on the new values of x,y. they should be updated before calling this procedure
	pusha
	;---------------------------check for bounadry conditions---------------------------------;
			cmp x_min_new,MIN_X
			jge ValidateTrack_check1
			jmp wrong

		ValidateTrack_check1: 
			cmp x_max_new,MAX_X
			jle ValidateTrack_check2
			jmp wrong

		ValidateTrack_check2:
			cmp y_min_new,MIN_Y
			jge ValidateTrack_check3
			jmp wrong

		ValidateTrack_check3:
			cmp y_max_new,MAX_Y
			jle CheckIntersection
			jmp wrong 

	CheckIntersection:
   cmp TotalParts ,0
   jz ValidateTrack_skip
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

    ValidateTrack_skip:

    mov TrackCheckFlag,1
	popa
	ret 
   
    wrong:
    mov TrackCheckFlag,0
	popa
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
        pusha
        mov cx,START_X
        mov dx,START_Y
        mov posx,cx
        mov posy,dx
        mov toggleboundarycolor , 0
		mov toggletrackmiddle , 0
		mov middletrackflag , 0
		movehorizontal:      
			mov boundaryflag ,  0
			  ;call Delay
			drawvertical:
			    mov cx,posx
				mov dx,posy
				
                ;------- if it is the first two lines then draw it with the BOUNDARY_COLOR1 and BOUNDARY_COLOR2 (boundary) else gray and lightgray with toggle-----;
				mov bx,START_X
				add bx,BOUNDARY_WIDTH
				cmp posx,bx
				jge roadcoloring
				
                ;-----------toggle the color of the boundary-----------------;
				boundarycoloring:

				cmp boundaryflag,0
				jnz whitebounadrycolor 
				mov al,BOUNDARY_COLOR1
				jmp boundarytogglecheck
				whitebounadrycolor:
				mov al,BOUNDARY_COLOR2
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
                mov toggleboundarycolor,0
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
                cmp middletrackflag,0   ;Checl if i will color BOUNDARY_COLOR2 incase of middle track
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
        popa
		ret
	GenerateVerticalTrack endp

GenerateHorizontalTrack PROC
	pusha
    mov ah, 0ch
    mov si, LINE_WIDTH + 2*BOUNDARY_WIDTH ; Outer loop counter
    mov dx, START_Y
    drawRoad:
	        ;call Delay
            mov cx, START_X
            mov di, HORIZONTAL_LINE_LENGTH ; Inner loop counter
            mov bl, BOUNDARY_LENGTH
            mov bh, 2*DASHEDLINE_LENGTH
            mov al, GRAY 
            cmp si, LINE_WIDTH + BOUNDARY_WIDTH ;check if upper bound
            jle check123
            mov al, BOUNDARY_COLOR1
        check123:
            cmp si, 2 ;check if lower bound
            jge check25
            mov al, BOUNDARY_COLOR1 
        check25:
            cmp si, LINE_WIDTH/2 +BOUNDARY_WIDTH+1 ;check if middle 
            jnz drawLine
            mov al , DASHESCOLOR ;anything but BOUNDARY_COLOR2 or gray or BOUNDARY_COLOR1
        drawLine:
            cmp al, GRAY ;check if normal road
            jnz drawRed
            int 10h
            jmp next_horizontal
            drawRed:
                cmp al, BOUNDARY_COLOR1 ;check if BOUNDARY_COLOR1 boundary
                jnz drawWhite
                int 10h
                dec bl
                cmp bl, 0
                jnz next_horizontal
                mov bl, BOUNDARY_LENGTH
                mov al, BOUNDARY_COLOR2
                jmp next_horizontal
            mid1_horizontal:
                jmp drawLine
            mid2_horizontal:
                jmp drawRoad
            drawWhite:
                cmp al, BOUNDARY_COLOR2 ;check if BOUNDARY_COLOR2 boundary
                jnz drawDashes
                int 10h
                dec bl
                cmp bl, 0
                jnz next_horizontal
                mov bl, BOUNDARY_LENGTH
                mov al, BOUNDARY_COLOR1
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
    mov al, BOUNDARY_COLOR1
    boundaryOuter:
        mov dx, START_Y
        sub dx, LINE_WIDTH
        dec dx
        mov di, LINE_WIDTH + 2*BOUNDARY_WIDTH 
        boundary:
        drawRed2:
            cmp al, BOUNDARY_COLOR1 ;check if BOUNDARY_COLOR1 boundary
            jnz drawWhite2
            int 10h
            dec bl
            cmp bl, 0
            jnz boundaryNext
            mov bl, BOUNDARY_LENGTH
            mov al, BOUNDARY_COLOR2
            jmp boundaryNext
        drawWhite2:
            cmp al, BOUNDARY_COLOR2 ;check if BOUNDARY_COLOR2 boundary
            jnz drawDashes
            int 10h
            dec bl
            cmp bl, 0
            jnz boundaryNext
            mov bl, BOUNDARY_LENGTH
            mov al, BOUNDARY_COLOR1
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
	popa
	call GenerateObstaclesPowerUps
	ret
 GenerateHorizontalTrack  ENDP

randomizer PROC
    pusha
  mov ah, 2Ch
  int 21H    ; puts the millseconds in dl
  mov al, dl ; contain hundreds of seconds

  mov ah, 0
  div helper ; divide ax by the helper which keep changing; puts the quotiant in al , and remainder in ah
  xor bx, bx
  xor dx, dx
  mov bl, divider
  div bx
  mov random, dl ; puts the remainder in random

  inc helper    ; increment helper to insure random value every time
  cmp helper, 100d ; return helper to 1 if it's ffh to avoid dividing by zero
  jne dontreturn
  mov helper, 1h
  dontreturn:
  popa
  ret
randomizer ENDP

specifiedrandom PROC
	pusha
	mov al, lastRandom
    mov bl, 3
    mul bl
    mov bh, 0
    mov bl, al
    mov al, casse0[bx]

    call randomizer
    mov al,random
	mov ah,0
	add bx,ax
	mov al,casse0[bx]
	mov random_part,al
	popa
	ret
specifiedrandom ENDP

Delay PROC
	    pusha
		MOV AH, 86h ; BIOS delay function
		XOR CX, CX ; High order word of delay
		MOV DX, 10000*TIME ; Low order word of delay (1,000,000 microseconds = 1 second)
		INT 15h ; Call BIOS delay
		popa
		ret
Delay ENDP

RandomStart PROC  ;! first select i will be on the X_MAX,X_MIN,Y_MAX,Y_MIN ->0,1,2,3 , then i will get random piont on this axis
	mov divider,4H
	mov helper,1h
	call randomizer
	;*----------------------------X_MAX-------------------
	cmp random,0
	jnz start_onx_min
	mov START_X,MAX_X
	mov divider,MAX_Y
	call randomizer
	mov ax,0
	mov al,random
	mov START_Y,ax

	cmp al,LINE_WIDTH+2*BOUNDARY_WIDTH
	jge RandomStart_skip
	mov START_Y,LINE_WIDTH+2*BOUNDARY_WIDTH
	RandomStart_skip:

    mov ax,START_Y
	mov START_CAR_Y,ax
	sub START_CAR_Y,START_CAR_SHIFT

	mov ax,START_X
	sub ax,CAR_HEIGHT/2
	mov START_CAR_X,ax

	mov current_frame,6

	mov divider,3H
	mov lastRandom,3
	mov START_DIR,3
	mov helper,1h
	ret
    ;*----------------------------X_MIN-------------------
	start_onx_min:
	cmp random,1
	jnz start_ony_max
	mov START_X,MIN_X
	mov divider,MAX_Y
	call randomizer
	mov ax,0
	mov al,random
	mov START_Y,ax
    
	cmp al,LINE_WIDTH+2*BOUNDARY_WIDTH
	jge RandomStart_skip2
	mov START_Y,LINE_WIDTH+2*BOUNDARY_WIDTH
	RandomStart_skip2:

    mov ax,START_Y
	mov START_CAR_Y,ax
	sub START_CAR_Y,START_CAR_SHIFT

	mov ax,START_X
	add ax,CAR_HEIGHT/2
	mov START_CAR_X,ax

	mov current_frame,2

	mov divider,3H
	mov lastRandom,2
	mov START_DIR,2
	mov helper,1h
	ret
	;*----------------------------Y_MAX-------------------
	start_ony_max:
	cmp random,2
	jnz start_ony_min
	mov START_Y,MAX_Y
	mov divider,255d
	call randomizer
	mov ax,0
	mov al,random
	mov START_X,ax
	add START_X,10

    mov ax,START_Y
	sub ax,CAR_HEIGHT/2
	mov START_CAR_Y,ax

	mov ax,START_X
	mov START_CAR_X,ax
	add START_CAR_X,START_CAR_SHIFT

	mov current_frame,0

	mov divider,3H
	mov lastRandom,0
	mov START_DIR,0
	mov helper,1h
	ret
	;*----------------------------Y_MIN-------------------
	start_ony_min:
	mov START_Y,MIN_Y
	mov divider,255d
	call randomizer
	mov ax,0
	mov al,random
	mov START_X,ax
	add START_X,10
	mov divider,3H
	mov lastRandom,1
	mov START_DIR,1
	mov helper,1h

	mov ax,START_Y
	add ax,CAR_HEIGHT/2
	mov START_CAR_Y,ax

	mov current_frame,4

	mov ax,START_X
	mov START_CAR_X,ax
	add START_CAR_X,START_CAR_SHIFT

	ret
RandomStart ENDP

FinalLine PROC
    call ModifyFinalStart

    call ModifyDirection

	mov cx,END_LINE_WIDTH

    mov ax ,0
	drawfinalline:

     cmp ax,EndTrackWidthToggle
	 jnz FinalLine_still
     mov bl,EndTrackColor2
     mov bh,EndTrackColor1
	 mov EndTrackColor2,bh
     mov EndTrackColor1,bl
	 mov ax,0
	 FinalLine_still:
     call ModifyEachIteration
	 call SingleLine
     inc ax

	loop drawfinalline
	ret
FinalLine ENDP
SingleLine PROC
	   pusha
        mov cx,START_X
        mov dx,START_Y
        mov posx,cx
        mov posy,dx
		mov cx,END_LINE_LENGTH
		mov boundaryflag,0
		mov toggleboundarycolor,0
        drawsingleline:
		        cmp boundaryflag,0
				jnz whitebounadrycolor1 
				mov al,EndTrackColor1
				jmp boundarytogglecheck2
				whitebounadrycolor1:
				mov al,EndTrackColor2
                boundarytogglecheck2:

                ;---------Check if i finfished the boundary length then toggle the flag-----------------;
				inc toggleboundarycolor 
				cmp toggleboundarycolor,END_LINE_DASHEDLINE_LENGTH
				jnz drawsingleline_skip
				mov toggleboundarycolor,0
				xor boundaryflag,1
                drawsingleline_skip:

				push cx
				mov cx,posx
				mov dx,posy
                mov ah,0ch
				int 10h
				pop cx

               cmp isvertical,1
			   jnz itshorizontal

			    cmp isup_right,1
					jnz itsdown

						dec posy
						jmp FinalLine_skip

					itsdown:
					    inc posy
						jmp FinalLine_skip

			   itshorizontal:
			   cmp isup_right,1
				jnz itsleft

					inc posx
					jmp FinalLine_skip

				itsleft:
					dec posx

			FinalLine_skip:
		loop drawsingleline
        popa
		ret
SingleLine ENDP
ModifyFinalStart PROC
	cmp CurrentCase,0d
	jnz nextCase9
	inc START_Y
	ret
	nextCase9:	

	cmp CurrentCase,1d
	jnz nextCase10
	dec START_Y
	ret
	nextCase10:	
	
	cmp CurrentCase,2d
	jnz nextCase11
	dec START_X
	ret

	nextCase11:	
	inc START_X
	ret
ModifyFinalStart ENDP
ModifyEachIteration PROC
    cmp CurrentCase,1
	jg FinalLine_case
    inc START_X
	ret
    FinalLine_case:
    dec START_Y
	ret
ModifyEachIteration ENDP
ModifyDirection PROC
	mov isvertical,1

    cmp CurrentCase,0
	jnz FinalLine_case1
	mov isup_right,0
	ret

    FinalLine_case1:

	cmp CurrentCase,1
	jnz FinalLine_case2
	mov isup_right,1
	ret

    FinalLine_case2:

    mov isvertical,0

	cmp CurrentCase,2d
	jnz lefthorizontal1
	mov isup_right,0
	ret

    lefthorizontal1:
	mov isup_right,1 
    ret
ModifyDirection ENDP

GenerateObstaclesPowerUps PROC
	pusha
	mov divider, 4H
	call randomizer ; randomize the existence of an obstacle
	call randomizer 
	call randomizer 
	cmp random, 1
	jg power1
	mov al, OBSTACLE_COLOR
	jmp initiateDraw
	power1:

	mov divider, 2H   ; randomize the powerup, draw or save for later
	call randomizer 
	mov al, random
	mov nowORthen, al

	typeRandomizer:
	call randomizer ; randomize the type of the powerup
	cmp random, 0
	jnz power34
	call randomizer
	cmp random, 0
	jnz power2
	mov al, POWERUP1_COLOR
	jmp initiateDraw
	power2:
	mov al, POWERUP2_COLOR
	jmp initiateDraw
	power34:
	call randomizer
	cmp random, 0
	jnz power4
	mov al, POWERUP3_COLOR
	jmp initiateDraw
	power4:
	mov al, POWERUP4_COLOR



	initiateDraw:
	cmp al, lastType
	jz typeRandomizer
	cmp al, latestType
	jz typeRandomizer
	mov ah, lastType
	mov latestType, ah
	mov lastType, al
	mov divider, 3H
	call randomizer ; randomize the position of the obstacle
	cmp random, 0
	jnz secondPlace
	mov cx, START_X
	cmp horizontalDirection, 0
	jnz leftDrawObs
	add cx, LINE_LENGTH/4
	jmp yPosition
	leftDrawObs:
	sub cx, LINE_LENGTH/4
	jmp yPosition

	secondPlace:
	cmp random, 1
	jnz thirdPlace
	mov cx, START_X
	cmp horizontalDirection, 0
	jnz leftDrawObs2
	add cx, LINE_LENGTH/2
	jmp yPosition
	leftDrawObs2:
	sub cx, LINE_LENGTH/2
	jmp yPosition


	thirdPlace:
	mov cx, START_X
	cmp horizontalDirection, 0
	jnz leftDrawObs3
	add cx, 3*LINE_LENGTH/4
	jmp yPosition
	leftDrawObs3:
	sub cx, 3*LINE_LENGTH/4
	jmp yPosition

	

	yPosition:
	mov dx, START_Y
	sub dx, BOUNDARY_WIDTH
	push ax
	randomizerLoop:
		call randomizer
		mov ah, random
		mov al, latestPos
		cmp ah, al
		jz randomizerLoop
		mov latestPos, ah
	pop ax
	
	cmp random, 1
	jnz secondYPlace
	sub dx, 3  ; to make it above the track boundary
	jmp saveObs
	secondYPlace:
	cmp random, 0
	jnz thirdYPlace
	sub dx, LINE_WIDTH/3
	jmp saveObs
	thirdYPlace:

	sub dx, 2*LINE_WIDTH/3

	saveObs:
	cmp horizontalDirection, 0 
	jnz saveLeft1
	cmp al, OBSTACLE_COLOR
	jnz centerPowerUps
	add cx, OBSTACLE_WIDTH/2 ; to save the center of the obstacle (x-axis)
	jmp saveObs1
	centerPowerUps:
	add cx, POWERUP_WIDTH/2
	jmp saveObs1
	saveLeft1:
	cmp al, OBSTACLE_COLOR
	jnz savePowerUps2
	sub cx, OBSTACLE_WIDTH/2
	jmp saveObs1
	savePowerUps2:
	sub cx, POWERUP_WIDTH/2


	saveObs1:
	cmp al, OBSTACLE_COLOR
	jnz savePowerUps
	lea si, obstacles
	add si, saveObstaclesCounter

	mov [si], cx
	mov [si+2], dx
	add saveObstaclesCounter, 4
	jmp endGenerateObstaclesPowerUps

	savePowerUps:
	cmp nowORthen, 0
	jnz saveForLater
	lea si, compilationPowerups
	add si, saveCompilationPowerupsCounter
	mov [si], cx
	mov [si+2], dx
	add saveCompilationPowerupsCounter, 4
	lea si, compilationPowerupsClrs
	add si, saveCompilationPowerupsClrsCounter
	mov [si], al
	add saveCompilationPowerupsClrsCounter, 1
	jmp endGenerateObstaclesPowerUps

	saveForLater:
	lea si, inGamePowerups
	add si, saveInGamePowerupsCounter
	mov [si], cx
	mov [si+2], dx
	add saveInGamePowerupsCounter, 4
	lea si, inGamePowerupsClrs
	add si, saveInGamePowerupsClrsCounter
	mov [si], al
	add saveInGamePowerupsClrsCounter, 1


	endGenerateObstaclesPowerUps:
	popa
	ret
GenerateObstaclesPowerUps ENDP

DrawObstacle PROC
	pusha
	mov bx, [si]
	sub bx, OBSTACLE_WIDTH/2
	mov dx, [si+2]
	mov horizontalDirection, 0
	mov ah, 0ch
	mov si, OBSTACLE_WIDTH
	mov al, OBSTACLE_COLOR
	drawObsLoop:
	mov di, OBSTACLE_LENGTH
	mov cx, bx
		drawObsLoop2:
		int 10h
		cmp horizontalDirection, 0
		jnz leftDrawObs4
		inc cx
		jmp drawObsLoop3
		leftDrawObs4:
		dec cx
		drawObsLoop3:
		dec di
		cmp di, 0
		jnz drawObsLoop2
		dec dx
		dec si
		cmp si, 0
		jnz drawObsLoop

		popa
	ret
DrawObstacle ENDP

DrawPower PROC
	pusha
	mov bx, [si]
	sub bx, POWERUP_WIDTH/2
	mov dx, [si+2]
	mov horizontalDirection, 0
	mov si, POWERUP_WIDTH
	push bx
	lea bx, compilationPowerupsClrs
	add bx, loadCompilationPowerupsClrsCounter
	mov al, [bx]
	pop bx
	mov ah, 0ch
	drawPowLoop:
	mov di, POWERUP_LENGTH
	mov cx, bx
		drawPowLoop2:
		int 10h
		cmp horizontalDirection, 0
		jnz leftDrawPow4
		inc cx
		jmp drawPowLoop3
		leftDrawPow4:
		dec cx
		drawPowLoop3:
		dec di
		cmp di, 0
		jnz drawPowLoop2
		dec dx
		dec si
		cmp si, 0
		jnz drawPowLoop

	popa
	ret
DrawPower ENDP
	
;*--------------------------------------------display string  in si and with color 'stringcolor'------------------------------------------	
DisplayString PROC
	pusha
	display_string:
		mov ah,9          ;Display
		mov bh,0          ;!Page does not work here like normal text mode so leave it 0 so that i work properly (noticed this by testing)
		mov al,[si]       
		mov cx,1d               ;1 times
		mov bl,stringcolor       ;?colors are diffrent from text mode
		int 10h
		mov ah,2           
		inc dl
		int 10h 
		inc si
		mov al,[si]
		cmp al,'$'
	jnz display_string
	popa
	ret
DisplayString ENDP

DrawScoreBoard PROC
	pusha
	;----------------------------------------Display user names and scores----------------------------;
		;---------------------------Moving Cursor-------------------;
		mov ah,2           
		mov dl,1  ;*1.the row can write up to 40 characters one character takes 8 pixels
		mov dh,20 ;*2.so the screen is 40*24 char  (24 by testing)
		int 10h 

	lea si , user1name
	mov cl,user1theme
	mov stringcolor , cl
	call DisplayString
     
		mov dh,21 
		int 10h 

	lea si ,scorename
    call DisplayString

		mov dl,20 
		mov dh,20 
		int 10h

	lea si ,user2name
	mov cl,user2theme
	mov stringcolor , cl
    call DisplayString
    
		mov dh,21
		int 10h

	lea si ,scorename
    call DisplayString

	popa
	ret
;--------------------------------------------------------------------------------------------------;
DrawScoreBoard ENDP

UpdateUserScore PROC
	pusha
	lea si,ClosedArea
	mov cx,TotalParts
	mov ax,0
	mov al,usernextpart
	mov bl,8
	mul bl
	add si,ax
	
	mov ax,[si]
	mov x_min_old,ax

	mov ax,[si+2]
	mov x_max_old,ax

	mov ax,[si+4]
	mov y_min_old,ax

	mov ax,[si+6]
	mov y_max_old,ax

	mov ax,x_min_old
	cmp user_posX,ax
	jl notpassed

	
	mov ax,x_max_old
	cmp user_posX,ax
	jg notpassed

	mov ax,y_max_old
	cmp user_posY,ax
	jg notpassed

	mov ax,y_min_old
	cmp user_posY,ax
	jl notpassed
    
	mov ax,0
    mov al,usernextpart
	dec al
	mov bl,100
	mul bl
	mov bx,0
    mov bx,TotalParts
	div bl
	mov userscore,al
	inc usernextpart
    cmp usernextpart,cl
	jnz notpassed
	mov userscore,100
	popa
	ret 
	notpassed:
	popa
	ret
UpdateUserScore ENDP
UpdateUsersScore PROC
	mov ax,user1_posX
    mov user_posX,ax
    mov ax,user1_posY
    mov user_posY,ax
    mov al,user1nextpart
    mov usernextpart,al
    mov al,user1score
    mov userscore,al
    call UpdateUserScore
    mov al,usernextpart
    mov user1nextpart,al
    mov al,userscore
    mov user1score,al

	mov ax,user1_posX
    mov user_posX,ax
    mov ax,user1_posY
    mov user_posY,ax
    mov al,user2nextpart
    mov usernextpart,al
    mov al,user2score
    mov userscore,al
    call UpdateUserScore
    mov al,usernextpart
    mov user2nextpart,al
    mov al,userscore
    mov user2score,al

	call DrawScore

	ret
UpdateUsersScore ENDP

DrawScore PROC
	mov ah,2           
	mov dl,7  ;*1.the row can write up to 40 characters one character takes 8 pixels
	mov dh,21 ;*2.so the screen is 40*24 char  (24 by testing)
	int 10h 

	pusha
	mov ax,0
	mov al,user1score
	shownum
	popa

	mov ah,2           
	mov dl,26  ;*1.the row can write up to 40 characters one character takes 8 pixels
	mov dh,21 ;*2.so the screen is 40*24 char  (24 by testing)
	int 10h 

	pusha
	mov ax,0
	mov al,user2score
	shownum
	popa
	ret
DrawScore ENDP

BackgroundColoring PROC
    ;---------------------------------------Screen Coloring--------------------------------------------;
	mov ax ,0600h
	mov bh,DefaultBackground
	mov cx,0h
	mov dl , 79d
	mov dh , 18d
	int 10h
	;-----------------------------------------------------------------------------------------------------; 
	;----------------------------------------Separating Line----------------------------------------------;
	mov al,SEPARATING_LINE_COLOR
	mov cx,0
	mov dx,MAX_Y+1
	SEPARATING_LINE_draw:
		mov ah,0ch
		int 10h
		inc cx
		cmp cx,320
	jl SEPARATING_LINE_draw
	;-------------------------------------------------------------------------------------------------;
	ret
BackgroundColoring ENDP


