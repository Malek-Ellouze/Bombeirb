/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <bomb.h>
#include <game.h>
#include <monster.h>

struct player {
	int x, y;
	enum direction current_direction;
	int nb_bombs;
	int nb_bomb_now;
	int life;
	int portee;
	int key;
	int level;
	int invulnerability_state;
	int invulnerability_time_on;
	int stop_bomb;
	int stop_bonus[12][12];
	int princess;
	int vitesse;
};

struct player* player_init(int bomb_number) {
	struct player* player = malloc(sizeof(*player));
	int i,j;
	if (!player)
		error("Memory error");

	player->current_direction = SOUTH;
	player->nb_bombs = bomb_number;
	player->nb_bomb_now = bomb_number;
	player->life=4;
	player->portee=1;
	player->key=0;
	player->level=0;
	player->invulnerability_state=0;
	player->stop_bomb=0; // to decrease only one life whe the player is on an explosion.
	for (i = 0; i < 12; i++) {
						for (j = 0; j < 12; j++) {
	player->stop_bonus[i][j]=0;
						}} // to garantee that a bonus contained in a box doesn't explode
	player->princess=0; // to know if the player reached the princess or not.
	player->vitesse=4000; // the speed of monsters.
	return player;
}
// set the state of the princess to know if the player won the game or not.
void player_set_princess(struct player* player,int x){
	assert(player);
		player->princess = x;
}

int player_get_princess(struct player* player){
	assert(player);
		return player->princess ;
}

void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}


void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

int player_get_inv(struct player* player) {
	assert(player != NULL);
	return player->invulnerability_state;
}

int player_get_portee(struct player* player) {
	assert(player != NULL);
	return player->portee;
}

int player_get_stop_bomb(struct player* player) {
	assert(player != NULL);
	return player->stop_bomb;
}
void player_set_stop_bomb(struct player* player,int x) {
	assert(player);
	player->stop_bomb = x;
}

void player_set_x(struct player* player,int x) {
	assert(player);
	player->x = x;
}
void player_set_y(struct player* player,int x) {
	assert(player);
	player->y = x;
}

void player_set_stop_bonus(struct player* player,int x,int i, int j) {
	assert(player);
	player->stop_bonus[i][j] = x;
}
int player_get_stop_bonus(struct player* player,int i,int j) {
	assert(player);
	return player->stop_bonus[i][j];
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bomb_now += 1;
	player->nb_bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	if (player->nb_bombs > 1) {

		player->nb_bombs -= 1;

		if (player->nb_bomb_now >= 1)
			player->nb_bomb_now -= 1;

	}
}

int player_get_vitesse(struct player* player) {
	assert( player);
	return player->vitesse;
}
void player_set_vitesse(struct player* player,int x) {
	assert(player);
	player->vitesse = x;
}
// to set the player invulnerable.
void player_set_invulnerability_on(struct player* player) {
	assert(player);
	player->invulnerability_state = 1;
	player->invulnerability_time_on=SDL_GetTicks();
}

void time_of_invulnerability(struct player* player){
	int actualTime=SDL_GetTicks();
	 if((actualTime - player->invulnerability_time_on) > 2000)
		 player_set_invulnerability_off(player);

}
void player_set_invulnerability_off(struct player* player) {
	assert(player);
	player->invulnerability_state = 0;
}


int player_get_nb_bomb_now(struct player* player) {
	assert(player);
	return player->nb_bomb_now;
}
int player_get_nb_life(struct player* player) {
	assert(player);
	return player->life;
}

void player_inc_nb_bomb_now(struct player* player) {
	assert(player);
	if (player->nb_bomb_now + 1 <= player->nb_bombs)
		player->nb_bomb_now += 1;
}

void player_dec_nb_bomb_now(struct player* player) {
	assert(player);
	player->nb_bomb_now -= 1;
}

void player_dec_life(struct player* player) {
	assert(player);
		player->life -= 1;
}

void player_inc_life(struct player * player) {
	assert(player);
	if (player->life <9)
		player->life ++;
}

void player_inc_key(struct player * player) {
	assert(player);
	if (player->key <9)
		player->key ++;
}

void player_dec_key(struct player * player) {
	assert(player);
	if (player->key >0)
		player->key --;
}

void player_dec_range(struct player* player) {
	assert(player);
	if (player->portee >1)
		player->portee -= 1;
}
void player_dec_nb_bomb_case_0(struct player * player){
	assert(player);
			player->nb_bombs -= 1;
}

void player_inc_range(struct player * player) {
	assert(player);
	if (player->portee <9)
		player->portee ++;
}
enum direction player_get_direction(struct player* player) {
	assert(player != NULL);
	return player->current_direction;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->current_direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->nb_bombs;
}
int player_get_nb_key(struct player* player) {
	assert(player);
	return player->key;
}
int player_get_nb_level(struct player* player) {
	assert(player);
	return player->level;
}
void next_lvl (struct player* player, struct game* game, struct map* map, int x, int y) {
	// sets the speed of monsters in each map and the doors between the maps//
	int lvl,i,j;


	struct list_of_monsters* list=game_get_list_of_monsters(game);
	int vitesse =player_get_vitesse(player);int current_level=game_get_current_level(game);
	//if the door is open
	if ((map_get_door_state(map,x,y))) {
		// to open the door while crossing it
		 for (i = 0; i < map_get_width(map); i++) {
				 					for (j = 0; j < map_get_height(map); j++) {
				 						if (map_get_cell_compose_type(map, i, j) == 50){
				 							map_set_cell_compose_type(map,i,j,51);

				 						}}}

		 for (i = 0; i < map_get_width(map); i++) {
		 				 			for (j = 0; j < map_get_height(map); j++) {
		 				 			if (map_get_cell_compose_type(map, i, j) == 52){
		 				 			map_set_cell_compose_type(map,i,j,53);

		 				 				}}}

		lvl = get_level_door(map,x,y);
		if (lvl>current_level)
		player_set_vitesse(player,vitesse/(lvl+1));
		else
		player_set_vitesse(player,vitesse*(current_level+1));
		game_set_list_of_monsters_NULL(game);
		game_set_current_level(game,lvl);
		game_set_list_of_monsters(list, game);
		game_set_list_of_monsters(list_of_monsters_init(game_get_current_map(game)), game);
	}
	// if the door is closed.
	else if (!map_get_door_state(map,x,y)&&player_get_nb_key(player)>0) {

				 for (i = 0; i < map_get_width(map); i++) {
						 					for (j = 0; j < map_get_height(map); j++) {
						 						if (map_get_cell_compose_type(map, i, j) == 50){
						 							map_set_cell_compose_type(map,i,j,51);

						 						}}}

				 for (i = 0; i < map_get_width(map); i++) {
				 						 					for (j = 0; j < map_get_height(map); j++) {
				 						 						if (map_get_cell_compose_type(map, i, j) == 52){
				 						 							map_set_cell_compose_type(map,i,j,53);

				 						 						}}}

				lvl = get_level_door(map,x,y);
				player_dec_key(player);
			lvl = get_level_door(map,x,y); // read the level from the code of the door
			if (lvl>current_level)
					player_set_vitesse(player,vitesse/(lvl+1)); // set the speed of the monster in the map
					else
					player_set_vitesse(player,vitesse*(current_level+1));
			game_set_list_of_monsters_NULL(game);
					game_set_current_level(game,lvl);
					game_set_list_of_monsters(list, game);
					game_set_list_of_monsters(list_of_monsters_init(game_get_current_map(game)), game);
		}

	return;
}
// conditions to make te player move.
static int player_move_aux(struct player* player, struct map* map, int x, int y ,struct game* game) {
	int i,j;
	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		switch(map_get_cell_compose_type(map,x,y)){
		case CELL_PRINCESS:
			player_set_princess(player,1);
			break;
		default:
			return 0;
			break;
		}

		break;

	case CELL_BOX:
		return 1;
		break;

	case CELL_BONUS:
		switch(map_get_cell_compose_type(map,x,y)){

		    	  				case CELL_BONUS_RANGEINC:
		    	  					player_inc_range(player);
		    	  					map_set_cell_type(map, x, y, CELL_EMPTY);
		    	  					break;
		    	  				case CELL_BONUS_RANGEDEC:
		    	  					player_dec_range(player);
		    	  					map_set_cell_type(map, x, y, CELL_EMPTY);
		    	  					break;
		    	  				case CELL_BONUS_BOMBINC:
		    	  					player_inc_nb_bomb(player);
		    	  					map_set_cell_type(map, x, y, CELL_EMPTY);
		    	  					break;
		    	  				case CELL_BONUS_BOMBDEC:
		    	  					player_dec_nb_bomb(player);
		    	  					map_set_cell_type(map, x, y, CELL_EMPTY);
		    	  					break;
		    	  				case CELL_BONUS_LIFE:
		    	  					player_inc_life(player);
		    	  					map_set_cell_type(map, x, y, CELL_EMPTY);
		    	  					break;

		    	  				default:
		    	  					break;
		    	  				}
		break;

	case CELL_MONSTER: // if the player is on a cell monster.
		map_set_cell_type(map,x,y,CELL_MONSTER);
		if(player->invulnerability_state==0){
		player_dec_life(player);
		player_set_invulnerability_on(player);
		}
		if(player->invulnerability_state==1){
		time_of_invulnerability( player);
		}
		break;

	case CELL_BOMB:
//		player_dec_life(player);
//			return 1;
//			break;
	case CELL_KEY:

		 map_set_cell_type(map, x, y, CELL_EMPTY);
		 player_inc_key( player);

		break;

	case CELL_DOOR:
		//go to the next level.
		next_lvl(player,game,game_get_current_map(game),x,y);

			return 1;



	default:
		break;
	}

	// Player has moved
	return 1;
}
// set the new poision of the player when he moves and the modifications in the map.
int player_move(struct player* player, struct map* map,struct game* game) {
	int x = player->x;
	int y = player->y;
	int move = 0;
	int pos_boxx=x,pos_boxy=y;

	switch (player->current_direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1,game)) {
			player->y--;
			pos_boxy=y-2;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1,game)) {
			player->y++;
			pos_boxy=y+2;
			move = 1;

		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y,game)) {
			player->x--;
			pos_boxx=x-2;

			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y,game)) {
			player->x++;
			pos_boxx=x+2;
			move = 1;

		}
		break;
	}
	// Cas si le joueur rencontre une caisse et doit donc la déplacer//
	if (map_get_cell_type(map,player->x,player->y)== CELL_BOX)
	{
		if ((map_is_inside(map,pos_boxx,pos_boxy))&& (map_get_cell_type(map,pos_boxx,pos_boxy)!= CELL_BOX)&& (map_get_cell_type(map,pos_boxx,pos_boxy)!= CELL_DOOR)&& (map_get_cell_type(map,pos_boxx,pos_boxy)!= CELL_MONSTER)&& (player_move_aux(player,map,pos_boxx,pos_boxy,game)))
			//il faut vérifier que le fait de pousser la caisse ne l'entrâine pas en dehors de l'écran et respecter le fait qu'on ne puisse pas pousser deux caisses à la fois.//
		{




			switch(map_get_cell_compose_type(map,player->x,player->y)){

					    	  				case CELL_BOX_RANGEINC:
					    	  					map_set_cell_compose_type(map,pos_boxx,pos_boxy,CELL_BOX_RANGEINC);
					    	  					break;
					    	  				case CELL_BOX_RANGEDEC:
					    	  					map_set_cell_compose_type(map,pos_boxx,pos_boxy,CELL_BOX_RANGEDEC);
					    	  					break;
					    	  				case CELL_BOX_BOMBINC:
					    	  					map_set_cell_compose_type(map,pos_boxx,pos_boxy,CELL_BOX_BOMBINC);
					    	  					break;
					    	  				case CELL_BOX_BOMBDEC:
					    	  					map_set_cell_compose_type(map,pos_boxx,pos_boxy,CELL_BOX_BOMBDEC);
					    	  					break;
					    	  				case CELL_BOX_LIFE:
					    	  					map_set_cell_compose_type(map,pos_boxx,pos_boxy,CELL_BOX_LIFE);
					    	  					break;

					    	  				default:
					    	  					map_set_cell_type(map,pos_boxx,pos_boxy,CELL_BOX);
					    	  					break;
					    	  				}
			map_set_cell_type(map, player->x,player->y,CELL_EMPTY);


// Cas si le joueur arrive à la porte.//
if (map_get_cell_type(map,player->x,player->y)== CELL_DOOR)
{

}


		}
		else
			player->x=x;
			player->y=y;
			move=0;

	}

	if (move) {
//		map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return move;
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->current_direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}




