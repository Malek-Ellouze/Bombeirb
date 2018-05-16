/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>
#include <game.h>
#include <map.h>
struct game;
struct player;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number);
void   player_free(struct player* player);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);
int player_get_portee(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);
void player_dec_nb_bomb_case_0(struct player * player);
int player_get_stop_bomb(struct player* player);
void player_set_stop_bomb(struct player* player,int x);
void player_set_stop_bonus(struct player* player,int x,int i, int j);
int player_get_stop_bonus(struct player* player,int i, int j);
void player_set_invulnerability_on(struct player* player);
void player_set_invulnerability_off(struct player* player);
int player_get_inv(struct player* player) ;
void time_of_invulnerability(struct player* player);
void player_set_x(struct player* player,int x);
void player_set_y(struct player* player,int x);
void player_display(struct player* player);
void player_dec_life(struct player* player);
void player_inc_life(struct player * player);
int player_get_nb_life(struct player* player);
int player_get_nb_key(struct player* player);
int player_get_nb_level(struct player* player);
void player_dec_range(struct player* player);
void player_inc_range(struct player * player);
void player_inc_key(struct player * player);
void player_dec_key(struct player * player);
void player_set_princess(struct player* player,int x);
int player_get_princess(struct player* player);
int player_get_vitesse(struct player* player);
void player_set_vitesse(struct player* player,int x);


// Move the player according to the current direction
int player_move(struct player* player, struct map* map,struct game* game);

// Display the player on the screen

void next_lvl (struct player* player, struct game* game, struct map* map, int x, int y);
int player_on_bomb(struct player* player, struct map* map);


#endif /* PLAYER_H_ */
