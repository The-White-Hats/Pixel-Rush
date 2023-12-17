include macros.inc
.286
public GameMain
public IsUser1Win,IsUser2Win
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
          
	    
          putWallPaperM 14, 10, 0, 0
	    
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

          ;call WinnerScreen
     ; return to DOS
          MOV AH,4CH
          INT 21H
GameMain ENDP
     
END 