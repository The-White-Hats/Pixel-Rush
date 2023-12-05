include macros.inc

.MODEL SMALL
.STACK 64

;-----------------------

.DATA

    BUFF_SIZE     equ 40*23

    CAR_HEIGHT    equ 40
    CAR_WIDTH     equ 23

    SCREEN_WIDTH  equ 320
    SCREEN_HEIGHT equ 200

    ;----------car movement----------;
    carFile       db  'car5.bin', 0
    
    car           db  BUFF_SIZE dup(?)
    errorMsg      db  "Something went wrong with files !!", 10, "$"

    ;-----------------------

.CODE

MAIN PROC FAR
              mov       ax, @DATA
              mov       ds, ax

              mov       ax,0A000h
              mov       es,ax

    ; clear and Set video mode
              clear

    ; Remove Blinking from the screen and allowing to use 16 colors as background
              rmBlink

              mov       bx, offset carFile                                                                  ; filename to open

    again:    
              push      bx

    ; get car image
              mov       dx, bx
              call      inputFile
            
    ; draw car
              call      drawCar

    ; wait for key press
              mov       ah, 0
              int       16h

              pop       bx

    ; get next car image
    back:     
              inc       bx
              cmp       byte ptr [bx], 0
              jnz       back
              inc       bx

    ; check if end of car images (restart)
              cmp       byte ptr [bx], '$'
              jnz       again

              mov       bx, offset carFile

              jmp       again

              mov       AH,4ch
              int       21h

MAIN ENDP

    ;-----------------------

inputFile PROC

    ; Open file
              openFile
              jc        error

    ; Read file
              readFile  BUFF_SIZE, car
              jc        error

    ; Close file
              closeFile

              ret

    error:    
              showMsg   errorMsg

              ret

inputFile ENDP

    ;-----------------------

drawCar PROC

    ; Draw background
              mov       di,0
              mov       cx,64000
              mov       al,00h

              rep       stosb

              mov       di, SCREEN_WIDTH*(SCREEN_HEIGHT/2 - CAR_HEIGHT/2) + SCREEN_WIDTH/2 - CAR_WIDTH/2    ; starting pixel of screen
              mov       si, offset car                                                                      ; starting byte of car

              mov       cx, CAR_HEIGHT                                                                      ; number of lines to draw

    outerLoop:

              mov       dx, CAR_WIDTH                                                                       ; number of pixels to draw

    innerLoop:

              cmp       byte ptr [si], 250
              jz        skip

              movsb                                                                                         ; copy byte from si to di (draw pixel)
              jmp       continue

    skip:     
              inc       si
              inc       di

    continue: 
              dec       dx

              jnz       innerLoop

              add       di, SCREEN_WIDTH - CAR_WIDTH                                                        ; move to next line

              loop      outerLoop

              ret

drawCar ENDP

    ;-----------------------

END MAIN