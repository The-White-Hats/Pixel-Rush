include macros.inc

.286
.MODEL SMALL
.STACK 128
.DATA
          include data.inc

.CODE
          include           generate.inc
          include           car.inc

MAIN PROC FAR
     ; initialize memory
          mov               ax , @DATA
          mov               ds , ax
    
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
          colorScreen       BACK_GROUND
					    
     ; Input files
          inputAllFiles

     ; Generate Track
          call              RandomStart
          mov               helper, 1
          call              GenerateTrackDirections
          call              Draw
          call              FinalLine

     ; draw intial position of the player's cars
          intialCarPosition

     ; Main Loop
          carMainLoop

     ; return to DOS
          mov               AH, 4ch
          int               21h
MAIN ENDP
     
END MAIN