#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


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
