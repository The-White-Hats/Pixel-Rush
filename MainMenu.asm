
; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt
         include mymacro.inc
		.MODEL SMALL
		.STACK 64
		.DATA       
		firstuser db 'first user : $'
		seconduser db 'second user : $'
		startchatmes db 'To start chatting press F1$'
		startgamemes db 'To start the game press F2$'
		exitmes db 'To end the program press ESC$' 
		esc db 'Exit Pressed$'
		f1 db 'F1 Pressed$'    
		f2 db 'F2 Pressed$' 
		none db 'I Dont Know$' 
		start db 28      
     	.CODE
MAIN 	PROC FAR
     	MOV AX , @DATA
      	MOV DS , AX 
      	;----------------------------------move the cursor to the middle of the screen--------------------;
      	mov ah,2
      	mov dl,start
      	mov dh,9
      	int 10h    
        ;----------------------------------Printing the Main Menu-----------------------------------------;

      	showmes startchatmes
      	endlst start  ;endl to a certian x-coordinate
      	endlst start
      	showmes startgamemes
      	endlst start
      	endlst start
      	showmes exitmes  
      	endlst start 
        endlst start  
        ;------------------------------------------------------------------------------------------------;   
        
        ; this loop is like while(true) until the user press esc to exit the program
        again :      
        
        
        MOV AH, 00H         ;get the pressed key scan code and put it in ah        
        INT 16H  
               
          
        cmp ah,01h          ; compare with ESC scan code  
        jnz checkF1
        showmes esc
        jmp exit
           
        checkF1:
        cmp ah,3bh
        jnz checkF2 
        showmes f1 
        jmp finish 
        
        checkF2:
        cmp ah,3ch
        jnz else
        showmes f2 
        jmp finish 
        
        else:      ; esc,f1,f2 not pressed
        showmes none   
        
        finish:  
        jmp again  ;jmp to again listen again for the next instruction
        
        exit:
HLT
MAIN ENDP
END MAIN



