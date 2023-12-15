
my_isr PROC
                 in                  al, 60H                                       ; put the scan code of the pressed or unpressed

                 cmp                 al, 1h                                        ; pressing the esc key
                 jz                  midKill

                 lea                 si, user1_dir_arr
                 lea                 di, user1_dir_bools
                 call                CheckDir

                 jmp                 dontKill

    midKill:     
                 mov                 al, 0ffH
                 mov                 killSignal, al                                ; Call DOS interrupt to exit

    dontKill:    
                 mov                 al, 20h                                       ; The non specific EOI (End Of Interrupt)
                 out                 20h, al
                 iret
my_isr endp

    ;-----------------------

inputFile PROC

    ; Open file
                 openFile
                 jc                  error

    ; Read file
                 readFile            BUFF_SIZE, car
                 jc                  error

    ; Close file
                 closeFile

                 ret

    error:       
                 showMsg             errorMsg

                 ret

inputFile ENDP

    ;-----------------------


update_frame PROC

                 mov                 bl, 1
                 add_to_frame
                 cmp                 bl, current_dir
                 jz                  add_one

                 mov                 bl, 2
                 add_to_frame
                 cmp                 bl, current_dir
                 jz                  add_one

                 mov                 bl, 5
                 add_to_frame
                 cmp                 bl, current_dir
                 jz                  add_one

                 mov                 bl, 3
                 add_to_frame
                 cmp                 bl, current_dir
                 jz                  sub_one

                 mov                 bl, 6
                 add_to_frame
                 cmp                 bl, current_dir
                 jz                  sub_one

                 mov                 bl, 7
                 add_to_frame
                 cmp                 bl, current_dir
                 jz                  sub_one

                 jmp                 done_frame

    add_one:     
                 mov                 bl, 1
                 add_to_frame
                 mov                 current_frame, bl
                 jmp                 done_frame

    sub_one:     
                 mov                 bl, 7
                 add_to_frame
                 mov                 current_frame, bl

    done_frame:  
                 ret

update_frame ENDP

    ;-----------------------

CheckDir PROC
    ; move user_dir_arr offset into si
    ; move user_dir_bools offset into di
                 mov                 cx, 0H
                 xor                 ah, ah
    check_user:  
    ; check for the key arrow key
                 cmp                 al, [si]
                 jnz                 notPressed
                 mov                 byte ptr [di], 1H
    notPressed:  
                 mov                 bl, [si]
                 add                 bl, 80H
                 cmp                 al, bl
                 jnz                 notReleased
                 mov                 byte ptr [di], 0H
    notReleased: 

                 inc                 si
                 add                 di, 2h
                 inc                 cx
                 cmp                 cx, 4H
                 jnz                 check_user
                 ret
CheckDir ENDP

    ;-----------------------

; Set bx => 1 to save the background - 0 to draw it
setBackground PROC
                ; set source => screen - distination => current_background
                sub     di, (SCREEN_WIDTH * CAR_OFFSET) + CAR_OFFSET
                mov     si, offset current_background

                mov     cx, CAR_HEIGHT + CAR_OFFSET*2                ; number of lines
    background_outerLoop:   
                mov     dx, CAR_WIDTH + CAR_OFFSET*2                 ; number of pixels
    background_innerLoop:   
                cmp     byte ptr [si], 250
                jz      background_skip_pixel
                
                cmp     bx, 1
                jz      background_save
    
    background_draw:
                movsb
                jmp     background_continue

    background_save:
                mov     ax, es:[di]
                mov     [si], ax

    background_skip_pixel:        
                inc     si
                inc     di

    background_continue:    
                dec     dx
                jnz     background_innerLoop

                add     di, SCREEN_WIDTH - CAR_WIDTH - (CAR_OFFSET*2) ; move to next line    
                loop    background_outerLoop

                ret

setBackground ENDP
    
    ;-----------------------

drawCar PROC
                 setStartPixel user1_posX, user1_posY
                 mov                 si, offset car

    ; set the car frame
                 mov                 cx, BUFF_SIZE
                 mov                 al, current_frame
                 mov                 ah, 0
                 mul                 cx
                 add                 si, ax

                 mov                 cx, CAR_HEIGHT                                ; number of lines to draw
    car_outerLoop:   

                 mov                 dx, CAR_WIDTH                                 ; number of pixels to draw
    car_innerLoop:   
                 cmp                 byte ptr [si], 250
                 jz                  car_skip_pixel
  
                 movsb
                 jmp                 car_continue

    car_skip_pixel:        
                 inc                 si
                 inc                 di

    car_continue:    
                 dec                 dx
                 jnz                 car_innerLoop

                 add                 di, SCREEN_WIDTH - CAR_WIDTH                  ; move to next line 
                 loop                car_outerLoop

                 ret

drawCar ENDP