chip: chip8.o main.c
	gcc -o chip chip8.o main.c

debug: chip8.o main.c
	gcc -D DEBUG -o chip chip8.o main.c

chip8.o: chip8.c chip8.h
	gcc -c chip8.c

clean:
	rm -rf *.o chip
