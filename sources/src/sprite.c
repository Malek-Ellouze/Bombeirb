/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <sprite.h>
#include <misc.h>

// Sprites general
#define MAP_CASE        "sprite/wood_box.png"
#define MAP_KEY			"sprite/key.png"
#define MAP_DOOR_OPENED	"sprite/door_opened.png"
#define MAP_DOOR_CLOSED	"sprite/door_closed.png"
#define woman	"sprite/bomberwoman.png"

// Scenery elements
#define MAP_STONE		"sprite/stone.png"
#define MAP_TREE        "sprite/tree.png"

// Sprites of Banner
#define BANNER_LINE		"sprite/banner_line.png"
#define BANNER_LIFE		"sprite/banner_life.png"
#define BANNER_BOMB		"sprite/bomb3.png"
#define BANNER_RANGE		"sprite/banner_range.png"
#define BANNER_0			"sprite/banner_0.jpg"
#define BANNER_1			"sprite/banner_1.jpg"
#define BANNER_2			"sprite/banner_2.jpg"
#define BANNER_3			"sprite/banner_3.jpg"
#define BANNER_4			"sprite/banner_4.jpg"
#define BANNER_5			"sprite/banner_5.jpg"
#define BANNER_6			"sprite/banner_6.jpg"
#define BANNER_7			"sprite/banner_7.jpg"
#define BANNER_8			"sprite/banner_8.jpg"
#define BANNER_9			"sprite/banner_9.jpg"

//// Sprites of Bombs
//#define BOMB_TTL1       "sprite/bomb1.png"
//#define BOMB_TTL2       "sprite/bomb2.png"
//#define BOMB_TTL3       "sprite/bomb3.png"
//#define BOMB_TTL4       "sprite/bomb4.png"

// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"
#define IMG_BONUS_LIFE   		  "sprite/banner_life.png"

// Sprites of monsters
#define MONSTER_LEFT     "sprite/monster_left.png"
#define MONSTER_UP       "sprite/monster_up.png"
#define MONSTER_RIGHT    "sprite/monster_right.png"
#define MONSTER_DOWN     "sprite/monster_down.png"
// Sprites of Players
#define PLAYER_LEFT     "sprite/player_left.png"
#define PLAYER_UP       "sprite/player_up.png"
#define PLAYER_RIGHT    "sprite/player_right.png"
#define PLAYER_DOWN     "sprite/player_down.png"
//DIFFERENTES BOMBES
#define FIRST_BOMB     "sprite/bomb1.png"
#define SECOND_BOMB     "sprite/bomb2.png"
#define THIRD_BOMB     "sprite/bomb3.png"
#define FOURTH_BOMB     "sprite/bomb4.png"
#define EXPLOSIONN     "sprite/explosion.png"
#define drapeau     "sprite/flag.png"
// MENU GAME OVER
#define menu     "sprite/menu.png"
#define game_over     "sprite/game_over.png"
#define you_win     "sprite/you_win.png"
#define pauuse     "sprite/pause.png"
// banner
SDL_Surface* numbers[10];
SDL_Surface* banner_life;
SDL_Surface* banner_bomb;
SDL_Surface* banner_range;
SDL_Surface* banner_line;

// map
SDL_Surface* box;
SDL_Surface* goal;
SDL_Surface* key;
SDL_Surface* door_opened;
SDL_Surface* door_closed;
SDL_Surface* stone;
SDL_Surface* tree;
SDL_Surface* flagg;
SDL_Surface* princess;
// bonus
#define NB_BONUS 6
SDL_Surface* bonus[NB_BONUS+1];

// player
SDL_Surface* player_img[4];
// monster
SDL_Surface* monster_img[4];
//BOMBES
SDL_Surface* bomb1;
SDL_Surface* bomb2;
SDL_Surface* bomb3;
SDL_Surface* bomb4;
SDL_Surface* explosion;
SDL_Surface* bomb_various_images[5];
//MENU
SDL_Surface* menuu;
SDL_Surface* gameover;
SDL_Surface* youwin;
SDL_Surface* pause;

static void banner_load() {
	// numbers imgs
	numbers[0] = load_image(BANNER_0);
	numbers[1] = load_image(BANNER_1);
	numbers[2] = load_image(BANNER_2);
	numbers[3] = load_image(BANNER_3);
	numbers[4] = load_image(BANNER_4);
	numbers[5] = load_image(BANNER_5);
	numbers[6] = load_image(BANNER_6);
	numbers[7] = load_image(BANNER_7);
	numbers[8] = load_image(BANNER_8);
	numbers[9] = load_image(BANNER_9);

	// other banner sprites
	banner_life = load_image(BANNER_LIFE);
	banner_bomb = load_image(BANNER_BOMB);
	banner_range = load_image(BANNER_RANGE);
	banner_line = load_image(BANNER_LINE);
	flagg=load_image(drapeau);
}

static void banner_unload() {
	// numbers imgs
	for (int i = 0; i < 10; i++) {
		SDL_FreeSurface(numbers[i]);
	}

	// other banner sprites
	SDL_FreeSurface(banner_life);
	SDL_FreeSurface(banner_bomb);
	SDL_FreeSurface(banner_range);
	SDL_FreeSurface(banner_life);
}

void bomb_load() {
	bomb_various_images[0] = load_image(FIRST_BOMB);
	bomb_various_images[1] = load_image(SECOND_BOMB);
	bomb_various_images[2] = load_image(THIRD_BOMB);
	bomb_various_images[3] = load_image(FOURTH_BOMB);
	bomb_various_images[4] = load_image(EXPLOSIONN);

}
void menu_load(){
	menuu = load_image(menu);

}
void end_load(){
	gameover = load_image(game_over);
}
void win_load(){
	youwin = load_image(you_win);
}

void princess_load(){
	princess = load_image(woman);

}
void pause_load(){
	pause = load_image(pauuse);

}
void menu_unload() {
	SDL_FreeSurface(menuu);

}

void end_unload() {
	SDL_FreeSurface(gameover);

}

void win_unload() {
	SDL_FreeSurface(youwin);

}
void princess_unload(){
	SDL_FreeSurface(princess);

}
void pause_unload(){
	SDL_FreeSurface(pause);

}
static void map_load() {
	// Sprite loading
	tree = load_image(MAP_TREE);
	box = load_image(MAP_CASE);
	key = load_image(MAP_KEY);
	stone = load_image(MAP_STONE);
	door_opened = load_image(MAP_DOOR_OPENED);
	door_closed = load_image(MAP_DOOR_CLOSED);
	//BOMB LOADING
	bomb1=load_image(FIRST_BOMB);
	bomb2=load_image(SECOND_BOMB);
	bomb3=load_image(THIRD_BOMB);
	bomb4=load_image(FOURTH_BOMB);
	explosion=load_image(EXPLOSIONN);
}
 void bomb_unload() {
	SDL_FreeSurface(bomb1);
	SDL_FreeSurface(bomb2);
	SDL_FreeSurface(bomb3);
	SDL_FreeSurface(bomb4);
	SDL_FreeSurface(explosion);

}
static void map_unload() {
	SDL_FreeSurface(tree);
	SDL_FreeSurface(box);
	SDL_FreeSurface(goal);
	SDL_FreeSurface(key);
	SDL_FreeSurface(stone);
	SDL_FreeSurface(door_opened);
	SDL_FreeSurface(door_closed);
}

static void bonus_load() {
	bonus[0] = NULL;
	bonus[BONUS_BOMB_RANGE_INC] = load_image(IMG_BONUS_BOMB_RANGE_INC);
	bonus[BONUS_BOMB_RANGE_DEC] = load_image(IMG_BONUS_BOMB_RANGE_DEC);
	bonus[BONUS_BOMB_NB_INC] = load_image(IMG_BONUS_BOMB_NB_INC);
	bonus[BONUS_BOMB_NB_DEC] = load_image(IMG_BONUS_BOMB_NB_DEC);
	bonus[BONUS_LIFE] = load_image(IMG_BONUS_LIFE);
}

static void bonus_unload() {
	for (int i = 0; i < NB_BONUS; i++)
		if(bonus[i])
			SDL_FreeSurface(bonus[i]);
}

static void player_load() {
	player_img[WEST] = load_image(PLAYER_LEFT);
	player_img[EAST] = load_image(PLAYER_RIGHT);
	player_img[NORTH] = load_image(PLAYER_UP);
	player_img[SOUTH] = load_image(PLAYER_DOWN);
}

static void player_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(player_img[i]);
}

 void monster_load() {
	monster_img[WEST] = load_image(MONSTER_LEFT);
	monster_img[EAST] = load_image(MONSTER_RIGHT);
	monster_img[NORTH] = load_image(MONSTER_UP);
	monster_img[SOUTH] = load_image(MONSTER_DOWN);
}

 void monster_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(player_img[i]);
}

void sprite_load() {
	map_load();
	bonus_load();
	banner_load();
	player_load();
	monster_load();
	menu_load();
	end_load();
	win_load();
	princess_load();
	pause_load();
}

void sprite_free() {
	map_unload();
	bonus_unload();
	banner_unload();
	player_unload();
	monster_unload();
	bomb_unload();
	menu_unload();
		end_unload();
		win_unload();
		princess_unload();
		pause_unload();
}

SDL_Surface* sprite_get_number(short number) {
	assert(number >= 0 && number < 10);
	return numbers[number];
}

SDL_Surface* sprite_get_player(enum direction direction) {
	assert(player_img[direction]);
	return player_img[direction];
}
SDL_Surface* sprite_get_monster(enum direction direction) {
	assert(monster_img[direction]);
	return monster_img[direction];
}

SDL_Surface* sprite_get_banner_life() {
	assert(banner_life);
	return banner_life;
}

SDL_Surface* sprite_get_banner_bomb() {
	assert(banner_bomb);
	return banner_bomb;
}

SDL_Surface* sprite_get_banner_line() {
	assert(banner_line);
	return banner_line;
}

SDL_Surface* sprite_get_banner_range() {
	assert(banner_range);
	return banner_range;
}

SDL_Surface* sprite_get_bonus(enum bonus_type bonus_type) {
	assert(bonus[bonus_type]);
	return bonus[bonus_type];
}

SDL_Surface* sprite_get_tree() {
	assert(tree);
	return tree;
}

SDL_Surface* sprite_get_box() {
	assert(box);
	return box;
}

SDL_Surface* sprite_get_key() {
	assert(key);
	return key;
}

SDL_Surface* sprite_get_stone() {
	assert(stone);
	return stone;
}

SDL_Surface* sprite_get_door_opened() {
	assert(door_opened);
	return door_opened;
}

SDL_Surface* sprite_get_door_closed() {
	assert(door_closed);
	return door_closed;
}
SDL_Surface* sprite_get_bomb_1() {
	assert(bomb1);
	return bomb1;
}
SDL_Surface* sprite_get_bomb_2() {
	assert(bomb2);
	return bomb2;
}
SDL_Surface* sprite_get_bomb_3() {
	assert(bomb3);
	return bomb3;
}
SDL_Surface* sprite_get_bomb_4() {
	assert(bomb4);
	return bomb4;
}
SDL_Surface* sprite_get_explosion() {
	assert(explosion);
	return explosion;
}
SDL_Surface* sprite_get_bomb(int etat) {
	assert(bomb_various_images[etat]);
	return bomb_various_images[etat];
}
SDL_Surface* sprite_get_flag() {
	assert(flagg);
	return flagg;
}

SDL_Surface* sprite_get_menu() {
	assert(menuu);
	return menuu;
}

SDL_Surface* sprite_get_end() {
	assert(gameover);
	return gameover;
}

SDL_Surface* sprite_get_win() {
	assert(youwin);
	return youwin;
}

SDL_Surface* sprite_get_princess() {
	assert(princess);
	return princess;
}

SDL_Surface* sprite_get_pause() {
	assert(pause);
	return pause;
}
