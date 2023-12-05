
CheckHorizontalTrack PROC  ;! Dependent on the START_X and START_Y and does not change them
	    mov bx,0
		mov cx, START_X
		mov dx, START_Y
        ;---------------------------check for bounadry conditions---------------------------------;
		    ;--------------------------------put the final position of dx and check with it---------------------------------;
	        cmp horizontalDirection ,1
				jnz down4
				sub cx,LINE_LENGTH ;dx
				jmp skip4
				down4:
				add cx,LINE_LENGTH ;dx
				skip4:

				cmp dx,MAX_Y
				jle Hcheck1
				jmp return1
            ;----------------------------------------------------------------------------------------------------------------;
			Hcheck1: 
				sub dx,LINE_WIDTH
				sub dx,BOUNDARY_WIDTH*2
				cmp dx,MIN_Y
				jge Hcheck2
				jmp return1

			Hcheck2:
				cmp cx,MAX_X ;dx
				jle Hcheck3
				jmp return1

			Hcheck3:
				cmp cx,MIN_X ;dx
				jge checkcolor1
				jmp return1

		checkcolor1:
		;-------------------------- horizontal line check = ---------------------------------;
        mov bx,0
        mov cx,START_X
		checkHorizontal1:
		    mov dx,START_Y


		    cmp bx,LINE_LENGTH
			jz checkVertical_pre1
			inc bx ; inc counter untill reach the length of the line

             ;-----------------------------update dx according to horizontalDirection -----------------------------------------;
			cmp horizontalDirection ,1
			jnz down5
			dec cx
			jmp skip5
			down5:
			inc cx
			skip5:
             ;----------------------------------------------------------------------------------------------------------------;
			
			mov ah,0dh
			int 10h
			cmp al,DefaultBackground  
            
		jz checkHorizontalparallel1 ;if not DefaultBackground return
		ret

		checkHorizontalparallel1:

			sub dx,LINE_WIDTH  
			sub dx,BOUNDARY_WIDTH*2 ;same y-axis but x-axis is increased by the width of the line and the boundary
            inc dx

			mov ah,0dh
			int 10h  
			cmp al,DefaultBackground
          
		jz checkHorizontal1  ;if not DefaultBackground return
		ret 
        
		;--------------------------vertical line check | | ---------------------------------;
		checkVertical_pre1:
		mov bx,0
        mov dx,START_Y
		checkVertical1:
            mov cx,LINE_WIDTH
            add cx,BOUNDARY_WIDTH*2
		    cmp bx,cx
			jz break1
			inc bx

            mov cx,START_X

			dec dx

			mov ah,0dh
			int 10h 
			cmp al,DefaultBackground
           
		jz checkVerticalparallel1
		ret

		checkVerticalparallel1:

			cmp horizontalDirection ,1
			jnz down6
			sub cx ,LINE_LENGTH
			jmp skip6
			down6:
			add cx ,LINE_LENGTH
			skip6:

			mov ah,0dh
			int 10h
			cmp al,DefaultBackground
		jz checkVertical1
         ret 
		 
		 return1:
		 ret
         break1:
		horizontalPart START_X, START_Y, horizontalDirection
        ret 
CheckHorizontalTrack ENDP

CheckVerticalTrack PROC ;! Dependent on the START_X and START_Y and does not change them
	    mov bx,0
		mov cx,START_X
		mov dx,START_Y
        ;---------------------------check for bounadry conditions---------------------------------;
		    ;--------------------------------put the final position of dx and check with it---------------------------------;
	        cmp verticaldirection ,0
				jnz down3
				sub dx,LINE_LENGTH
				jmp skip3
				down3:
				add dx,LINE_LENGTH
				skip3:

				cmp cx,MIN_X
				jge check1
				jmp return
            ;----------------------------------------------------------------------------------------------------------------;
			check1: 
				add cx,LINE_WIDTH
				add cx,BOUNDARY_WIDTH*2
				cmp cx,MAX_X
				jle check2
				jmp return

			check2:
				cmp dx,MIN_Y
				jge check3
				jmp return

			check3:
				cmp dx,MAX_Y
				jle checkcolor
				jmp return 

		checkcolor:
		;-------------------------- vertical line check | | ---------------------------------;
        mov bx,1
		mov dx,START_Y
		checkvertical:
		    mov cx,START_X

		    cmp bx,LINE_LENGTH
			jz checkhorizontal_pre
			inc bx ; inc counter untill reach the length of the line

             ;-----------------------------update dx according to verticaldirection -----------------------------------------;
			cmp verticaldirection ,0
			jnz down1
			dec dx 
			jmp skip1
			down1:
			inc dx
			skip1:
             ;----------------------------------------------------------------------------------------------------------------;
			
			mov ah,0dh
			int 10h
			cmp al,DefaultBackground
		jz checkverticalparallel ;if not DefaultBackground return
		ret

		checkverticalparallel:

			add cx,LINE_WIDTH  
			add cx,BOUNDARY_WIDTH*2 ;same y-axis but x-axis is increased by the width of the line and the boundary
            dec cx
			mov ah,0dh
			int 10h
			cmp al,DefaultBackground

		jz checkvertical  ;if not DefaultBackground return
		ret 
        
		;--------------------------horizontal line check = ---------------------------------;
		checkhorizontal_pre:
		mov bx,0
		mov cx,START_X
		checkhorizontal:

		    mov dx,START_Y
		    cmp bx,LINE_WIDTH
			jz break
			inc bx

			inc cx

			mov ah,0dh
			int 10
			cmp al,DefaultBackground

		jz checkhorizontalparallel
		ret

		checkhorizontalparallel:

			cmp verticaldirection ,0
			jnz down2
			sub dx ,LINE_LENGTH
            inc dx
			jmp skip2
			down2:
			add dx ,LINE_LENGTH
            dec dx
			skip2:

			mov ah,0dh
			int 10h
			cmp al,DefaultBackground
		jz checkhorizontal
         ret 
		 
		 return:
		 ret
         break:
		call GenerateVerticalTrack
        ret
CheckVerticalTrack ENDP
