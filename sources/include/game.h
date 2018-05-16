/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>

// Abstract data type
struct game;

// Create a new game
struct game* game_new();

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);
int game_get_pause(struct game* game);

// Return the current map
struct map* game_get_current_map(struct game* game);
int game_get_max_levels(struct game* game);
int game_get_current_level(struct game* game);
struct list_of_monsters* game_get_list_of_monsters(struct game* game);
struct liste_de_bombes* game_get_list_of_bombs(struct game* game);
void game_set_list_of_monsters(struct list_of_monsters* list, struct game* game);
void game_set_list_of_bombs(struct liste_de_bombes* list, struct game* game);
void game_set_current_level(struct game* game, int level);
void game_set_list_of_monsters_NULL(struct game* game);
void game_set_current(struct game* game,int x);
void game_set_max(struct game* game,int x);
int game_menu(struct game* game);

// Display the game on the screen
void game_display(struct game* game);

// update
int game_update(struct game* game);


#endif /* GAME_H_ */
