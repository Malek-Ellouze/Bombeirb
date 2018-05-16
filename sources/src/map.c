/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <bomb.h>
#include <monster.h>
#include <game.h>
#include <player.h>






struct map {
	int width;
	int height;
	unsigned char* grid;
};

#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);
	if (x<0 || x+1> map-> width || y<0 || y+1> map-> height)
		return 0;
	return 1;
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}

enum bonus_type map_get_cell_bonus_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}
enum compose_type map_get_cell_compose_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)];
}
int get_level_door(struct map* map,int x,int y){
	assert(map && map_is_inside(map, x, y));
	return (map->grid[CELL(x,y)] & 0x0e)/2;
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void  map_set_cell_compose_type(struct map* map, int x, int y, enum compose_type type){
	assert(map && map_is_inside(map, x, y));
		map->grid[CELL(x,y)] = type;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;
	case BONUS_LIFE:
		window_display_image(sprite_get_bonus(BONUS_LIFE), x, y);
		break;
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	case SCENERY_PRINCESS:
		window_display_image(sprite_get_princess(), x, y);
				break;
	}
}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];
	    
	    switch (type & 0xf0) {
		case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		  break;
	    case CELL_BOX:
	      window_display_image(sprite_get_box(), x, y);
	      break;
	    case CELL_BONUS:
	    	display_bonus(map, x, y, type);
	      break;
	    case CELL_KEY:
	      window_display_image(sprite_get_key(), x, y);
	      break;
	    case CELL_DOOR:

	      if ((type & 0x01)==0){
	  		   	window_display_image(sprite_get_door_closed(), x, y);}

	      if ((type & 0x01)==1){
	    		window_display_image(sprite_get_door_opened(), x, y);}
	      break;

	    case CELL_BOMB:
	    	  break;

	    case CELL_MONSTER:
//	    	    window_display_image(sprite_get_monster(SOUTH), x,y);
	    	    break;

	    }
	  }
	}
}

enum compose_type map_get_door_state(struct map* map, int x, int y) {
	assert(map && map_is_inside(map, x, y));
		return map->grid[CELL(x,y)] & 0x01;
}



struct map* map_get_static(void)
{
	struct map* map = map_new(STATIC_MAP_WIDTH, STATIC_MAP_HEIGHT);

	unsigned char themap[STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT] = {
			CELL_MONSTER, CELL_EMPTY, CELL_BONUS_RANGEINC, CELL_BONUS_LIFE, CELL_BONUS_RANGEDEC, CELL_KEY, CELL_BONUS_BOMBINC, CELL_BONUS_BOMBDEC, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
	  CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX_MONSTER, CELL_BOX_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX_RANGEDEC, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX_RANGEINC, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX_RANGEDEC, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_BOX_LIFE, CELL_MONSTER, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_BOX, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_BOX_LIFE, CELL_EMPTY,
	  CELL_BOX,  CELL_EMPTY, CELL_DOOR_CLOSED , CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,

		};

	for (int i = 0; i < STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT; i++)
		map->grid[i] = themap[i];

	return map;
}



//load a map from a file.
struct map* map_from_file(char map_file[]) {
	unsigned int cell;
	FILE* file = NULL;
	int height, width, i, j;
	file = fopen(map_file, "r+b");
	fscanf(file, "%d x %d\n", (int*) &height, (int*) &width);
	printf("%i x %i", height, width);

	struct map* map = map_new(width, height);

	for (j = 0; j < width; j++) {
		for (i = 0; i < height; i++) {
			fscanf(file, "%d ", (unsigned int*) &cell);
			map->grid[CELL(i, j)] = cell;
		}
	}
	return map;
}

// Initializing all maps from the file under the folder game(easy ou hard)
struct map ** maps_init(int max_levels, int difficulty) {
	struct map** maps = malloc(max_levels*sizeof(struct map));
	int i;
	char map_file[] = "map/diff_0.txt";

	for (i=0; i<max_levels; i++) {
		if (difficulty == 1) {
			map_file[4] = 'e';
			map_file[5] = 'a';
			map_file[6] = 's';
			map_file[7] = 'y';
		} else {
			map_file[4] = 'h';
			map_file[5] = 'a';
			map_file[6] = 'r';
			map_file[7] = 'd';
		}
		map_file[9] = i + '0';
		maps[i] = map_from_file(map_file);
	}

	return maps;
}
// prepare loading the file.
void map_load_file(char map_file[],struct game* game) {

	struct player* player=game_get_player(game);
	FILE* file = NULL;
	int  max_level,x,y,current_level;
	char name[20];
	file = fopen(map_file, "r");
	fscanf(file, "%d", (int*) &max_level);
	fscanf(file,"%d : %d , %d",(int*) &current_level,(int*) &x,(int*) &y);
	fscanf(file,"%s",(char*) &name);
	game_set_max(game,max_level);
	game_set_current(game,current_level);
	player_set_x(player,x);
	player_set_y(player,y);

}

// save a game.
//void save_map(struct map* map, FILE* name_of_file) {
//	assert(map != NULL);
//	assert(map->height > 0 && map->width > 0);
//	name_of_file= fopen("fichier.txt", "w+b");
//	unsigned char cell;
//	fwrite(&(map->height), sizeof(int), 1, name_of_file);
//	fwrite(&" x ", 3*sizeof(char), 1, name_of_file);
//	fwrite(&(map->width), sizeof(int), 1, name_of_file);
//	fwrite(&"\n", sizeof(char), 1, name_of_file);
//	for (int i = 0; i < map->width; i++) {
//		for (int j = 0; j < map->height; j++) {
//
//			cell = map->grid[CELL(i,j)];
//
//			fwrite(&cell, sizeof(unsigned char), 1, name_of_file);
//			fwrite(&"\n", sizeof(char), 1, name_of_file);
//		}
//
//	}
//
//}
