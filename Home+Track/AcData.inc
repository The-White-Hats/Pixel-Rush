

start_Y_ofpress equ 28 
start_X_ofpress equ 5

user_lastaction db ?

closeprogram db 'Program Closed$'
closeprogramsize equ $-closeprogram-1  
f1 db 'Entering Chatting ...                    $' 
f1size equ $-f1-1   
f2 db 'Generating Tarck ...                    $' 
f2size equ $-f2-1   
none db 'Please enter One of the shown keys$' 
nonesize equ $-none-1    