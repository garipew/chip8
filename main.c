#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


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
	fclose(f);
	cleanup(chip);
	return 0;
}
