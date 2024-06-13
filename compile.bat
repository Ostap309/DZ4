nasm -g -fwin32 f_lib_windows.asm -o f_lib_windows.o
gcc -m32 -o prog.exe prog.c f_lib_windows.o