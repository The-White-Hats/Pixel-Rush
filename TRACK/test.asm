
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
		 BOUNDARY_COLOR1 equ WHITE
		 BOUNDARY_COLOR2 equ RED
		 EndTrackColor1 db GRAY
		 EndTrackColor2 db LIGHT_GRAY
		 EndTrackWidthToggle equ 4
         DefaultBackground equ GREEN
         DASHESCOLOR equ LIGHT_GRAY
         ;*----------------------------------Positions-------------------------------------------------;      
		 START_X dw 2
         START_Y dw 150
         START_DIR db 0
         END_X dw ?
         END_Y dw ?
         ;*----------------------------------BOUNDARY-------------------------------------------------;      
         MIN_X equ 0
         MAX_X equ 319
         MIN_Y equ 0
         MAX_Y equ 150 
         ;*----------------------------------Dimensions-------------------------------------------------;
         LINE_WIDTH equ 20
         LINE_LENGTH equ 5
		 HORIZONTAL_LINE_LENGTH equ LINE_WIDTH+1
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
        
        MAX_PARTS equ 50
        TIME equ 0
		WRONGTHRESHOLD equ 12

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
		s db 0
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

    mov CurrentCase,0
    mov START_X , 2
    mov START_Y , 150
    call FinalLine

    MOV    AH,0               
    INT    16H

    mov ah, 4ch        
    INT 21h         
main endp

FinalLine PROC
    call ModifyFinalStart

    call ModifyEachIteration

    call SingleLine

	mov cx,LINE_WIDTH

    mov EndTrackColor2,BLACK
    mov EndTrackColor1,WHITE
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

    mov EndTrackColor1,GRAY
    mov EndTrackColor2,LIGHT_GRAY

	call ModifyEachIteration
	call SingleLine

	ret
FinalLine ENDP
SingleLine PROC
	   pusha
        mov cx,START_X
        mov dx,START_Y
        mov posx,cx
        mov posy,dx
		mov cx,LINE_WIDTH
        mov boundaryflag,0
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
				cmp toggleboundarycolor,BOUNDARY_LENGTH
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

					inc posy
					jmp FinalLine_skip

				itsdown:
					dec posy
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

	  cmp CurrentCase,4d
	   jnz nextCasel
	   		dec START_Y
			ret
	   nextCasel:
	   cmp CurrentCase,6d
	   jnz nextCase2l
	   		dec START_X
			add START_X,LINE_WIDTH+BOUNDARY_WIDTH*2
			ret
	   nextCase2l:
       cmp CurrentCase,7d
	   jnz nextCase3l
			inc START_X
			ret
       nextCase3l:
	   cmp CurrentCase,8d
	   jnz nextCase4l
			dec START_X
			ret
	   nextCase4l:
	   cmp CurrentCase,9d
	   jnz nextCase5l
			dec START_Y
			add START_Y,LINE_WIDTH+BOUNDARY_WIDTH*2
			ret
	   nextCase5l:
        cmp CurrentCase,10d
	   jnz nextCase6l
	        inc START_X
			ret
		nextCase6l:	
        cmp CurrentCase,11d
	    jnz nextCase7
		dec START_X
		ret
        nextCase7:	

		  cmp CurrentCase,5d
	    jnz nextCase8
		dec START_Y
        ret
        nextCase8:	

        cmp CurrentCase,0d
	    jnz nextCase9
		dec START_Y
		ret
        nextCase9:	

		  cmp CurrentCase,1d
	    jnz nextCase10
		inc START_Y
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
 cmp CurrentCase,0
	 jnz FinalLine_case1

      inc START_X
       mov isvertical,1
	   mov isup_right,0
	  ret

    FinalLine_case1:

	 cmp CurrentCase,1
	 jnz FinalLine_case2
      inc START_X
      mov isvertical,1
	   mov isup_right,1
	  ret

    FinalLine_case2:

	 cmp CurrentCase,8d
	 jz righthorizontal
	 cmp CurrentCase,11d
	 jz righthorizontal
	 cmp CurrentCase,2d
	 jz righthorizontal
	 cmp CurrentCase,6d
	 jz righthorizontal
	 cmp CurrentCase,5d
	 jz righthorizontal
	 jmp lefthorizontal

     righthorizontal:
       dec START_Y
	   mov isvertical,0
	   mov isup_right,0
	   ret

    lefthorizontal:
        dec START_Y
        mov isvertical,0
	    mov isup_right,1 
     ret
ModifyEachIteration ENDP
end main