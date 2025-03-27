#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define push(c, data) \
	c->memory[STACK_ADDR+c->sp++] = data


#define pop(c) \
	c->memory[STACK_ADDR+(--c->sp)]


#define X(c) \
	(c->opcode&0xF00)>>8


#define Y(c) \
	(c->opcode&0xF0)>>4


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
	if(chip->sp+1 < STACK_SIZE){
		push(chip, (chip->pc >> 8) & 0xFF);
		push(chip, chip->pc & 0xFF);
	} else{
		printf("Stack Overflow.\n");
		return;
	}
	int c;
	chip->pc = 0x200;
	while((c = getc(stream)) != EOF){
		if(chip->pc >= STACK_ADDR){
			ungetc(c, stream);
			break;
		}
		chip->memory[chip->pc++] = (unsigned char)c;
	}
	chip->pc = pop(chip) | pop(chip)<<8;
}


void run_cycle(Chip* chip){
	chip->opcode = chip->memory[chip->pc++]<<8 | chip->memory[chip->pc++];	
	switch(chip->opcode & 0xF000){
		case FUNCTION_CALL: 
			if(chip->sp+1 < STACK_SIZE){
				push(chip, (chip->pc>>8)&0xFF);
				push(chip, chip->pc&0xFF);
				chip->pc = chip->opcode&0xFFF;
			} else{
				printf("Stack overflow\n");
				return;
			}
			break;
		case JP: chip->pc = chip->opcode & 0xFFF; break; 
		case JEQ: if(chip->V[X(chip)] == (chip->opcode&0xFF)){ chip->pc+=2; } break;
		case JNE: if(chip->V[X(chip)] != (chip->opcode&0xFF)){ chip->pc+=2; } break;
		case JEI: if(chip->V[X(chip)] == chip->V[Y(chip)]){ chip->pc+=2; } break;
		case JNI: if(chip->V[X(chip)] != chip->V[Y(chip)]){ chip->pc+=2; } break;
		case MOV: chip->V[X(chip)] = chip->opcode&0xFF; printf("%x\n",chip->V[X(chip)]); break;
		case ADDI: chip->V[X(chip)] += chip->opcode&0xFF;printf("%x\n",chip->V[X(chip)]); break;
		default: // operation not found
			printf("Operation 0x%4x not supported.\n", chip->opcode);
	}
}


void cleanup(Chip* chip){
	if(chip){
		free(chip);
	}
}
