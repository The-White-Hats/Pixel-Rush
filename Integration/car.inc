
my_isr PROC
                 in                  al, 60H                                       ; put the scan code of the pressed or unpressed

                 cmp                 al, 1h                                        ; pressing the esc key
                 jz                  midKill

                 
                 cmp                 al, 19h                                       ; pressing the 'p' key for first player power up
                 jnz                check_user2
                 mov                processed_power_holder, 1
                 mov al, user1_stored_powerup
                 mov processed_power, al
                 mov user1_stored_powerup, 0
                 call activatePowerUp
                 jmp dontKill
                 
    check_user2:            
                 cmp                 al, 10h                                       ; pressing the 'q' key for second player power up
                 jnz                continue_whatever
                 mov                processed_power_holder, 2
                 mov al, user2_stored_powerup
                 mov processed_power, al
                 mov user2_stored_powerup, 0
                 call activatePowerUp
                 jmp dontKill
                                                    
    continue_whatever:
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

activatePowerUp PROC
    ; check for power up type
        cmp processed_power, 0  
        jz activatePowerUp_end
        cmp processed_power, 1
        jz activatePowerUp_speedUp
        cmp processed_power, 2
        jz activatePowerUp_speedDown
        cmp processed_power, 3
        jz activatePowerUp_dropObs
        cmp processed_power, 4
        jz activatePowerUp_passObs
        jmp activatePowerUp_end

    activatePowerUp_speedUp:
        call SpeedUp
        jmp activatePowerUp_end
    activatePowerUp_speedDown:
        call SpeedDown
        jmp activatePowerUp_end
    activatePowerUp_dropObs:
        call DropObs
        jmp activatePowerUp_end
    activatePowerUp_passObs:
        call PassObs
        jmp activatePowerUp_end
    activatePowerUp_end:
        mov processed_power, 0
        mov processed_power_holder, 0
        ret
activatePowerUp ENDP

SpeedUp PROC
        ret
SpeedUp ENDP

SpeedDown PROC
        ret
SpeedDown ENDP

DropObs PROC
    pusha
    ; move to current location of last saved obstacle generated by a power up
    lea si, obstacles
    add si, saveObstaclesCounter
    add si, savePowerObstaclesCounter

    ; check for the power up uesr
    cmp processed_power_holder, 1
    jz DropObs_user1
    cmp processed_power_holder, 2
    jz DropObs_mid_user2
    jmp DropObs_mid
    DropObs_user1:
        ; check the current orientation of the car to drop the obstacle opposite to direction of motion
        cmp current_dir, 2
        jl DropObs_down1
        cmp current_dir, 4
        jl DropObs_left1
        cmp current_dir, 6
        jl DropObs_up1
        cmp current_dir, 8
        jl DropObs_mid_right1

        DropObs_down1:
            mov bx, user1_posX  
            mov [si], bx                              ;! Xobs = Xcar
            add si, 2
            mov bx, user1_posY
            mov [si], bx
            mov bx, CAR_HEIGHT/2
            add [si], bx
            mov bx, OBSTACLE_LENGTH
            add [si], bx                              ;! Yobs = Ycar + (CarHeight/2) + ObstacleLength
            sub si, 2
            call DrawObstacle
            add savePowerObstaclesCounter, 4
            jmp DropObs_end
        
        DropObs_left1: 
            mov bx, user1_posX
            mov [si], bx
            mov bx, CAR_HEIGHT/2
            sub [si], bx
            mov bx, OBSTACLE_LENGTH
            sub [si], bx                              ;! Xobs = Xcar - (CarHeight/2) - ObstacleLength
            add si, 2
            mov bx, user1_posY
            mov [si], bx                              ;! Yobs = Ycar      
            sub si, 2
            call DrawObstacle
            add savePowerObstaclesCounter, 4
            jmp DropObs_end

            ;? JMP splitters
            DropObs_mid:
                jmp DropObs_end

            DropObs_mid_right1:
                jmp DropObs_right1
            
            DropObs_mid_user2:
                jmp DropObs_user2
        
        DropObs_up1:
            mov bx, user1_posX
            mov [si], bx                             ;! Xobs = Xcar
            add si, 2
            mov bx, user1_posY
            mov [si], bx
            mov bx, CAR_HEIGHT/2
            sub [si], bx
            mov bx, OBSTACLE_LENGTH
            sub [si], bx                            ;! Yobs = Ycar - (CarHeight/2) - ObstacleLength
            sub si, 2
            call DrawObstacle
            add savePowerObstaclesCounter, 4
            jmp DropObs_end

        DropObs_right1:
            mov bx, user1_posX
            mov [si], bx
            mov bx, CAR_HEIGHT/2
            add [si], bx
            mov bx, OBSTACLE_LENGTH
            add [si], bx                           ;! Xobs = Xcar + (CarHeight/2) + ObstacleLength
            add si, 2
            mov bx, user1_posY
            mov [si], bx                           ;! Yobs = Ycar
            sub si, 2
            call DrawObstacle
            add savePowerObstaclesCounter, 4
            jmp DropObs_end

    
        jmp DropObs_end
    DropObs_user2:
        
        jmp DropObs_end
    DropObs_end:
        popa
        ret
DropObs ENDP

PassObs PROC
        cmp processed_power_holder, 1
        jz PassObs_user1
        cmp processed_power_holder, 2
        jz PassObs_user2
        jmp PassObs_end
    PassObs_user1:
        ;! now the user1_obstacles_passed is 0, it must be checked in the collision logic to pass this user through one obstacle
        ;! and then it must be set to 1 again
        dec user1_obstacles_passed  
        jmp PassObs_end
    PassObs_user2:
        dec user2_obstacles_passed
        jmp PassObs_end
    PassObs_end:
        ret
PassObs ENDP

