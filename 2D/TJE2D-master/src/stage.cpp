#include "stage.h"
#include "input.h"
#include "game.h"


Stage::Stage()
{
	Color bgcolor(130, 80, 100);
}

void Stage::render(Image& fb)
{
}

void Stage::update(float delta)
{
}

IntroStage::IntroStage()
{
	introImg.loadTGA("data/intro.tga"); //load bitmap-font image
	titol.loadTGA("data/titol.tga"); //load bitmap-font image
	tutorial.loadTGA("data/tutorial.tga");

}

void IntroStage::render(Image& fb)
{
	int x = fb.width / 2;
	int y = fb.height / 2;
	fb.drawImage(introImg, 0, 0, fb.width, fb.height);
	if (page == 0) { fb.drawImage(titol, 0, 0, fb.width, fb.height); }
	if (page == 1) { fb.drawImage(tutorial, 0, 0, fb.width, fb.height); }
	if (page == 2) { Game::instance->changeState(1); }
	
}

void IntroStage::update(float delta)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key Z was pressed
	{
		page++;
	}
}

PlayStage::PlayStage()
{
	float time = getTime() * 0.001; // Time in sec
	init_time = time + init_time;

	font.loadTGA("data/bitmap-font-white.tga"); //load bitmap-font image
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
	minifont1.loadTGA("data/mini-font-red_bad-4x6.tga");

	tileset.loadTGA("data/dungeon.tga");
	fons_fosc1.loadTGA("data/fons1_forat.tga");
	fons_fosc2.loadTGA("data/fons2_forat.tga");
	fons_fosc3.loadTGA("data/fons3_forat.tga");
	endimage.loadTGA("data/intro.tga"); //load bitmap-font image
	//endimage.loadTGA("data/endimage.tga");

	std::vector<Image> fons = { fons_fosc1, fons_fosc2, fons_fosc3 };

	this->map = new GameMap(32,32);
	map->loadGameMap("data/mapajoc.tmj");
	this->player1 = new Player();
	player1->position = player1->initpos;

	//Game::instance->enableAudio(); //enable this line if you plan to add audio to your application
	//Game::instance->synth.playSample("data/music.wav",1,true);
}

void PlayStage::render(Image& fb)
{
	float time = getTime() * 0.001; // Time in sec
	if (init_time - time <= 0) { Game::instance->changeState(2); }
	for (int l = 1; l < 5; l++) {
		map->drawMap(&fb, l, time, tileset, camera);
	}
	//
	int time_int = ((int(time * 6)) % 4) * 14;
	fb.drawImage(player1->sprite, player1->position.x - camera.offset.x, player1->position.y - camera.offset.y, Area(player1->is_moving ? (time_int) : 0, player1->facing * 18, 14, 18));	//draws only a part of an image
	player1->is_moving = false;


	map->drawMap(&fb, 5, time, tileset, camera);
	map->drawMap(&fb, 6, time, tileset, camera);

	int wait_time;
	if (player1->fires_count == 1) { wait_time = 8; } else { wait_time = 4.5; }

	if (time - time_light > wait_time && player1->fires_count > 0) {
		player1->fires_count -= 1; time_light = time;

	}

	//Drawing the different types of dark top layers
	int posx = player1->position.x - camera.offset.x;
	int posy = player1->position.y - camera.offset.y;
	int x = posx - fb.width / 2;
	int y = posy - fb.height / 2;
	int count = player1->fires_count;
	Image fons;
	if (count == 0) { fons = fons_fosc1; }
	if (count == 1) { fons = fons_fosc2; }
	if (count >= 2) { fons = fons_fosc3; }
	fb.drawImage(fons, x, y, fb.width - 70, fb.height - 10, 350, 350);


	//Drawing decreasing time.
	if ((init_time - time) > 100) {
		fb.drawText(toString(init_time - time), 5, 10, minifont, 4, 6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)
	}
	if (100 > (init_time - time) && (init_time - time) > 0) {
		fb.drawText(toString(init_time - time), 5, 8, minifont1, 4, 6);
	}

	std::string myText = "Collected lights";
	std::string myString = std::to_string(player1->fires_count) + " " + myText;
	fb.drawText(myString, 80, 6, minifont, 4, 6);
	std::string myText1 = "Fighting elements";
	std::string myString1 = std::to_string(player1->num_fight_elements) + " " + myText1;
	fb.drawText(myString1, 80, 12, minifont, 4, 6);

}

void PlayStage::update(float delta)
{

	speed = 100;
	target = player1->position;

	//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key UP
	{
		target.y -= delta * speed;
		player1->facing = BACK;
		player1->is_moving = true;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key DOWN
	{
		target.y += delta * speed;
		player1->facing = FRONT;
		player1->is_moving = true;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key LEFT
	{
		target.x -= delta * speed;
		player1->facing = LEFT;
		player1->is_moving = true;

	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key RIGHT
	{
		target.x += delta * speed;
		player1->facing = RIGHT;
		player1->is_moving = true;
	}

	if (map->isValid(target, player1->size / 2)) {
		player1->position = target;
	}
	else if (map->isValid(Vector2(target.x, player1->position.y), player1->size / 2))
		player1->position = Vector2(target.x, player1->position.y);

	else if (map->isValid(Vector2(player1->position.x, target.y), player1->size / 2))
		player1->position = Vector2(player1->position.x, target.y);


	//example of 'was pressed'
	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key A was pressed
	{
		int cellX = ((player1->position.x + 16) / 2) / tileWidth - 1;
		int cellY = ((player1->position.y + 16) / 2) / tileHeight - 1;

		eCellType& type1 = map->getCell(cellX, cellY, 3).type;
		eCellType& type11 = map->getCell(cellX + 1, cellY + 1, 3).type;
		eCellType& type2 = map->getCell(cellX + 1, cellY + 1, 5).type;
		eCellType& type22 = map->getCell(cellX + 2, cellY + 2, 5).type;

		if ((((type2) == (PORTA5)) || ((type2) == PORTA6) || ((type1) == PORTA7) || ((type1) == PORTA8)) || (((type22) == (PORTA5)) || ((type22) == PORTA6) || ((type11) == PORTA7) || ((type11) == PORTA8))) {
			for (int i = cellX - 2; i < cellX + 2; i++) {
				for (int j = cellY - 3; j < cellY + 3; j++) {
					eCellType& type1 = map->getCell(i, j, 3).type;
					eCellType& type2 = map->getCell(i, j, 5).type;
					eCellType& type3 = map->getCell(i, j, 6).type;
					if (type2 == PORTA5) { type2 = PORTA1; }
					if (type2 == PORTA6) { type2 = PORTA2; }
					if (type1 == PORTA7) { type1 = PORTA3; }
					if (type1 == PORTA8) { type1 = PORTA4; }
					if (type3 == NO_COLLISION) { type3 = COLLISION; }
				}
			}
		}
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key A was pressed
	{
		int cellX = ((player1->position.x + 16) / 2) / tileWidth - 1;
		int cellY = ((player1->position.y + 16) / 2) / tileHeight - 1;

		eCellType& type1 = map->getCell(cellX, cellY, 4).type;
		eCellType& type2 = map->getCell(cellX + 1, cellY, 4).type;
		eCellType& type3 = map->getCell(cellX, cellY + 1, 4).type;
		eCellType& type4 = map->getCell(cellX + 1, cellY + 1, 4).type;
		//printf("%d %d %d %d", type1, type2, type3, type4);
		for (int i = cellX - 2; i < cellX + 2; i++) {
			for (int j = cellY - 3; j < cellY + 3; j++) {
				eCellType& type = map->getCell(i, j, 4).type;

				if ((type == MONSTER1) || (type == MONSTER1) || (type == MONSTER1) || (type == MONSTER1)){
					type = EMPTY;
					Game::instance->changeState(3);
				}				
			}
		}
	}


	
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{

		int cellX = ((player1->position.x + 16) / 2) / tileWidth - 1;
		int cellY = ((player1->position.y + 16) / 2) / tileHeight - 1;
		//printf("%d", cellX);

		eCellType& type1 = map->getCell(cellX, cellY, 3).type;
		eCellType& type11 = map->getCell(cellX + 1, cellY + 1, 3).type;
		eCellType& type2 = map->getCell(cellX + 1, cellY + 1, 5).type;
		eCellType& type22 = map->getCell(cellX + 2, cellY + 2, 5).type;

		if ((((type2) == (PORTA1)) || ((type2) == PORTA2) || ((type1) == PORTA3) || ((type1) == PORTA4)) || (((type22) == (PORTA1)) || ((type22) == PORTA2) || ((type11) == PORTA3) || ((type11) == PORTA4))) {
			bool flag = false;
			for (int i = cellX - 2; i < cellX + 2; i++) {
				for (int j = cellY - 3; j < cellY + 3; j++) {
					eCellType& type3 = map->getCell(i, j, 6).type;
					if (type3 == SPECIAL_COLLISION) { flag = true; }
				}
			}

			for (int i = cellX - 2; i < cellX + 2; i++) {
				for (int j = cellY - 3; j < cellY + 3; j++) {
					eCellType& type1 = map->getCell(i, j, 3).type;
					eCellType& type2 = map->getCell(i, j, 5).type;
					eCellType& type3 = map->getCell(i, j, 6).type;
					if (!flag) {
						if (type2 == PORTA1) { type2 = PORTA5; }
						if (type2 == PORTA2) { type2 = PORTA6; }
						if (type1 == PORTA3) { type1 = PORTA7; }
						if (type1 == PORTA4) { type1 = PORTA8; }
						if (type3 == COLLISION) { type3 = NO_COLLISION; }
					}
					if (flag && (player1->num_fight_elements == 10)) {
						if (type2 == PORTA1) { type2 = PORTA5; }
						if (type2 == PORTA2) { type2 = PORTA6; }
						if (type1 == PORTA3) { type1 = PORTA7; }
						if (type1 == PORTA4) { type1 = PORTA8; }
						type3 = NO_COLLISION;
					}
				}
			}
		}
		cellX = ((player1->position.x + 10) / 2) / tileWidth ;
		cellY = ((player1->position.y + 10) / 2) / tileHeight ;
		eCellType& type = map->getCell(cellX, cellY, 4).type;
		if ( (type == FIRE2 || (type == FIRE1))) {
			type = EMPTY;
			time_light = time;
			player1->fires_count++;
			printf("%d", player1->fires_count++);
		}
		
		eCellType& type5 = map->getCell(cellX, cellY, 4).type;
		if ((type5 == SWORD1_UP || (type5 == SWORD1_DOWN))) {
			if (type5 == SWORD1_UP) { type5 = EMPTY; eCellType& type6 = map->getCell(cellX, cellY+1, 4).type; type6 = EMPTY; }
			if (type5 == SWORD1_DOWN) { type5 = EMPTY; eCellType& type6 = map->getCell(cellX, cellY-1, 4).type; type6 = EMPTY; }
			player1->num_fight_elements++;
		}
		if ((type5 == SWORD2_UP || (type5 == SWORD2_DOWN))) {
			if (type5 == SWORD2_UP) { type5 = EMPTY; eCellType& type6 = map->getCell(cellX, cellY + 1, 4).type; type6 = EMPTY; }
			if (type5 == SWORD2_DOWN) { type5 = EMPTY; eCellType& type6 = map->getCell(cellX, cellY - 1, 4).type; type6 = EMPTY; }
			player1->num_fight_elements++;
		}
	}
	

	//to read the gamepad state
	if (Input::gamepads[0].isButtonPressed(A_BUTTON)) //if the A button is pressed
	{
	}

	if (Input::gamepads[0].direction & PAD_UP) //left stick pointing up
	{
		bgcolor.set(0, 255, 0);
	}

	
	int marginW = 160 / 2;
	int marginH = 120 / 2;

	camera.offset.x = clamp(player1->position.x, marginW, map->width * 16 - marginW) - marginW;
	camera.offset.y = clamp(player1->position.y, marginH, map->height * 16 - marginH) - marginH;
}

LooseStage::LooseStage()
{
	font.loadTGA("data/bitmap-font-white.tga");
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
	fons_monster.loadTGA("data/monster.tga");
}

void LooseStage::render(Image& fb)
{
	fb.drawText("You lost the game", 5, 10, font);
	fb.drawText("See you!", 5, 28, font);
	fb.drawText( "To restart, press Z", 5, 42, minifont, 4, 6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)
}

void LooseStage::update(float delta)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{
		Game::instance->changeState(1);
	}
}

WinStage::WinStage()
{
	font.loadTGA("data/bitmap-font-white.tga");
	minifont.loadTGA("data/mini-font-white-4x6.tga");
}

void WinStage::render(Image& fb)
{
	fb.drawText("You found and beat", 5, 10, font);
	fb.drawText("the monster", 5, 22, font);
	fb.drawText("To restart, press Z", 5, 42, minifont, 4, 6);
}

void WinStage::update(float delta)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{
		Game::instance->changeState(1);
	}
}


