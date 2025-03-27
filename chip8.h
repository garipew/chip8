#ifndef CHIP8_H

#include <stddef.h>


#define STACK_ADDR 0xEA0
#define STACK_SIZE 48


typedef struct chip{
	unsigned char memory[0xFFF + 1]; // memory range: [0 - 0xFFF]
	unsigned char V[0xF + 1]; // data registers, range: [V0 - V15]
	unsigned short I; // address register
	unsigned short pc;
	unsigned char sp;
	unsigned char delay_timer;
	unsigned char sound_timer;
	unsigned char keys[16];
	unsigned char display[64*32];
	unsigned short opcode;
} Chip;


Chip* initialize();
void load_game(Chip*, FILE*);
