#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define push(c, data) \
	c->memory[STACK_ADDR+c->sp++] = data;


#define pop(c) \
	c->memory[STACK_ADDR+(--c->sp)];


Chip* initialize(){
	Chip* c = malloc(sizeof(*c));
	if(!c){
		return c;
	}
	c->pc = 0x200;
	c->sp = 0;
	c->opcode = 0;
	c->I = 0;
	memset(c->keys, 0, sizeof(*c->keys)*16);
	memset(c->display, 0, sizeof(*c->display)*64*32);
	srand(time(NULL));
	return c;
}


void load_game(Chip* chip, FILE* stream){
	if(c->sp+1 < STACK_SIZE){
		push(c, (chip->pc >> 8) & 0xFF);
		push(c, chip->pc & 0xFF);
	} else{
		printf("Stack Overflow.\n");
		return;
	}
	for(chip->pc = 0x200; chip->pc < STACK_ADDR; chip->pc+=2){
		fgets(chip->memory + chip->pc, 2, stream);
	}
	chip->pc = pop(c) | pop(c)<<8;
}


void cleanup(Chip* chip){
	if(chip){
		free(chip);
	}
}
