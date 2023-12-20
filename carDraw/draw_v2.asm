include macros.inc

.MODEL SMALL
.STACK 64

;-----------------------

.DATA

    BUFF_SIZE     equ 85*120

    CAR_HEIGHT    equ 120
    CAR_WIDTH     equ 85

    SCREEN_WIDTH  equ 320
    SCREEN_HEIGHT equ 200

    ;----------car movement----------;
    carFile       db  'Car_B0.bin', 0
                  db  'Car_B1.bin', 0
                  db  'Car_B2.bin', 0
                  db  'Car_B3.bin', 0
                  db  'Car_B4.bin', 0
                  db  'Car_B5.bin', 0
                  db  'Car_B6.bin', 0
                  db  'Car_B7.bin', 0
                  db  'Car_B8.bin', 0
                  db  'Car_B9.bin', 0
                  db  'Car_B10.bin', 0
                  db  'Car_B11.bin', 0
                  db  'Car_B12.bin', 0
                  db  'Car_B13.bin', 0
                  db  'Car_B14.bin', 0
                  db  'Car_B15.bin', 0
                  db  '$'
    
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