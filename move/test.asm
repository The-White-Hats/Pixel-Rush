; A simple program to read keyboard input and display it on the screen
.model small
.stack 100h
.data
    flag1 db 'n'
    flag2 db '0'
.code
main proc
    mov ax, @data ; set data segment
    mov ds, ax


    ; read keyboard input and display it on the screen
    readkey:
        
        in al, 60h ; read scan code

        cmp al, 1h
        jz kill
        ; check for the up arrow key
        cmp al, 48h ; test the most significant bit
        jnz not_pressed
        mov flag1, 'y'
        not_pressed:
        cmp al, 0c8h
        jnz not_release
        mov flag1, 'n'
        not_release:

        ; check for the w key
        cmp al, 11h ; test the most significant bit
        jnz not_pressed2
        mov flag2, '1'
        not_pressed2:
        cmp al, 91h
        jnz not_release2
        mov flag2, '0'
        not_release2:

        
        mov ah, 2h
        mov dl, flag1
        int 21H
        mov dl, flag2
        int 21H

        jmp readkey ; loop until the program is terminated

        kill:
        mov ah, 4CH
        int 21H
    main endp
    end main
