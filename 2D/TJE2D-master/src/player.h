#pragma once

#include "utils.h"
#include "image.h""

enum Orientation {
	FRONT, RIGHT, LEFT, BACK
};

class Player {
public:

	Player();

	Vector2 position = Vector2(50, 50);
	Vector2 initpos = Vector2(50,50);
	Image sprite;
	Orientation facing = FRONT;
	
	int size = 16;
	bool is_moving = false;

	int fires_count;
	int num_fight_elements;

};

