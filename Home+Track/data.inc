
;------------------General---------------------;
    SCREEN_WIDTH    equ 320
    SCREEN_HEIGHT   equ 200

;---------------------Car-----------------------;
    CAR_HEIGHT      equ 11
    CAR_WIDTH       equ 11

    BUFF_SIZE       equ CAR_HEIGHT*CAR_WIDTH
    
    CAR_OFFSET      equ 0


    FILE_NAME_LEN   equ 9

    START_CAR_SHIFT equ 10
    ;------------Files-------------;
    carFile         db  'car0.bin', 0
                    db  'car1.bin', 0
                    db  'car2.bin', 0
                    db  'car3.bin', 0
                    db  'car4.bin', 0
                    db  'car5.bin', 0
                    db  'car6.bin', 0
                    db  'car7.bin', 0

    errorMsg        db  "Something went wrong with files !!", 10, "$"
    
    ;------------Car Directions---------;
    car1             db  8 dup (BUFF_SIZE dup(?))
                     dt  ?
    car2             db  8 dup (BUFF_SIZE dup(?))
                     dt  ?
    ;-----------Saved Background---------;
    user1_current_background  db ((CAR_HEIGHT+(2*CAR_OFFSET))*(CAR_WIDTH+(2*CAR_OFFSET))) dup(?)
                        dt  ?
    user2_current_background  db ((CAR_HEIGHT+(2*CAR_OFFSET))*(CAR_WIDTH+(2*CAR_OFFSET))) dup(?)
                        dt  ?

    ; start from 0 on up and move clockwise
    user1_current_frame   db  0
    user1_prev_frame      db  ?
    user1_current_dir     db  0
    user1_prev_dir        db  ?
    
    user1_posX      dw  ?
    user1_posY      dw  ?
    user1_dir_bools db  7 dup(0)                ; up, right, down, left
                    db  0                       ; margin
    user1_dir_arr   db  48h, 4dh, 50H, 4BH
    user1_prev_posX dw  ?
    user1_prev_posY dw  ?

    user2_posX      dw  30 
	user2_posY      dw  10
	user2_dir_bools db  7 dup(0)          	    ; up, right, down, left
	                dw  0                 	    ; margin
	user2_dir_arr   db  11h, 20h, 1fH, 1eH
	user2_prev_posX dw  0
	user2_prev_posY dw  0
                    db  0
    user2_current_frame   db  0
    user2_prev_frame      db  ?
    user2_current_dir     db  0
    user2_prev_dir        db  ?
                    db  0
    killSignal      db  0
                    dw  0
    origInt9Offset  dw  0
    origInt9Segment dw  0
                    dt  ?

;--------------------------------------------;

;------------------Generation----------------;
    ;*----------------------------------COLORS------------------------------------------------------;
         SCREEN_ATTR equ 0F0h ; BOUNDARY_COLOR2 background (high nibble) and DefaultBackground text (low nibble)
         DASHEDLINEATTR equ 0F0h ; 
         
		 BOUNDARY_COLOR1 equ LIGHT_ORANGE
		 BOUNDARY_COLOR2 equ EXTRA_LIGHT_GRAY

		 EndTrackColor1 db BLACK ;!Finish Line
		 EndTrackColor2 db WHITE ;!Finish Line

		
         DefaultBackground equ GREEN
         DASHESCOLOR equ LIGHT_GRAY
		 OBSTACLE_COLOR equ RED
		 POWERUP1_COLOR equ LIGHT_BLUE
		 POWERUP2_COLOR equ BLUE
		 POWERUP3_COLOR equ PURPLE
		 POWERUP4_COLOR equ YELLOW
		 SEPARATING_LINE_COLOR equ LIGHT_BLUE
         ;*----------------------------------Positions-------------------------------------------------;      
		 START_X dw 2
         START_Y dw 150
         START_CAR_X dw 2
         START_CAR_Y dw 150
         START_DIR db 0
         END_X dw ?
         END_Y dw ?
         ;*----------------------------------BOUNDARY-------------------------------------------------;      
         MIN_X equ 0
         MAX_X equ 319
         MIN_Y equ 0
         MAX_Y equ 150 
         ;*----------------------------------Dimensions-------------------------------------------------;
         LINE_WIDTH equ 25
         LINE_LENGTH equ 5
		 HORIZONTAL_LINE_LENGTH equ 26

		 END_LINE_WIDTH equ LINE_WIDTH ;!Finish Line
		 END_LINE_LENGTH equ 6 ;!Finish Line
		 END_LINE_DASHEDLINE_LENGTH equ 2 ;!Finish Line
         EndTrackWidthToggle equ 2

         BOUNDARY_WIDTH equ 1
         BOUNDARY_LENGTH equ 3
         DASHEDLINE_LENGTH equ 5
		 OBSTACLE_WIDTH equ 4
		 OBSTACLE_LENGTH equ 4
		 POWERUP_WIDTH equ 2
		 POWERUP_LENGTH equ 2
          ;*----------------------------------Variables-------------------------------------------------;
		 posx dw 0
		 posy dw 0
		 toggleboundarycolor db 0
		 toggletrackmiddle db 0
		 boundaryflag db  0
		 middletrackflag db 0
         verticaldirection db 0 ;! 0 up 1 down

         cornerFlag db 1  ;! 0 means no corner, 1 means corner
         cornerType db 1 ;! 1 means up, 0 means down
         horizontalDirection db 1 ;! 1 left 0 right
		 isvertical db 0 ;! 1 vertical ,0 horizontal
		 isup_right db 0 ;! 1 up or right 0 oppisite
        ;*----------------------------------Track Directions Generation Variables-------------------------------------------------;
        
        MAX_PARTS equ 50
        TIME equ 0
		WRONGTHRESHOLD equ 12

        starttime db ?

		prev_start_x dw ?
		prev_start_y dw ?

        last_success_x dw ?
        last_success_y dw ?
        
        x_max_new dw ?
        x_min_new dw ?

        x_max_old dw ?
        x_min_old dw ?

        y_max_new dw ?
        y_min_new dw ?

        y_max_old dw ?
        y_min_old dw ?
        

        Directions dw MAX_PARTS+1 dup(0)

        ClosedArea dw (MAX_PARTS+1)*4 dup(0) ;! Xmin, Xmax, Ymin, Ymax
        
		PrevStart  dw (MAX_PARTS+1)*2 dup(0) ;! START_X,START_Y

		PartWrongs dw (MAX_PARTS+1) dup(0) ;! number of wrongs for each part

		PartWrongsOffset dw 0

        SingleCheckFlag db 1 ;? check for single part in track

		TrackCheckFlag db 1 ;? check for all track parts with the new part

		TotalParts dw 0 ;? check for all track parts so far

		WrongCounter dw 0 ;? count number of consecutive wrong parts

		startoffsetdirection dw 0 
		startoffsetclosedarea dw 0 
		startoffsetprevstart dw 0 

        ;*----------------------------------Track Generation Variables------------------------------------------------
        CurrentCase dw 0 
        
        ;*----------------------------------Random Proc Variables------------------------------------------------
        seed DW 1234H       ; Initial seed value
		multiplier Dw 1d51H
		increment Dw 8c45H
		modulus DW 0F4bFh 
		random_part db 0 ; 4 bytes of memory to store the random seed
        
		
		
		;*------------------------------------------- CASES -------------------------------------------; 
		lastRandom db 0
        currentRandom db 0
		validationFlag db 0
		casse0 db 5,7,0
		casse1 db 8,10,1
		casse2 db 11,2,6
		casse3 db 9,3,4
		casse4 db 1,1,1
		casse5 db 2,2,2
		casse6 db 1,1,1
		casse7 db 3,3,3
		casse8 db 2,2,2
		casse9 db 0,0,0
		casse10 db 3,3,3
		casse11 db 0,0,0
		;*------------------------------------------- OBSTACLES AND POWERUPS -------------------------------------------;
		POWERUP_SHOW_DELAY equ 5fh
        
        latestPos db 0
		nowORthen db 0 ; 0 means now, 1 means then (POWERUPS)
		lastType db POWERUP1_COLOR
		latestType db POWERUP2_COLOR

		obstacles dw MAX_PARTS*2 dup(0) ;! START_X,START_Y (bottom left corner)
		compilationPowerups dw MAX_PARTS*2 dup(0)
        ;*------------------------------------------- USERS DATA -------------------------------------------;
		stringcolor db ?
		;user1name db 'Marwan$' 
		;user2name db 'Youssef$'
		scorename db 'Score: $'
        userscore db 0
        user1score db 0
		user2score db 0
		user1theme db LIGHT_BLUE
		user2theme db RED
        usernextpart db  1
        user1nextpart db 1
        user2nextpart db 1
        user_posX dw ?
        user_posY dw ? 
		;*--------------------------------------------------------------------------------------------------;
		
		compilationPowerupsClrs db MAX_PARTS dup(0) 

		inGamePowerups dw MAX_PARTS*2 dup(0) ;! START_X,START_Y (center, bottom)
		inGamePowerupsClrs db MAX_PARTS dup(0)


		saveObstaclesCounter dw 0 ;! offset to add for obstacles array 
        savePowerObstaclesCounter dw 0 ;! offset to add for obstacles array (MADE FROM POWERUPS)

		saveCompilationPowerupsCounter dw 0 ;! offset to add for compilationPowerups array
		saveCompilationPowerupsClrsCounter dw 0 ;! offset to add for compilationPowerupsClrs array
		loadCompilationPowerupsClrsCounter dw 0 ;! offset to add for compilationPowerupsClrs array
		saveInGamePowerupsCounter dw 0 ;! offset to add for inGamePowerups array (Insertion)
		saveInGamePowerupsClrsCounter dw 0 ;! offset to add for inGamePowerupsClrs array (Insertion)
        loadInGamePowerupsCounter dw 0 ;! offset to add for inGamePowerups array (Loading)
        loadInGamePowerupsClrsCounter dw 0 ;! offset to add for inGamePowerupsClrs array (Loading)


        generatedPowerupSource dw 0 ;! 0 means compilationPowerups, 1 means inGamePowerups

        showPowerupsCounter dw POWERUP_SHOW_DELAY

        ;*------------------------------------------- EQUIPPING POWERUPS -------------------------------------------;
        processed_power_holder db 0 ;! 1 means user1, 2 means user2
        processed_power db 0 ;! 0 means no powerup, 1 means speed up, 2 means slow down, 3 means drop obstacle, 4 means pass through obstacle

        ;! 0 means no powerup, 1 means speed up, 2 means slow down, 3 means drop obstacle, 4 means pass through obstacle
        user1_stored_powerup db 3
        user2_stored_powerup db 0 

        ;! stated in the doc that the car can pass through 1 obstacle (didn't mention limitid time)
        user1_obstacles_passed db 1
        user2_obstacles_passed db 1
        dt ?
        divider db 3H
		helper db 1H
		random db 0
        s db 0
;-----------------------------------;