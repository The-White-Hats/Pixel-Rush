

start_Y_ofpress equ 29 
start_X_ofpress equ 5

user_lastaction db ?

closeprogram db 'Program Closed                    $'
closeprogramsize equ $-closeprogram-1  
f1 db 'You sent a chat invitation to                    $' 
f1size equ $-f1-1   
f2 db 'You sent a game invitation to                     $' 
f2size equ $-f2-1   
sentToMe db 'Got a message ...                    $'
sentToMeSize equ $-sentToMe-1
agreed db "agreed ...                       $"
agreedSize equ $-agreed-1
none db 'Please enter One of the shown keys$' 
nonesize equ $-none-1    