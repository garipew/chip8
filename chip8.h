#ifndef CHIP8_H

#include <stdio.h>


#define STACK_ADDR 0xEA0
#define STACK_SIZE 48

#define FUNCTION_CALL 0x2000
#define CALC 0x8000
#define KEYS 0xE000
#define JP 0x1000
#define JEQ 0x3000
#define JNE 0x4000
#define JEI 0x5000
#define JNI 0x9000
#define JP_OFFSET 0xB000
#define MOV 0x6000
#define ADDI 0x7000
#define SET_I 0xA000
#define RAND 0xC000
#define DRAW 0xD000
#define GET_DELAY 0x07
#define GET_KEY 0x0A
#define SET_DELAY 0x15
#define SET_SOUND 0x18
#define ADD_ADDR 0x1E
#define SET_FONT 0x29
#define WRITE_VX 0x33
#define DUMP 0x55
#define LOAD 0x65


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
void run_cycle(Chip*);
void cleanup(Chip*);
#endif
