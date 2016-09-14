#pragma once
#include "common.h"
#include "game.h"


typedef struct {
    
	char* name;
	char* ctr;
	u32 adress;
	
} ListGame;

typedef struct {

	char* name;
	
}NameCarte;

NameCarte cartename;

u32 titlescreen();