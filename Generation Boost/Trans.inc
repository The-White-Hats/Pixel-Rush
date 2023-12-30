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
        call checksend
        mov ax, TotalParts
        mov dx, 03f8h
        out dx, ax

        ;****************************************************************
        mov cx,TotalParts
        lea si,Directions
        sendDirectionData:

        call checksend
        mov ax, [si] ;? sending a word 
        mov dx, 03f8h
        out dx, ax
        add si,2
        
        loop sendDirectionData

        mov cx,TotalParts
        shl cx,2 ;? mul cx by 4
        lea si,ClosedArea

        sendClosedAreaData:
        
        call checksend
        mov ax, [si]
        mov dx, 03f8h
        out dx, ax
        add si,2
        
        loop sendClosedAreaData
        
        mov cx,TotalParts
        shl cx,2 ;? mul cx by 2
        lea si,PrevStart

        sendPrevStartData:

        call checksend
        mov ax, [si]
        mov dx, 03f8h
        out dx, ax
        add si,2
        
        loop sendPrevStartData

    popa
    ret
TrackTransmission_Send ENDP


TrackTransmission_Recieve PROC
    pusha

        waitfortrack:
         call CheckRecieve
        
            mov dx , 03F8H
            in al , dx 
            cmp al, ';'
        jnz waitfortrack

        call CheckRecieve
        mov dx , 03F8H
        in ax , dx 
        mov TotalParts , ax
        ;****************************************************************
        mov cx,TotalParts
        lea si,Directions
        RecieveDirectionData:

        call CheckRecieve
        mov dx , 03F8H
        in ax , dx 
        mov [si] , ax
        add si,2
        
        loop RecieveDirectionData
        
    
        mov cx,TotalParts
        shl cx,2 ;? mul cx by 4
        lea si,ClosedArea

        RecieveClosedAreaData:

        call CheckRecieve
        mov dx , 03F8H
        in ax , dx 
        mov [si] , ax
        add si,2
        
        loop RecieveClosedAreaData


        mov cx,TotalParts
        shl cx,2 ;? mul cx by 2
        lea si,PrevStart

        RecievePrevStartData:

        call CheckRecieve
        mov dx , 03F8H
        in ax , dx 
        mov [si] , ax
        add si,2
        
        loop RecievePrevStartData
    popa
    ret
TrackTransmission_Recieve ENDP
