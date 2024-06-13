all:
	nasm -g -f elf32 f_lib_linux.asm -o f_lib_linux.o
	gcc -m32 -static -o prog f_lib_linux.o prog.c
clean:
	rm *.o prog
