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
ShowInGamePowerUps PROC
    mov ax, saveInGamePowerupsCounter
    cmp loadInGamePowerupsCounter, ax
    jz ShowInGamePowerUps_end2
    cmp showPowerupsCounter, 0
    jnz ShowInGamePowerUps_end
    mov generatedPowerupSource, 1
	lea si, inGamePowerups
    add si, loadInGamePowerupsCounter
	call DrawPower
	add loadInGamePowerupsCounter, 4 ;! Go to next saved power up
    add loadInGamePowerupsClrsCounter, 1 ;! Go to next saved power up color
    mov showPowerupsCounter, POWERUP_SHOW_DELAY
    ShowInGamePowerUps_end:
        dec showPowerupsCounter
    ShowInGamePowerUps_end2:
        ret

ShowInGamePowerUps ENDP
    ;-----------------------    
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
    ;-----------------------
SpeedUp PROC
        ret
SpeedUp ENDP
    ;-----------------------
SpeedDown PROC
        ret
SpeedDown ENDP
    ;-----------------------
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
    ;-----------------------
PassObs PROC
        pusha
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
        popa
        ret
PassObs ENDP

;* Check for the first line in the finish line and get its color
;* if it is not default then one of the users win the match 
;* then get this different color and compare it with the color of the user's  car to decide which one win the match 
CheckForGameFinish PROC
    pusha
    mov si,END_LINE_WIDTH
    mov cx,CHECK_WIN_START_X
    mov dx,CHECK_WIN_START_Y
    check_loop:
        mov bh,0
        mov ah,0dh
        int 10h
        cmp al,BLACK
        jz Nowinners
        cmp al,WHITE
        jz Nowinners
        jmp lets_make_celebration
        Nowinners:

        ; mov al,PURPLE
        ; mov ah,0ch
        ; int 10h

        ;* move on the line according to the last drawn part if vertical(0,1)  dec start_x
        ;* if horizontal(2,3) inc start_y
        cmp CurrentCase,1
        jg CheckForGameFinish_case1
        dec cx
        jmp CheckForGameFinish_skip
        CheckForGameFinish_case1:
        inc dx
        CheckForGameFinish_skip:
        dec si
        cmp si,0
    jnz check_loop
    popa
    ret
    lets_make_celebration:
    ;* we need to compare the color of the user's car with the color in al
    cmp user1car_color ,al
    jnz user2wins

    mov IsUser1Win,1
    jmp display_winnerscreen

    user2wins:
    mov IsUser2Win,1
    
    display_winnerscreen:
    ;call WinnerScreen
    mov killSignal,1h
    popa
    ret
CheckForGameFinish ENDP

; WinnerScreen PROC
;     pusha
;     lea si ,user1name
;     mov cx,0
;     cmp [si],'$'
;     jz emptyname
;     getuser1namesize:
;     inc cx
;     cmp [si],'$'
;     inc si
;     jnz getuser1namesize
;     emptyname:
;     mov username1Size,cx

;     lea si ,user2name
;     mov cx,0
;     cmp [si],'$'
;     jz emptyname2
;     getuser2namesize:
;     inc cx
;     cmp [si],'$'
;     inc si
;     jnz getuser2namesize
;     emptyname2:
;     mov username2Size,cx

;     ;clear

;     showColoredMsg2 WinnerMessagePart1, 2bh, part1size, 30, 15 ;* part1
;     cmp IsUser1Win,1
;     jnz user2isthewinner
;     showColoredMsg2 user1name, 2bh, username1Size, 30, 15 ;* username
;     jmp WinnerScreen_part2
;     user2isthewinner:
;     showColoredMsg2 user2name, 2bh, username2Size, 30, 15 ;* username
;     WinnerScreen_part2:
;     showColoredMsg2 WinnerMessagePart2, 2bh, part2size, 35, 17 ;* part2
;     popa
;     ret
; WinnerScreen ENDP    
