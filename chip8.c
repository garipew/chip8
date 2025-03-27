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


void dump(Chip* c, int limit){
	for(int i = 0; i <= limit; i++){
		c->V[i] = c->memory[c->I+i];
	}
}


void load(Chip* c, int limit){
	for(int i = 0; i <= limit; i++){
		c->V[i] = c->memory[c->I+i];
	}
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


void run_cycle(Chip* c){
	c->opcode = c->memory[c->pc++]<<8 | c->memory[c->pc++];	
	switch(c->opcode & 0xF000){
		case FUNCTION_CALL: 
			if(c->sp+1 < STACK_SIZE){
				push(c, (c->pc>>8)&0xFF);
				push(c, c->pc&0xFF);
				c->pc = c->opcode&0xFFF;
			} else{
				printf("Stack overflow\n");
				return;
			}
			break;
		case CALC: 
			switch(c->opcode&0xF){							
				case 0x0:c->V[X(c)] = c->V[Y(c)];break;							
				case 0x1:c->V[X(c)] |= c->V[Y(c)];break;							
				case 0x2: c->V[X(c)] &= c->V[Y(c)]; break;
				case 0x3: c->V[X(c)] ^= c->V[Y(c)]; break;
				case 0x4: push(c, c->V[X(c)]); c->V[X(c)] += c->V[Y(c)];
					c->V[0xF] = pop(c) > c->V[X(c)]; break;
				case 0x5: c->V[0xF] = c->V[X(c)] >= c->V[Y(c)];
					c->V[X(c)] -= c->V[Y(c)]; break;
				case 0x6: c->V[0xF] = c->V[X(c)]&0x1; c->V[X(c)] >>= 1; break;
				case 0x7: c->V[0xF] = c->V[Y(c)] >= c->V[X(c)];
					c->V[X(c)] = c->V[Y(c)] - c->V[X(c)]; break;
				case 0xE: c->V[0xF] = (c->V[X(c)]>>7)&0x1; c->V[X(c)]<<=1; break;
				default: printf("opcode: 0x%4x not found", c->opcode); return;
			}
		case KEYS: 
			switch(c->opcode&0xFF){ 
				case 0x9E: if(c->keys[X(c)]&0xF){ c->pc+=2; } break;
				case 0xA1: if(!(c->keys[X(c)]&0xF)){ c->pc+=2; } break;
				default: printf("opcode: 0x%4x not found", c->opcode); return;
			}
			break;
		case 0x0:
			switch(c->opcode&0xF){
		/*clear*/	case 0x0: printf("Clear screen\n"); break;
		/*return*/	case 0xE: c->pc = pop(c) | pop(c)<<8; break;
				default: printf("opcode: 0x%4x not found", c->opcode); return;
			}
			break;
		case 0xF000: 
			switch(c->opcode&0xFF){	
				case GET_DELAY: c->V[X(c)] = c->delay_timer; break;
				case GET_KEY:printf("Key capture"); break;
				case SET_DELAY: c->delay_timer = c->V[X(c)]; break;
				case SET_SOUND: c->sound_timer = c->V[X(c)]; break;
				case ADD_ADDR: c->I += c->V[X(c)]; break;	
				case SET_FONT: c->I = c->memory[c->V[X(c)]&0xF]; break;
				case WRITE_VX: c->memory[c->I] = c->V[X(c)]%10;
					c->memory[c->I+1] = ((c->V[X(c)]/10)%10);
					c->memory[c->I+2] = ((c->V[X(c)]/100)%10); break;
				case DUMP: dump(c, X(c)); break;
				case LOAD: load(c, X(c)); break;
				default: printf("opcode: 0x%4x not found", c->opcode); return;
			}
			break;
		case JP: c->pc = c->opcode & 0xFFF; break; 
		case JEQ: if(c->V[X(c)] == (c->opcode&0xFF)){ c->pc+=2; } break;
		case JNE: if(c->V[X(c)] != (c->opcode&0xFF)){ c->pc+=2; } break;
		case JEI: if(c->V[X(c)] == c->V[Y(c)]){ c->pc+=2; } break;
		case JNI: if(c->V[X(c)] != c->V[Y(c)]){ c->pc+=2; } break;
		case JP_OFFSET: c->pc = (c->opcode&0xFFF) + c->V[0]; break;
		case MOV: c->V[X(c)] = c->opcode&0xFF; break;
		case ADDI: c->V[X(c)] += c->opcode&0xFF; break;
		case SET_I: c->I = c->opcode&0xFFF; break;
		case RAND: c->V[X(c)] = rand() & (c->opcode&0xFF); break;
		case DRAW: printf("Drawn"); break;
		default: printf("opcode: 0x%4x not found", c->opcode); return;
	}
}


void cleanup(Chip* chip){
	if(chip){
		free(chip);
	}
}
