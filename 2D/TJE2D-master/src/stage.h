#pragma once

#include "image.h"
#include "player.h"
#include "gameMap.h"
#include "camera.h"


class Stage {
public:

	Stage();

	Image font;
	Image minifont;
	Image minifont1;
	Color bgcolor;
	int tileWidth = 8;
	int tileHeight = 8;

	virtual void render(Image& fb);
	virtual void update(float delta);

};

class IntroStage : public Stage {
public:

	IntroStage();

	Image introImg;
	Image titol;
	Image tutorial;
	int page = 0;

	virtual void render(Image& fb);
	virtual void update(float delta);

};

class PlayStage : public Stage {
public:

	PlayStage();

	bool playable = true;
	float time;

	Image tileset;
	Image fons_fosc1;
	Image fons_fosc2;
	Image fons_fosc3;
	Image endimage;

	std::vector<Image> fons = { fons_fosc1, fons_fosc2, fons_fosc3 };
	float time_light;
	bool light_up;

	GameMap* map = nullptr;
	Player* player1 = nullptr;
	Camera camera;
	float init_time = 480 * 1.000f;
	float speed = 100;
	Vector2 target;

	virtual void render(Image& fb);
	virtual void update(float delta);

};

class LooseStage : public Stage {
public:

	LooseStage();

	Image font;
	Image minifont;
	Image fons_monster;

	virtual void render(Image& fb);
	virtual void update(float delta);

};

class WinStage : public Stage {
public:

	WinStage();

	Image font;
	Image minifont;

	virtual void render(Image& fb);
	virtual void update(float delta);

};


