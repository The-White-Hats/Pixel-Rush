serial1=directserial realport:com2
tasm home2
tasm main
tasm chat
tlink home2+main+chat
home2.exe
del *.EXE
DEL *.OBJ
DEL *.MAP
exit