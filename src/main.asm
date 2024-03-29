include ./inc/macros.inc
.286
public GameMain
public IsUser1Win,IsUser2Win,user1score,user2score
extrn user1name:BYTE
extrn user2name:BYTE
public sender
.MODEL SMALL
.STACK 128
.DATA
          include ./inc/data.inc
          include ./inc/logoData.inc

.CODE
          include           ./inc/generate.inc
          include           ./inc/car.inc
          include           ./inc/draw.inc
          include           ./inc/Trans.inc

GameMain PROC FAR
     ; initialize memory
          mov               ax , @DATA
          mov               ds , ax

          call              InitailizeConnection


     ;?   check if this user if the sender if true make him generate and send the data 
     ;?   else make him recieve the data from the other user

          cmp sender,1
          jz GenerateTheTrack
          jmp RecieveTrack 

          GenerateTheTrack:
          call              GenerateTrackDirections
          call              GenerateAllObsPowers  
          call              TrackTransmission_Send
          call              ObsPowerTransmission_Send

          jmp LetsPlay

          RecieveTrack:

          call              TrackTransmission_Receive
          call              ObsPowerTransmission_Receive

          LetsPlay:

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
          
         call ResetAllTrackData
         ret
GameMain ENDP
     
END 