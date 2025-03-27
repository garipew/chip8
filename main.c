#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>


#ifdef DEBUG
	#define debug_fn(c)\
		printf("opcode: %x\n", c->opcode);\
		for(int v = 0; v < 16; v+=4){\
			printf("V%x: %2x ", v, c->V[v]);\
			printf("V%x: %2x ", v+1, c->V[v+1]);\
			printf("V%x: %2x ", v+2, c->V[v+2]);\
			printf("V%x: %2x\n", v+3, c->V[v+3]);\
		}\
		printf("I: %4x\n", c->I);\
		for(int mem = 0; mem < 4096; mem+=256){\
			printf("%2x ", c->memory[mem]);	\
			printf("%2x ", c->memory[mem+1]);	\
			printf("%2x ", c->memory[mem+2]);	\
			printf("%2x ", c->memory[mem+3]);	\
			printf("%2x ", c->memory[mem+4]);	\
			printf("%2x ", c->memory[mem+5]);	\
			printf("%2x ", c->memory[mem+6]);	\
			printf("%2x ", c->memory[mem+7]);	\
			printf("%2x ", c->memory[mem+8]);	\
			printf("%2x ", c->memory[mem+9]);	\
			printf("%2x ", c->memory[mem+10]);	\
			printf("%2x ", c->memory[mem+11]);	\
			printf("%2x ", c->memory[mem+12]);	\
			printf("%2x ", c->memory[mem+13]);	\
			printf("%2x ", c->memory[mem+14]);	\
			printf("%2x\n", c->memory[mem+15]);	\
		}\
		printf("pc: %x, sp: %x\n", c->pc, c->sp)
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
	double period = 1/60.0;
	struct timespec start, end;
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
	clock_gettime(CLOCK_REALTIME, &start);
	/*
	struct timespec elapsed_time;
	elapsed_time.tv_sec = start.tv_sec;
	elapsed_time.tv_nsec = start.tv_nsec;
	int seconds = 0;
	int frame = 0;
	*/
	for(; ;){
		clock_gettime(CLOCK_REALTIME, &end);
		if(((end.tv_sec-start.tv_sec) + 1e-9*(double)(end.tv_nsec-start.tv_nsec)) < period){
			continue;
		}
		/*
		if(end.tv_sec > elapsed_time.tv_sec && end.tv_nsec >= elapsed_time.tv_nsec){
			printf("elapsed time: %ds\n", ++seconds);
			elapsed_time.tv_sec = end.tv_sec;
			elapsed_time.tv_nsec = end.tv_nsec;
			frame = 0;
		}
		printf("frame: %d\n", ++frame);
		*/
		start.tv_sec = end.tv_sec;
		start.tv_nsec = end.tv_nsec;
		run_cycle(chip);
		debug_fn(chip);
	}
	fclose(f);
	cleanup(chip);
	return 0;
}
