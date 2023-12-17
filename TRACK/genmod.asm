
include draw.inc
.286
.model small
.stack 128
.data
    ;*----------------------------------COLORS------------------------------------------------------;
         TEXT_COLOR equ 0    ; DefaultBackground text on BOUNDARY_COLOR2 background
         SCREEN_ATTR equ 0F0h ; BOUNDARY_COLOR2 background (high nibble) and DefaultBackground text (low nibble)
         DASHEDLINEATTR equ 0F0h ; 
         RED equ 04h
         LIGHT_GREEN equ 0Ah
         GREEN equ 02h
         LIGHT_GRAY equ 07h
         GRAY equ 08h
         WHITE equ 0Fh
         BLACK equ 00h
		 CYAN equ 03h
		 LIGHT_CYAN equ 0bh	
		 Magenta equ 05h
		 Brown equ 06h
		 BLUE equ 01h
		 BOUNDARY_COLOR1 equ BLUE
		 BOUNDARY_COLOR2 equ RED

		 EndTrackColor1 db BLACK ;!Finish Line
		 EndTrackColor2 db WHITE ;!Finish Line

		

         DefaultBackground equ GREEN
         DASHESCOLOR equ LIGHT_GRAY
         ;*----------------------------------Positions-------------------------------------------------;      
		 START_X dw 2
         START_Y dw 150
         START_DIR db 0
         END_X dw ?
         END_Y dw ?
         ;*----------------------------------BOUNDARY-------------------------------------------------;      
         MIN_X equ 1
         MAX_X equ 319
         MIN_Y equ 1
         MAX_Y equ 150 
         ;*----------------------------------Dimensions-------------------------------------------------;
         LINE_WIDTH equ 25
         LINE_LENGTH equ 5
		 HORIZONTAL_LINE_LENGTH equ 26

		 END_LINE_WIDTH equ LINE_WIDTH ;!Finish Line
		 END_LINE_LENGTH equ 6 ;!Finish Line
		 END_LINE_DASHEDLINE_LENGTH equ 2 ;!Finish Line
         EndTrackWidthToggle equ 2

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
		 isvertical db 0 ;! 1 vertical ,0 horizontal
		 isup_right db 0 ;! 1 up or right 0 oppisite
        ;*----------------------------------Track Directions Generation Variables-------------------------------------------------;
        
        MAX_PARTS equ 60
        TIME equ 0
		WRONGTHRESHOLD equ 12

		prev_start_x dw ?
		prev_start_y dw ?

		last_success_x dw ?
        last_success_y dw ?
		last_success_dir db ?

		currentValididation db ?
        
        x_max_new dw ?
        x_min_new dw ?

        x_max_old dw ?
        x_min_old dw ?

        y_max_new dw ?
        y_min_new dw ?

        y_max_old dw ?
        y_min_old dw ?

        Directions dw MAX_PARTS+1 dup(0)

        ClosedArea dw (MAX_PARTS+1)*4 dup(0) ;! Xmin, Xmax, Ymin, Ymax
        
		PrevStart  dw (MAX_PARTS+1)*2 dup(0) ;! START_X,START_Y

		PartWrongs dw (MAX_PARTS+1) dup(0) ;! number of wrongs for each part

		PartWrongsOffset dw 0

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
        
		helper db 1H
		divider db 3H
		random db 0
		resetTH db 1
		;*------------------------------------------- CASES -------------------------------------------; 
		lastRandom db 0
        currentRandom db 0
		validationFlag db 0
		casse0 db 0,5,7
		casse1 db 8,10,1
		casse2 db 2,6,11
		casse3 db 3,4,9
		casse4 db 1,1,1
		casse5 db 2,2,2
		casse6 db 1,1,1
		casse7 db 3,3,3
		casse8 db 2,2,2
		casse9 db 0,0,0
		casse10 db 3,3,3
		casse11 db 0,0,0
        s db 0
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
		mov ax ,0600h
		mov bh,DefaultBackground
		mov cx,0h
		mov dx , 184fh
    	int 10h
    ;-----------------------------------------------------------------------------------------------------;


    stresstest:

	    
      	call RandomStart

        mov helper,1
		call GenerateTrackDirections

		call Draw

		call FinalLine

		MOV    AH,0               
		INT    16H

		; clear

		; mov ax ,0600h
		; mov bh,DefaultBackground
		; mov cx,0h
		; mov dx , 184fh
		; int 10h

	; mp stresstest

	; call GenerateTrackDirections

 	; call Draw

    MOV    AH,0               
    INT    16H

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
    ;*--------------------------------------Check that the last direction is horizontal or vertical------;
    add si, 2*MAX_PARTS-2
    mov ax,0
    modify_maxparts:
	  cmp byte ptr [si],3d
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
	ret
Draw ENDP

GenerateTrackDirections PROC 
    Restart:
	mov ah, 2Ch
	int 21H    ; puts the millseconds in dl
	add dl,50		; threshold time
	mov al, dl ; contain hundreds of seconds
	mov bl,100
	mov ah,0
	xor dx,dx
	div bl
	mov resetTH,ah

    call RandomStart
	 mov helper, 1

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
       
        pusha
		mov ah, 2Ch
		int 21H    ; puts the millseconds in dl
		mov al, dl ; contain hundreds of seconds
		mov bl,100
		mov ah,0
		xor dx,dx
		div bl
		cmp ah,resetTH
		jge Restart
		popa

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

	
 		mov cx,0
		;*-------------------------------Case0--------------------------------;
		    mov currentValididation,0
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

            call ValidateTrack ;? call ValidateTrack with new x,y

		    cmp TrackCheckFlag,0 ;? check if it is valid
            jz case0_dontincrementCX
			    mov last_success_dir,0
				mov ax,START_X
				mov last_success_x,ax
				mov ax,START_Y
				mov last_success_y,ax
            cmp cl,random 
			jz case0_Validated 

			jmp case0_bridge

				case0_Validated:
				
				jmp Accepted

			case0_bridge:

			inc cx
            case0_dontincrementCX:
            mov ax,prev_start_x
			mov START_X,ax
			mov ax,prev_start_y
			mov START_Y,ax
        
		;*--------------------------------------------------------------------;
		
		;*-------------------------------Case1--------------------------------;
		    mov currentValididation,1
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


		    call ValidateTrack ;? call ValidateTrack with new x,y

		    cmp TrackCheckFlag,0 ;? check if it is valid
            jz case1_dontincrementCX
				mov last_success_dir,1
				mov ax,START_X
				mov last_success_x,ax
				mov ax,START_Y
				mov last_success_y,ax
				
            cmp cl,random
			jz case1_Validated 

			jmp case1_bridge

				case1_Validated:
				
				jmp Accepted

			case1_bridge:

			inc cx
            case1_dontincrementCX: 
			mov ax,prev_start_x
			mov START_X,ax
			mov ax,prev_start_y
			mov START_Y,ax  
        ;*--------------------------------------------------------------------;

		;*-------------------------------Case2--------------------------------;
		    mov currentValididation,2
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

			call ValidateTrack ;? call ValidateTrack with new x,y

		    cmp TrackCheckFlag,0 ;? check if it is valid
            jz case2_dontincrementCX
			mov last_success_dir,2
				mov ax,START_X
				mov last_success_x,ax
				mov ax,START_Y
				mov last_success_y,ax

            cmp cl,random 
			jz case2_Validated 

			jmp case2_bridge

				case2_Validated:
				
				jmp Accepted

			case2_bridge:

			inc cx
            case2_dontincrementCX:
			mov ax,prev_start_x
			mov START_X,ax
			mov ax,prev_start_y
			mov START_Y,ax
        ;*--------------------------------------------------------------------;

		;*-------------------------------Case3--------------------------------;
		    mov currentValididation,3
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

			call ValidateTrack ;? call ValidateTrack with new x,y

		    cmp TrackCheckFlag,0 ;? check if it is valid
            jz case3_dontincrementCX
			mov last_success_dir,3
				mov ax,START_X
				mov last_success_x,ax
				mov ax,START_Y
				mov last_success_y,ax

            cmp cl,random 
			jz case3_Validated 

			jmp case3_bridge

				case3_Validated:
				
				jmp Accepted

			case3_bridge:

			inc cx
            case3_dontincrementCX:
			mov ax,prev_start_x
			mov START_X,ax
			mov ax,prev_start_y
			mov START_Y,ax
        ;*--------------------------------------------------------------------;

		;*-------------------------------Case4--------------------------------;
			mov currentValididation,4
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

            cmp random_part,4
			jnz staynormal
			jmp SkipValidation
			staynormal:

			call ValidateTrack ;? call ValidateTrack with new x,y

		    cmp TrackCheckFlag,0 ;? check if it is valid
            jz case4_dontincrementCX
			mov last_success_dir,4
				mov ax,START_X
				mov last_success_x,ax
				mov ax,START_Y
				mov last_success_y,ax

            cmp cl,random
			jz case4_Validated 

			jmp case4_bridge

				case4_Validated:
				
				jmp Accepted

			case4_bridge:

			inc cx
            case4_dontincrementCX:
			mov ax,prev_start_x
			mov START_X,ax
			mov ax,prev_start_y
			mov START_Y,ax
		;*--------------------------------------------------------------------;

		;*-------------------------------Case5--------------------------------;
			mov currentValididation,5
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
            
			
			call ValidateTrack ;? call ValidateTrack with new x,y

		    cmp TrackCheckFlag,0 ;? check if it is valid
            jz case5_dontincrementCX
			mov last_success_dir,5
				mov ax,START_X
				mov last_success_x,ax
				mov ax,START_Y
				mov last_success_y,ax

            cmp cl,random
			jz case5_Validated 

			jmp case5_bridge

				case5_Validated:
				
				jmp Accepted

			case5_bridge:

			inc cx
            case5_dontincrementCX:
			mov ax,prev_start_x
			mov START_X,ax
			mov ax,prev_start_y
			mov START_Y,ax
		;*--------------------------------------------------------------------;

		;*-------------------------------Case6--------------------------------;
			mov currentValididation,6
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

			cmp random_part,6
			jnz staynormal2
			jmp SkipValidation
			staynormal2:
			
			call ValidateTrack ;? call ValidateTrack with new x,y

		    cmp TrackCheckFlag,0 ;? check if it is valid
            jz case6_dontincrementCX
			mov last_success_dir,6
				mov ax,START_X
				mov last_success_x,ax
				mov ax,START_Y
				mov last_success_y,ax

            cmp cl,random
			jz case6_Validated 

			jmp case6_bridge
									GenerateTrackDir_mid2:
									jmp GenerateTrackDir_loop
				case6_Validated:
				
				jmp Accepted

			case6_bridge:

			inc cx
            case6_dontincrementCX:
            mov ax,prev_start_x
			mov START_X,ax
			mov ax,prev_start_y
			mov START_Y,ax			
		;*--------------------------------------------------------------------;

		;*-------------------------------Case7--------------------------------;
			mov currentValididation,7
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

		 	
			call ValidateTrack ;? call ValidateTrack with new x,y

		    cmp TrackCheckFlag,0 ;? check if it is valid
            jz case7_dontincrementCX
			mov last_success_dir,7
				mov ax,START_X
				mov last_success_x,ax
				mov ax,START_Y
				mov last_success_y,ax

            cmp cl,random
			jz case7_Validated 

			jmp case7_bridge

				case7_Validated:
				
				jmp Accepted

			case7_bridge:

			inc cx
            case7_dontincrementCX:  
			mov ax,prev_start_x
			mov START_X,ax
			mov ax,prev_start_y
			mov START_Y,ax
		;*--------------------------------------------------------------------;

	    ;*-------------------------------Case8--------------------------------;
			mov currentValididation,8
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
			
			
			call ValidateTrack ;? call ValidateTrack with new x,y

		    cmp TrackCheckFlag,0 ;? check if it is valid
            jz case8_dontincrementCX
			mov last_success_dir,8
				mov ax,START_X
				mov last_success_x,ax
				mov ax,START_Y
				mov last_success_y,ax

            cmp cl,random
			jz case8_Validated 
				
			jmp case8_bridge

				case8_Validated:
			
				jmp Accepted

			case8_bridge:

			inc cx
            case8_dontincrementCX:
			mov ax,prev_start_x
			mov START_X,ax
			mov ax,prev_start_y
			mov START_Y,ax
		;*--------------------------------------------------------------------;

		;*-------------------------------Case9--------------------------------;
		    mov currentValididation,9
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
			
            cmp random_part,9
			jnz staynormal3
			jmp SkipValidation
			staynormal3:
            
			call ValidateTrack ;? call ValidateTrack with new x,y

		    cmp TrackCheckFlag,0 ;? check if it is valid
            jz case9_dontincrementCX
			mov last_success_dir,9
				mov ax,START_X
				mov last_success_x,ax
				mov ax,START_Y
				mov last_success_y,ax

            cmp cl,random
			jz case9_Validated 
				
			jmp case9_bridge

				case9_Validated:
				
				jmp Accepted

			case9_bridge:

			inc cx
            case9_dontincrementCX:
            mov ax,prev_start_x
			mov START_X,ax
			mov ax,prev_start_y
			mov START_Y,ax
		;*--------------------------------------------------------------------;

		;*-------------------------------Case10--------------------------------;
			mov currentValididation,10
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

			
			call ValidateTrack ;? call ValidateTrack with new x,y

		    cmp TrackCheckFlag,0 ;? check if it is valid
            jz case10_dontincrementCX
			mov last_success_dir,10
				mov ax,START_X
				mov last_success_x,ax
				mov ax,START_Y
				mov last_success_y,ax

            cmp cl,random
			jz case10_Validated 
				
			jmp case10_bridge

				case10_Validated:
				
				jmp Accepted

			case10_bridge:

			inc cx
            case10_dontincrementCX:
			mov ax,prev_start_x
			mov START_X,ax
			mov ax,prev_start_y
			mov START_Y,ax
		;*---------------------------------------------------------------------;

		;*-------------------------------Case11--------------------------------;
			mov currentValididation,11
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

			cmp random_part,11
			jnz staynormal4
			jmp SkipValidation
			staynormal4:
			
			call ValidateTrack ;? call ValidateTrack with new x,y

		    cmp TrackCheckFlag,0 ;? check if it is valid
            jz case11_dontincrementCX
			mov last_success_dir,11
			mov ax,START_X
			mov last_success_x,ax
			mov ax,START_Y
			mov last_success_y,ax

            cmp cl,random
			jz case11_Validated 
			
			jmp case11_bridge

				case11_Validated:
				jmp Accepted
									GenerateTrackDir_mid:
									jmp GenerateTrackDir_mid2
			case11_bridge:

			inc cx
            case11_dontincrementCX:
			mov ax,prev_start_x
			mov START_X,ax
			mov ax,prev_start_y
			mov START_Y,ax
		;*---------------------------------------------------------------------;
		cmp cx,0
		jnz Accepted
	    jmp GenerateTrackDir_wrong
	
            Accepted:

            mov ax,last_success_x
			mov START_X,ax
			mov ax,last_success_y
			mov START_Y,ax
			mov al, last_success_dir
			mov random_part,al

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

			jmp dont_try
             GenerateTrackDir_mid6:
			 jmp  GenerateTrackDir_mid3
			dont_try:

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
	call CheckCases
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
			  call Delay
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
	        call Delay
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
    sub dx, BOUNDARY_WIDTH


    exec:
    mov si, LINE_WIDTH
	add si, BOUNDARY_WIDTH

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
    dec dx
    dec si
    cmp si,0
    jnz open1
	call GenerateCorner
    final:
	popa
	ret
 GenerateHorizontalTrack  ENDP
 
GenerateCorner PROC 
	pusha
	mov al, DASHESCOLOR
	mov cx, START_X
	mov dx, START_Y
	mov si, LINE_WIDTH/2
	sub si, DASHEDLINE_LENGTH
	mov di, LINE_WIDTH/2
	sub di, DASHEDLINE_LENGTH

	cmp cornerType, 0
	jnz upOpenedCorner
	sub dx, DASHEDLINE_LENGTH
	sub dx, BOUNDARY_WIDTH
	jmp checkX
	upOpenedCorner:
	sub dx, LINE_WIDTH
	add dx, DASHEDLINE_LENGTH

	checkX:
	cmp horizontalDirection, 0
	jnz leftCorner
	add cx, LINE_WIDTH/2
	add cx, BOUNDARY_WIDTH
	jmp drawCorner
	leftCorner:
	sub cx, LINE_WIDTH/2
	sub cx,BOUNDARY_WIDTH
	
	drawCorner:
	mov ah, 0ch	
	DrawVerticalCorner:
		int 10h
		cmp cornerType, 0
		jnz DrawUpOpenedCorner
		dec dx
		jmp DrawVerticalCornerCheck
		DrawUpOpenedCorner:
		inc dx
		DrawVerticalCornerCheck:
		dec si
		cmp si, 0
		jnz DrawVerticalCorner

	DrawHorizontalCorner:
		int 10h
		cmp horizontalDirection, 0
		jnz DrawLeftCorner
		inc cx
		jmp DrawHorizontalCornerCheck
		DrawLeftCorner:
		dec cx
		DrawHorizontalCornerCheck:
		dec di
		cmp di, 0
		jnz DrawHorizontalCorner
	
	popa
	ret
GenerateCorner ENDP

randomizer PROC
  pusha
  mov ah, 2Ch
  int 21H    ; puts the millseconds in dl
  mov al, dl ; contain hundreds of seconds

  mov ah, 0
  div helper ; divide ax by the helper which keep changing; puts the quotiant in al , and remainder in ah
  mov ah, 0  
  div divider
  mov random, ah ; puts the remainder in random

  inc helper    ; increment helper to insure random value every time
  cmp helper, 05d ; return helper to 1 if it's ffh to avoid dividing by zero
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
ENDP
CheckCases PROC
	pusha
	mov al, lastRandom
    mov bl, 3
    mul bl
    mov bh, 0
    mov bl, al

    mov al, casse0[bx]
	cmp al,currentValididation
	jz ok

	mov al, casse0[bx+1]
	cmp al,currentValididation
	jz ok

	mov al, casse0[bx+2]
	cmp al,currentValididation
	jz ok

	mov TrackCheckFlag,0
	popa
	ret

	ok:
	popa
	ret
CheckCases ENDP
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
end main