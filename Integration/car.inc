
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

; Set bx => 1 to draw the car - 0 to clear it
drawCar PROC
                 mov                 si, offset car

    ; set the car frame
                 mov                 cx, BUFF_SIZE
                 mov                 al, current_frame
                 mov                 ah, 0
                 mul                 cx
                 add                 si, ax

                 mov                 al, BACK_GROUND

                 mov                 cx, CAR_HEIGHT                                ; number of lines to draw
                 cmp                 bx, 0                                         ; 0 -> clear , 1 -> draw
                 jnz                 outerLoop
                 add                 cx, CAR_OFFSET*2
                 sub                 di, SCREEN_WIDTH * CAR_OFFSET + CAR_OFFSET
    outerLoop:   

                 mov                 dx, CAR_WIDTH                                 ; number of pixels to draw
                 cmp                 bx, 0                                         ; 0 -> clear , 1 -> draw
                 jnz                 innerLoop
                 add                 dx, CAR_OFFSET*2
    innerLoop:   
                 cmp                 byte ptr [si], 250
                 jz                  skip_pixel

                 cmp                 bx, 0                                         ; 0 -> clear , 1 -> draw
                 jz                  clear_car

    draw_car:    
                 movsb
                 jmp                 continue
              
    clear_car:   
                 stosb
                 jmp                 continue

    skip_pixel:        
                 inc                 si
                 inc                 di

    continue:    
                 dec                 dx
                 jnz                 innerLoop

                 add                 di, SCREEN_WIDTH - CAR_WIDTH                  ; move to next line
                 cmp                 bx, 0                                         ; 0 -> clear , 1 -> draw
                 jnz                 done_car
                 sub                 di, CAR_OFFSET*2
    done_car:    
                 loop                outerLoop

                 ret

drawCar ENDP