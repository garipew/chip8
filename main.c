#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#ifdef DEBUG
	#define debug_fn(c)\
		for(int v = 0; v < 16; v+=4){\
			printf("V%d: %x ", v, c->V[v]);\
			printf("V%d: %x ", v+1, c->V[v+1]);\
			printf("V%d: %x ", v+2, c->V[v+2]);\
			printf("V%d: %x\n", v+3, c->V[v+3]);\
		}\
		for(int mem = 0; mem < 4096; mem+=256){\
			printf("%x ", c->memory[mem]);	\
			printf("%x ", c->memory[mem+1]);	\
			printf("%x ", c->memory[mem+2]);	\
			printf("%x ", c->memory[mem+3]);	\
			printf("%x ", c->memory[mem+4]);	\
			printf("%x ", c->memory[mem+5]);	\
			printf("%x ", c->memory[mem+6]);	\
			printf("%x ", c->memory[mem+7]);	\
			printf("%x ", c->memory[mem+8]);	\
			printf("%x ", c->memory[mem+9]);	\
			printf("%x ", c->memory[mem+10]);	\
			printf("%x ", c->memory[mem+11]);	\
			printf("%x ", c->memory[mem+12]);	\
			printf("%x ", c->memory[mem+13]);	\
			printf("%x ", c->memory[mem+14]);	\
			printf("%x\n", c->memory[mem+15]);	\
		}\
		printf("pc: %x sp: %x, opcode: %x\n", c->pc, c->sp, c->opcode)
#else
	#define debug_fn(c)\
		do{\
			}while(0)
#endif


int main(int argc, char** argv){
	if(argc < 2){
		printf("Usage: %s <rom-path>\n", argv[0]);
		return 1;
	}
	FILE* f = fopen(argv[1], "rb");
	if(!f){
		printf("%s: Unable to open %s\n", argv[0], argv[1]);
		return 1;
	}
	Chip* chip = initialize();
	if(!chip){
		printf("%s: Unexpected error\n", argv[0]);
		fclose(f);
		return 1;
	}
		
	load_game(chip, f);
	for(int i = 0; i < 3; i++){
		run_cycle(chip);
	}
	debug_fn(chip);
	fclose(f);
	cleanup(chip);
	return 0;
}
