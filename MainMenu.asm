
; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt

		.MODEL SMALL
		.STACK 64
		.DATA   
		 TEXT_COLOR equ 0    ; Black text on white background
         SCREEN_ATTR equ 0F0h ; White background (high nibble) and black text (low nibble)
         DASHEDLINEATTR equ 0F0h ; 

		start_X_ofmain equ 28 
		start_Y_ofmain equ 9
		
        start_X_ofusername equ 20 
		start_Y_ofusername equ 7

		start_X_ofpress equ 1 
		start_Y_ofpress equ 21
        

		start_X_ofstatusbar equ 0 
		start_Y_ofstatusbar equ 20
		         
		user db 'Welcome Newbie : $' 
		user_lastaction db ?
		

		 
		startchatmes db 'To start chatting press F1$'
		startgamemes db 'To start the game press F2$'
		exitmes db 'To end the program press ESC$' 
		closeprogram db 'Program Closed$'
		f1 db 'Entering Chatting ...$'    
		f2 db 'Starting the Game ...$' 
		none db 'I Dont Know !!$'  
  
     	.CODE

		;-------------------------------------Macros--------------------------------------------------------;
         showmes macro str
			mov ah,09h
			lea dx,str
			int 21h  
		endm showmes  
        
		 endlst macro  start     ; give it the start of the vertical cordinate (X-axis)
			; newline
			mov ah,02h
			mov dl,10
			int 21h
			
			mov ah,3
			mov bh,0h
			int 10h
			
			mov ah,2
			mov dl,start
			int 10h 
		
	    endm endlst

         clear macro 
			mov ah, 0       ; AH=0 is the function for setting video mode
			mov al, 3       ; AL=3 sets 80x25 text mode (standard text mode)
			int 10h         ; BIOS interrupt for video services
		 endm clear 

		 clearline macro
		    mov ah,9
			mov bh,0
			mov al, 0h
			mov cx,79
			mov bl,SCREEN_ATTR
			int 10h
		endm cearline
        ;------------------------------------------------------------------------------------------------;

MAIN 	PROC FAR
     	MOV AX , @DATA
      	MOV DS , AX  

      	clear


		;---------------------------------------Screen Coloring------------------------------------------------
            MOV AH, 09h          ; Set text background and foreground color
			MOV AL, ' '          ; Space character to clear the screen
			MOV BL, SCREEN_ATTR  ; Set text attribute
			MOV CX, 2000h        ; Clear entire screen (80x25)
			MOV BH, 0            ; Page number (0 for default)
			INT 10h
         
      	;----------------------------------move the cursor to the middle of the screen and print user name turn--------------------;
      	Printusername:  
      	 ;----------------------set the start of printing---------
			mov ah,2
			mov dl,start_X_ofusername
			mov dh, start_Y_ofusername
			int 10h  
			 
			showmes user       
			
      	
      	printmainmenu:
      	
			mov ah,2
			mov dl,start_X_ofmain
			mov dh,start_Y_ofmain
			int 10h    
        ;----------------------------------Printing the Main Menu-----------------------------------------;

			showmes startchatmes
			endlst start_X_ofmain  ;endl to a certian x-coordinate
			endlst start_X_ofmain
			showmes startgamemes
			endlst start_X_ofmain
			endlst start_X_ofmain
			showmes exitmes  
        ;------------------------------------------------------------------------------------------------;   
        
        ;----------------------------------Printing the statusbarline-----------------------------------------;
		    mov ah,2
			mov dl,start_X_ofstatusbar
			mov dh,start_Y_ofstatusbar
		    int 10h  

			mov ah,9
			mov bh,0
			mov al,2dh
			mov cx,80
			mov bl,DASHEDLINEATTR
			int 10h
        ;------------------------------------------------------------------------------------------------;  

        ; this loop is like while(true) until the user press esc to exit the program
		again:      
			;------------------------move the cursor to the pressed key message location ---------------------------------;

			mov ah,2
			mov dl,start_X_ofpress
			mov dh,start_Y_ofpress
			int 10h   
			

			MOV AH, 00H         ;get the pressed key scan code and put it in ah        
			INT 16H  
            
			mov user_lastaction,ah  ; save the pressed key scan code in user_lastaction variable

			clearline
  
			cmp user_lastaction,01h          ; compare with ESC scan code  
			jnz checkF1
			showmes closeprogram
			jmp exit
			
			checkF1:
			cmp user_lastaction,3bh         ; compare with F1 scan code 
			jnz checkF2 
			showmes f1 
			jmp finish 
			
			checkF2:
			cmp user_lastaction,3ch        ; compare with F2 scan code 
			jnz not_pressed
			showmes f2 
			jmp finish 
			
			not_pressed:      ; esc,f1,f2 not pressed
			showmes none   
			
			finish: 
			
			jmp again  ;jmp to again listen again for the next instruction
        
        exit:

        MOV AH, 4CH         ; Function to exit program
        INT 21H             ; Call DOS interrupt to exit

MAIN ENDP
END MAIN



