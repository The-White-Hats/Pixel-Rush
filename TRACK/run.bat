@echo off
masm /Zi /c %1.asm
link / %1.obj
%1.exe

