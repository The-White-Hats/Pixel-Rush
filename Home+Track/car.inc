my_isr PROC
                 in                  al, 60H                                       ; put the scan code of the pressed or unpressed

                 cmp                 al, 3eh                                        ; pressing the f4 key
                 jz                  midKill

                 
    ;              cmp                al, 19h                                       ; pressing the 'p' key for first player power up
    ;              jnz                check_user2
    ;              mov                processed_power_holder, 1
    ;              mov                al, user1_stored_powerup
    ;              mov                processed_power, al
    ;              mov                user1_stored_powerup, 0
    ;              call               activatePowerUp
    ;              jmp                dontKill
                 
    ; check_user2:            
    ;              cmp                 al, 10h                                       ; pressing the 'q' key for second player power up
    ;              jnz                continue_whatever
    ;              mov                processed_power_holder, 2
    ;              mov                al, user2_stored_powerup
    ;              mov                processed_power, al
    ;              mov                user2_stored_powerup, 0
    ;              call               activatePowerUp
    ;              jmp                dontKill

    continue_whatever:
                 lea                 si, user1_dir_arr
	             lea                 di, user1_dir_bools
	             call                CheckDir

	             lea                 si, user2_dir_arr
	             lea                 di, user2_dir_bools
	             call                CheckDir

                 jmp                 dontKill

    midKill:     
                 mov                 al, 1H
                 mov                 killSignal, al                                ; Call DOS interrupt to exit

    dontKill:    
                 mov                 al, 20h                                       ; The non specific EOI (End Of Interrupt)
                 out                 20h, al
                 iret
my_isr endp


    ;-----------------------

inputFile PROC

    ; input file for first user
                 push dx
                 openFile
                 jc                  error

                 readFile            BUFF_SIZE, car1
                 jc                  error

                 closeFile
                 pop dx

    ; input file for second user
                 openFile
                 jc                  error

                 readFile            BUFF_SIZE, car2
                 jc                  error

                 closeFile

                 ret

    error:       
                 showMsg             errorMsg

                 ret

inputFile ENDP

    ;-----------------------

; put current_dir in dh, current_frame in dl
update_frame PROC

                 mov                 bl, 1
                 add_to_frame
                 cmp                 bl, dh
                 jz                  add_one

                 mov                 bl, 2
                 add_to_frame
                 cmp                 bl, dh
                 jz                  add_one

                 mov                 bl, 5
                 add_to_frame
                 cmp                 bl, dh
                 jz                  add_one

                 mov                 bl, 3
                 add_to_frame
                 cmp                 bl, dh
                 jz                  sub_one

                 mov                 bl, 6
                 add_to_frame
                 cmp                 bl, dh
                 jz                  sub_one

                 mov                 bl, 7
                 add_to_frame
                 cmp                 bl, dh
                 jz                  sub_one

                 mov                 bl, dl
                 jmp                 done_frame

    add_one:     
                 mov                 bl, 1
                 add_to_frame
                 jmp                 done_frame

    sub_one:     
                 mov                 bl, 7
                 add_to_frame

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
                sub     di, (SCREEN_WIDTH * CAR_OFFSET) + CAR_OFFSET

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
; ShowInGamePowerUps PROC
;     mov ax, saveInGamePowerupsCounter
;     cmp loadInGamePowerupsCounter, ax
;     jz ShowInGamePowerUps_end2
;     cmp showPowerupsCounter, 0
;     jnz ShowInGamePowerUps_end
;     mov generatedPowerupSource, 1
; 	lea si, inGamePowerups
;     add si, loadInGamePowerupsCounter
; 	call DrawPower
; 	add loadInGamePowerupsCounter, 4 ;! Go to next saved power up
;     add loadInGamePowerupsClrsCounter, 1 ;! Go to next saved power up color
;     mov showPowerupsCounter, POWERUP_SHOW_DELAY
;     ShowInGamePowerUps_end:
;         dec showPowerupsCounter
;     ShowInGamePowerUps_end2:
;         ret

; ShowInGamePowerUps ENDP

;     ;-----------------------    

; activatePowerUp PROC
;     ; check for power up type
;         cmp processed_power, 0  
;         jz activatePowerUp_end
;         cmp processed_power, 1
;         jz activatePowerUp_speedUp
;         cmp processed_power, 2
;         jz activatePowerUp_speedDown
;         cmp processed_power, 3
;         jz activatePowerUp_dropObs
;         cmp processed_power, 4
;         jz activatePowerUp_passObs
;         jmp activatePowerUp_end

;     activatePowerUp_speedUp:
;         call SpeedUp
;         jmp activatePowerUp_end
;     activatePowerUp_speedDown:
;         call SpeedDown
;         jmp activatePowerUp_end
;     activatePowerUp_dropObs:
;         call DropObs
;         jmp activatePowerUp_end
;     activatePowerUp_passObs:
;         call PassObs
;         jmp activatePowerUp_end
;     activatePowerUp_end:
;         mov processed_power, 0
;         mov processed_power_holder, 0
;         ret
; activatePowerUp ENDP
;     ;-----------------------
; SpeedUp PROC
;         ret
; SpeedUp ENDP
;     ;-----------------------
; SpeedDown PROC
;         ret
; SpeedDown ENDP
;     ;-----------------------
; DropObs PROC
;     pusha
;     ; move to current location of last saved obstacle generated by a power up
;     lea si, obstacles
;     add si, saveObstaclesCounter
;     add si, savePowerObstaclesCounter

;     ; check for the power up uesr
;     cmp processed_power_holder, 1
;     jz DropObs_user1
;     cmp processed_power_holder, 2
;     jz DropObs_mid_user2
;     jmp DropObs_mid
;     DropObs_user1:
;         ; check the current orientation of the car to drop the obstacle opposite to direction of motion
;         cmp current_dir, 2
;         jl DropObs_down1
;         cmp current_dir, 4
;         jl DropObs_left1
;         cmp current_dir, 6
;         jl DropObs_up1
;         cmp current_dir, 8
;         jl DropObs_mid_right1

;         DropObs_down1:
;             mov bx, user1_posX  
;             mov [si], bx                              ;! Xobs = Xcar
;             add si, 2
;             mov bx, user1_posY
;             mov [si], bx
;             mov bx, CAR_HEIGHT/2
;             add [si], bx
;             mov bx, OBSTACLE_LENGTH
;             add [si], bx                              ;! Yobs = Ycar + (CarHeight/2) + ObstacleLength
;             sub si, 2
;             call DrawObstacle
;             add savePowerObstaclesCounter, 4
;             jmp DropObs_end
        
;         DropObs_left1: 
;             mov bx, user1_posX
;             mov [si], bx
;             mov bx, CAR_HEIGHT/2
;             sub [si], bx
;             mov bx, OBSTACLE_LENGTH
;             sub [si], bx                              ;! Xobs = Xcar - (CarHeight/2) - ObstacleLength
;             add si, 2
;             mov bx, user1_posY
;             mov [si], bx                              ;! Yobs = Ycar      
;             sub si, 2
;             call DrawObstacle
;             add savePowerObstaclesCounter, 4
;             jmp DropObs_end

;             ;? JMP splitters
;             DropObs_mid:
;                 jmp DropObs_end

;             DropObs_mid_right1:
;                 jmp DropObs_right1
            
;             DropObs_mid_user2:
;                 jmp DropObs_user2
        
;         DropObs_up1:
;             mov bx, user1_posX
;             mov [si], bx                             ;! Xobs = Xcar
;             add si, 2
;             mov bx, user1_posY
;             mov [si], bx
;             mov bx, CAR_HEIGHT/2
;             sub [si], bx
;             mov bx, OBSTACLE_LENGTH
;             sub [si], bx                            ;! Yobs = Ycar - (CarHeight/2) - ObstacleLength
;             sub si, 2
;             call DrawObstacle
;             add savePowerObstaclesCounter, 4
;             jmp DropObs_end

;         DropObs_right1:
;             mov bx, user1_posX
;             mov [si], bx
;             mov bx, CAR_HEIGHT/2
;             add [si], bx
;             mov bx, OBSTACLE_LENGTH
;             add [si], bx                           ;! Xobs = Xcar + (CarHeight/2) + ObstacleLength
;             add si, 2
;             mov bx, user1_posY
;             mov [si], bx                           ;! Yobs = Ycar
;             sub si, 2
;             call DrawObstacle
;             add savePowerObstaclesCounter, 4
;             jmp DropObs_end

    
;         jmp DropObs_end
;     DropObs_user2:
        
;         jmp DropObs_end
;     DropObs_end:
;         popa
;         ret
; DropObs ENDP
;     ;-----------------------
; PassObs PROC
;         pusha
;         cmp processed_power_holder, 1
;         jz PassObs_user1
;         cmp processed_power_holder, 2
;         jz PassObs_user2
;         jmp PassObs_end
;     PassObs_user1:
;         ;! now the user1_obstacles_passed is 0, it must be checked in the collision logic to pass this user through one obstacle
;         ;! and then it must be set to 1 again
;         dec user1_obstacles_passed  
;         jmp PassObs_end
;     PassObs_user2:
;         dec user2_obstacles_passed
;         jmp PassObs_end
;     PassObs_end:
;         popa
;         ret
; PassObs ENDP


    ;-----------------------

; At start
; Set bl to the color to check collision with
; bh => 1 to draw, 0 to detect collision
; put current_frame in al

; At end
; bh => 1 if collision happened, 0 if not
handleCar PROC
    ; set the car frame
                 mov                 cx, BUFF_SIZE
                 mov                 ah, 0
                 mul                 cx
                 add                 si, ax

                 mov                 cx, CAR_HEIGHT               ; number of lines to draw
    car_outerLoop:   

                 mov                 dx, CAR_WIDTH                ; number of pixels to draw
    car_innerLoop:   
                 cmp                 byte ptr [si], 250
                 jz                  car_skip_pixel
  
                 cmp bh, 0
                 jz car_collision
                    
                 ; draw car
                 movsb
                 jmp                 car_continue

    car_collision:
                 ; check for collision 
                 cmp                 byte ptr es:[di], bl
                 jnz                 car_skip_pixel
            
                 ; collision happened
                 mov bh, 1
                 ret

    car_skip_pixel:        
                 inc                 si
                 inc                 di

    car_continue:    
                 dec                 dx
                 jnz                 car_innerLoop

                 add                 di, SCREEN_WIDTH - CAR_WIDTH   ; move to next line 
                 loop                car_outerLoop

                 mov                 bh, 0
                 ret
handleCar ENDP;* Check for the first line in the finish line and get its color
;* if it is not default then one of the users win the match 
;* then get this different color and compare it with the color of the user's  car to decide which one win the match 
CheckForGameFinish PROC
    pusha
    cmp user1score ,100
    jnz user2wins

    mov IsUser1Win,1
    jmp display_winnerscreen

    user2wins:
    cmp user2score ,100
    jnz CheckForGameFinish_skip
    mov IsUser2Win,1
    
    display_winnerscreen:
    ;mov killSignal,1
    
    CheckForGameFinish_skip:
    popa
    ret
CheckForGameFinish ENDP
