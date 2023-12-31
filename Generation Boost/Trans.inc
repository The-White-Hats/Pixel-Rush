;description
InitailizeConnection PROC
    pusha
    ;Set Divisor Latch Access Bit
    mov dx,3fbh 			; Line Control Register
    mov al,10000000b		;Set Divisor Latch Access Bit
    out dx,al			;Out it
    ;Set LSB byte of the Baud Rate Divisor Latch register.
    mov dx,3f8h			
    mov al,0ch			
    out dx,al
    ;Set MSB byte of the Baud Rate Divisor Latch register.
    mov dx,3f9h
    mov al,00h
    out dx,al
    ;Set port configuration
    mov dx,3fbh
    mov al,00011011b
    ; 0:Access to Receiver buffer, Transmitter buffer
    ; 0:Set Break disabled
    ; 011:Even Parity
    ; 0:One Stop Bit
    ; 11:8bits
    out dx,al

    popa
    ret
InitailizeConnection ENDP

;? Transmit the track data 
;? 1-Directions 
;? 2-Closed Area 
;? 3-PrevStart 
;? 4-TotalParts
;description
CheckSend PROC
    pusha
    notallowed:
        mov dx , 3FDH		; Line Status Register
        In al , dx 			;Read Line Status
        AND al , 00100000b
    jz notallowed
    popa
    ret
CheckSend ENDP

CheckRecieve PROC
    pusha
    waitfordata:
            mov dx , 3FDH		; Line Status Register
            in al , dx 
            AND al , 1
    jz waitfordata
    popa
    ret
CheckRecieve ENDP

TrackTransmission_Send PROC
    pusha
        ;*********************Sending a Flag To tell the other user that we will send the trackdata
        call checksend
        mov al, ';' ;? Flag
        mov dx, 03f8h
        out dx, al
        ;******************************************************************
        ;***********************Send the direction of the line that close  the start****************************
        call checksend
        mov al, isvertical
        mov dx, 03f8h
        out dx, al

        call checksend
        mov al, isup_right
        mov dx, 03f8h
        out dx, al
        ;******************************************************************

        ;**************************Send Car Initailization***************************   
        lea si , START_CAR_X
        mov cx,8
        sendCarStart:
           call checksend
           mov al, [si]
           mov dx, 03f8h
           out dx, al
           inc si
        loop sendCarStart
    

        call checksend
        mov al, user1_current_frame
        mov dx, 03f8h
        out dx, al

        call checksend
        mov al, user2_current_frame
        mov dx, 03f8h
        out dx, al

        ;******************************************************************

        ;**************************Send Total Parts***************************         
        call checksend
        mov ax, TotalParts
        mov dx, 03f8h
        out dx, al

        ;***********************************Directions******************
        mov cx,TotalParts
        shl cx,2 ;? mul cx by 4
        lea si,Directions
        sendDirectionData:

        call checksend
        mov al, [si] ;? sending a word 
        mov dx, 03f8h
        out dx, al
        inc si
        
        loop sendDirectionData

        ;************************************Closed Area*****************
        mov cx,TotalParts
        shl cx,3 ;? mul cx by 8
        lea si,ClosedArea
        sendClosedAreaData:
        
        call checksend
        mov al, [si]
        mov dx, 03f8h
        out dx, al
        inc si
        
        loop sendClosedAreaData

        ;***************************************Prev Start**************
        
        mov cx,TotalParts
        inc cx
        shl cx,2 ;? mul cx by 4
        lea si,PrevStart

        sendPrevStartData:

        call checksend
        mov al, [si]
        mov dx, 03f8h
        out dx, al
        inc si
        
        loop sendPrevStartData

    popa
    ret
TrackTransmission_Send ENDP

TrackTransmission_Receive PROC
    pusha

        waitfortrack:
         call CheckRecieve
            mov dx , 03F8H
            in al , dx 
            cmp al, ';'
        jnz waitfortrack
        ;******************************************************************
        ;***********************Send the direction of the line that close  the start****************************
         call CheckRecieve
        
        mov dx , 03F8H
        in al , dx 
        mov  isvertical,al

         call CheckRecieve
        mov dx , 03F8H
        in al , dx 
        mov isup_right,al 
         ;******************************************************************

        ;**************************Send Car Initailization***************************   
        lea si , START_CAR_X
        mov cx,8
        receiveCarStart:
          call CheckRecieve
            mov dx , 03F8H
            in al , dx 
           mov [si], al
           inc si
        loop receiveCarStart
    

        call CheckRecieve
        mov dx , 03F8H
        in al , dx 
        mov  user1_current_frame,al

        call CheckRecieve
        mov dx , 03F8H
        in al , dx 
        mov user2_current_frame,al 
        ;******************************************************************
        ;**************************Send Total Parts***************************         
        mov ax,0
        call CheckRecieve
        mov dx , 03F8H
        in al , dx 
        mov TotalParts , ax
        ;***********************************Directions******************
        mov cx,TotalParts
        shl cx,2 ;? mul cx by 4
        lea si,Directions
        RecieveDirectionData:

        call CheckRecieve
        mov dx , 03F8H
        in al , dx 
        mov [si] , al
        inc si
        
        loop RecieveDirectionData
        ;************************************Closed Area*****************

    
        mov cx,TotalParts
        shl cx,3 ;? mul cx by 8
        lea si,ClosedArea

        RecieveClosedAreaData:

        call CheckRecieve
        mov dx , 03F8H
        in al , dx 
        mov [si] , al
        inc si
        
        loop RecieveClosedAreaData
        ;***************************************Prev Start**************

        mov cx,TotalParts
        inc cx
        shl cx,2 ;? mul cx by 4
        lea si,PrevStart

        RecievePrevStartData:

        call CheckRecieve
        mov dx , 03F8H
        in al , dx 
        mov [si] , al
        inc si
        
        loop RecievePrevStartData
    popa
    ret
TrackTransmission_Receive ENDP

ObsPowerTransmission_Send PROC
    pusha
        ;*********************Sending a Flag To tell the other user that we will send the trackdata
        call checksend
        mov al, ';' ;? Flag
        mov dx, 03f8h
        out dx, al
       ;******************************************************************
    ;************************Send obstacles****************************** 
      mov cx,MAX_PARTS*2
      shl cx,1
      lea si,obstacles
        sendObstacles:
            call checksend
            mov al, [si]
            mov dx, 03f8h
            out dx, al
            inc si
        loop sendObstacles
    ;******************************************************************
    ;************************Send compilationPowerups****************************** 
        mov cx,MAX_PARTS*2
        shl cx,1
        lea si,compilationPowerups
        sendcompilationPowerups:
            call checksend
            mov al, [si]
            mov dx, 03f8h
            out dx, al
            inc si
        loop sendcompilationPowerups
    ;******************************************************************
    ;************************Send compilationPowerupsClrs****************************** 
         mov cx,MAX_PARTS
         shl cx,1
        lea si,compilationPowerupsClrs
        sendcompilationPowerupsClrs:
            call checksend
            mov al, [si]
            mov dx, 03f8h
            out dx, al
            inc si
        loop sendcompilationPowerupsClrs

    ;******************************************************************
    ;************************Send inGamePowerups****************************** 
         mov cx,MAX_PARTS*2
         shl cx,1
        lea si,inGamePowerups
        sendinGamePowerups:
            call checksend
            mov al, [si]
            mov dx, 03f8h
            out dx, al
            inc si
        loop sendinGamePowerups
    ;******************************************************************
    ;************************Send inGamePowerupsClrs****************************** 
         mov cx,MAX_PARTS
            shl cx,1
        lea si,inGamePowerupsClrs
        sendinGamePowerupsClrs:
            call checksend
            mov al, [si]
            mov dx, 03f8h
            out dx, al
            inc si
        loop sendinGamePowerupsClrs
    ;******************************************************************
    ;************************Send saveObstaclesCounter****************************** 
         
    ;******************************************************************
    ;************************Send savePowerObstaclesCounter****************************** 
   
    ;******************************************************************
    ;************************Send saveCompilationPowerupsCounter****************************** 
   
    ;******************************************************************
    ;************************Send saveCompilationPowerupsClrsCounter****************************** 
   
    ;******************************************************************
    ;************************Send saveInGamePowerupsCounter****************************** 
   
    ;******************************************************************
    ;************************Send saveInGamePowerupsClrsCounter****************************** 
    mov cx,12
    lea si,saveObstaclesCounter
    sendsaveCounters:
        call checksend
        mov al, [si]
        mov dx, 03f8h
        out dx, al
        inc si
    loop sendsaveCounters
        
    popa
    ret
ObsPowerTransmission_Send ENDP

ObsPowerTransmission_Receive PROC
    pusha
        waitforobs:
        call CheckRecieve
            mov dx , 03F8H
            in al , dx 
            cmp al, ';'
        jnz waitforobs
      ;******************************************************************
    ;************************Receive obstacles****************************** 
      mov cx,MAX_PARTS*2
      shl cx,1
      lea si,obstacles
      receiveObstcales:
        call CheckRecieve
        mov dx , 03F8H
        in al , dx 
        mov [si] , al
        inc si
      loop receiveObstcales  
    ;******************************************************************
    ;************************Receive compilationPowerups****************************** 
        mov cx,MAX_PARTS*2
        shl cx,1
        lea si,compilationPowerups
        receivecompilationPowerups:
         call CheckRecieve
        mov dx , 03F8H
        in al , dx
        mov [si] , al
        inc si
        loop receivecompilationPowerups
    ;******************************************************************
    ;************************Receive compilationPowerupsClrs****************************** 
         mov cx,MAX_PARTS
         shl cx,1
        lea si,compilationPowerupsClrs
        receivecompilationPowerupsClrs:
         call CheckRecieve
        mov dx , 03F8H
        in al , dx
        mov [si] , al
        inc si
        loop receivecompilationPowerupsClrs

    ;******************************************************************
    ;************************Receive inGamePowerups****************************** 
         mov cx,MAX_PARTS*2
         shl cx,1
        lea si,inGamePowerups
        receiveinGamePowerups:
         call CheckRecieve
        mov dx , 03F8H
        in al , dx
        mov [si] , al
        inc si
        loop receiveinGamePowerups
    ;******************************************************************
    ;************************Receive inGamePowerupsClrs****************************** 
         mov cx,MAX_PARTS
            shl cx,1
        lea si,inGamePowerupsClrs
        receiveinGamePowerupsClrs:
         call CheckRecieve
        mov dx , 03F8H
        in al , dx
        mov [si] , al
        inc si
        loop receiveinGamePowerupsClrs
    ;******************************************************************
    ;************************Receive saveObstaclesCounter****************************** 
         
    ;******************************************************************
    ;************************Receive savePowerObstaclesCounter****************************** 
   
    ;******************************************************************
    ;************************Receive saveCompilationPowerupsCounter****************************** 
   
    ;******************************************************************
    ;************************Receive saveCompilationPowerupsClrsCounter****************************** 
   
    ;******************************************************************
    ;************************Receive saveInGamePowerupsCounter****************************** 
   
    ;******************************************************************
    ;************************Receive saveInGamePowerupsClrsCounter****************************** 
    mov cx,12
    lea si,saveObstaclesCounter
    receivesaveCounters:
        call CheckRecieve
        mov dx , 03F8H
        in al , dx
        mov [si] , al
        inc si
    loop receivesaveCounters
   
    popa
    ret
ObsPowerTransmission_Receive ENDP