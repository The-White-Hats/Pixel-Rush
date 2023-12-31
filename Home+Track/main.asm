include macros.inc
.286
public GameMain
public IsUser1Win,IsUser2Win,user1score,user2score
extrn user1name:BYTE
extrn user2name:BYTE
.MODEL SMALL
.STACK 128
.DATA
          include data.inc
          include logoData.inc

.CODE
          include           generate.inc
          include           car.inc
          include           draw.inc

GameMain PROC FAR
     ; initialize memory
          mov               ax , @DATA
          mov               ds , ax
          call              GenerateTrackDirections

     ; clear the screen
          clear

     ; override int 9h
          overrideInt9

     ; Remove Blinking from the screen
          rmBlink
		
     ; draw on screen using video memory
          mov               ax, 0A000h
          mov               es, ax

     ; Screen Coloring
          
	    
          putWallPaperM 7, 5, 0, 0
	     
          paintAreaM 0,MAX_Y+2,320,1,GOLD

          paintAreaM 0,MAX_Y+3,320,200-MAX_Y-3,BLACK

		call DrawScoreBoard
					    
     ; Input files
          inputAllFiles

     ; Generate Track
         
          call              DrawTrack
          call              FinalLine


     ; draw intial position of the player's cars
          intialCarPosition

     ; Main Loop
          carMainLoop
          
          ;  pressF4Please:
          ; MOV AH, 00H         ;get the pressed key scan code and put it in ah        
		; INT 16H
          ; cmp ah, 3EH         ;if the pressed key is F4 then jump to the label pressF4Please  
          ; jnz pressF4Please
         call ResetAllTrackData
         ret
GameMain ENDP
     
END 