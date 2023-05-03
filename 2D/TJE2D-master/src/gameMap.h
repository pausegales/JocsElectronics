#pragma once

#include "image.h"
#include "player.h"
#include "camera.h"

enum eCellType {
	FOSC = 196, TERRA = 98, PARET = 32, TOPPARET = 0, BOTTOMPARET = 40, FONT1 = 138, FONT2 = 139, FONT3 = 170, FONT4 = 171,
	PARET_L = 82, PARET_R = 80,
	PORTA1 = 55, PORTA2 = 56, PORTA3 = 87, PORTA4 = 88, PORTA5 = 59, PORTA6 = 60, PORTA7 = 91, PORTA8 = 92,

	TORCH_OFF = 328, FLAME_OFF = 296, FIRE1 = 386, FIRE2 = 418, MONSTER1 = 360, MONSTER2 = 361, MONSTER3 = 392, MONSTER4 = 393,
	COLLISION = 496, NO_COLLISION = 464, SPECIAL_COLLISION = 497,

	HOLE1 = 369, HOLE2 = 370, HOLE3 = 371, HOLE4 = 372, EMPTY = -1, HOLE6 = 374, HOLE7 = 375, HOLE8 = 376, HOLE9 = 377,
	PAINTED = 181,

	SWORD1_UP = 73, SWORD1_DOWN = 105, SWORD2_UP = 77, SWORD2_DOWN = 109,
};

enum eItemType {
	NOTHING, SWORD, POTION,
};


struct sCell {
	eCellType type;
	eItemType item;
};

struct sLayer {
	sCell* data;
};


class GameMap {

public:
	int width;
	int height;
	sCell* data;
	sLayer* layers;

	GameMap();
	GameMap(int w, int h);
	sCell& getCell(int x, int y);
	sCell& getCell(int x, int y, int l);
	void drawMap(Image* framebuffer, int layer, float time, Image tileset, Camera camera);

	void loadGameMap(const char* filename);
	bool isValid(Vector2 pos, int size);

};
