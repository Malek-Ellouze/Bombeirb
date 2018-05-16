/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>
#include <map.h>
#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>
#include <monster.h>

struct game {
	struct map** maps;       // the game's map
	int max_levels;        // nb maps of the game
	int current_level;
	int pause;
	struct player* player;
//	struct bomb* bomb;
	struct liste_de_bombes* list_of_bombs;
//	struct monster* monster;
	struct list_of_monsters* list_of_monsters;
	int difficulty;
};

struct game*
game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->max_levels = 3;
	game->maps = NULL;
//	game->maps[0] = map_get_static();

	game->pause=0;
	game->current_level = 0;
//	game->bomb=initialisation_bombe(0,0,1);
	game->list_of_bombs=NULL;
	game->player = player_init(1);
//	game->monster=monster_init(5,0);
	// Set default location of the player
//	player_set_position(game->player, 1, 0);
	game->list_of_monsters=NULL;
	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	for (int i = 0; i < game->max_levels; i++)
		map_free(game->maps[i]);
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->current_level];
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

int game_over(struct game* game) {
	if (player_get_nb_life(game_get_player(game)) == 0) {
		return 1;
	}
	return 0;

}
// Initialisze the game with information in the file easy or hard.
void game_init(struct game* game, int max_levels, int playerX, int playerY, int current_level, int difficulty) {
		 // load sprites into process memory

		game->max_levels = max_levels;
		game->difficulty = difficulty;
		game->maps = maps_init(game->max_levels, game->difficulty);
		game->current_level = current_level;
		game->list_of_bombs = liste_de_bombes_init();
		game->list_of_monsters = list_of_monsters_init(game_get_current_map(game));
		game->player = player_init(1);

		// Set default location of the player
		player_set_position(game->player, playerX, playerY);
}
//generates the game menu
int game_menu(struct game* game) {


	SDL_Event event;
	int difficulty = 0;
	int max_levels, playerX, playerY, current_level;
	char game_file[] = "game/easy.txt";

	while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_QUIT:
					return 0;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						return 0;
						break;
					case SDLK_e:
						difficulty = 1; // si diffulté =1, on charge les fichiers easy sinon les fichiers hard
						break;
					case SDLK_h:
						difficulty = 2;
						break;
					case SDLK_q:
						return -1;
						break;
					default:
						break;

					}
				}
			}

	window_clear();
	window_display_image(sprite_get_menu(),0,0);
	window_refresh();


	if (difficulty != 0) {


		if (difficulty == 1) {

			game_file[5]='e';
			game_file[6]='a';
			game_file[7]='s';
			game_file[8]='y';

		}
		else {
						game_file[5]='h';
						game_file[6]='a';
						game_file[7]='r';
						game_file[8]='d';
		}
		//regarder partie 1 ou partie 2 et initialiser les trucs qu'il faut

		FILE* file = NULL;
		file = fopen(game_file,"r");
		fscanf(file, "%i\n", (int*) &max_levels);
		fscanf(file, "%i:%i,%i", (int*) &current_level, (int*) &playerX, (int*) &playerY);
		game_init(game,max_levels, playerX, playerY, current_level, difficulty);
		return 0;

	}

	return 1;
}

struct list_of_monsters* game_get_list_of_monsters(struct game* game) {
	assert(game);
	return game->list_of_monsters;
}
void game_set_list_of_monsters(struct list_of_monsters* list, struct game* game){
	assert(game);
	game->list_of_monsters =list;
}
void game_set_list_of_monsters_NULL(struct game* game){
	assert(game);
	game->list_of_monsters =NULL;
}

void game_set_current_level(struct game* game, int level){
	assert(game);
	game->current_level =level;
}

struct liste_de_bombes* game_get_list_of_bombs(struct game* game) {
	assert(game);
	return game->list_of_bombs;
}
void game_set_list_of_bombs(struct liste_de_bombes* list, struct game* game){
	assert(game);
	game->list_of_bombs =list;
}
int game_get_max_levels(struct game* game) {
	assert(game);
	return game->max_levels;
}
int game_get_current_level(struct game* game) {
	assert(game);
	return game->current_level;
}
int game_get_pause(struct game* game) {
	assert(game);
	return game->pause;
}
void game_set_max(struct game* game,int x){
	assert(game);
	game->max_levels =x;
}

void game_set_current(struct game* game,int x){
	assert(game);
	game->current_level =x;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 10 * SIZE_BLOC) / 8;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_life(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_portee(game_get_player(game))), x, y);
// adding the key and the current level.
	x = 4 * white_bloc + 6 * SIZE_BLOC;
		window_display_image(sprite_get_key(), x, y);

	x = 4 * white_bloc + 7 * SIZE_BLOC;
		window_display_image(sprite_get_number(player_get_nb_key(game_get_player(game))), x, y);

	x = 5 * white_bloc + 8 * SIZE_BLOC;
		window_display_image(sprite_get_flag(), x, y);

	x = 5 * white_bloc + 9 * SIZE_BLOC;
		window_display_image(sprite_get_number(game_get_current_level(game)), x, y);




}
// see if the player won the game
int you_win(struct game* game){
	struct player* player=game_get_player(game);

	int won=0;
	if(player_get_princess(player)==1)
	{
		 won=1;
	}
	return won;
}
// adapt displaying to the state of the game (game over, still playing or game won)
void game_display(struct game* game) {
	assert(game);

	window_clear();
if (game_over(game)){
	window_display_image(sprite_get_end(),0,0);
}
else if (you_win(game)){
	window_display_image(sprite_get_win(),0,0);
}
else if (game_get_pause(game)){
	window_display_image(sprite_get_pause(),0,0);
}
else{
	game_banner_display(game);
	map_display(game_get_current_map(game));
	player_display(game->player);
	bomb_list_display(game->list_of_bombs,game);
	monster_list_display(game->list_of_monsters,game);
}
	window_refresh();
}
//controls the keyboard inputs.
static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	int x=player_get_x(player),y=player_get_y(player);

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				if (game->pause == 0){
				player_set_current_way(player, NORTH);
				player_move(player, map,game);}
				break;
			case SDLK_DOWN:
				if (game->pause == 0){
				player_set_current_way(player, SOUTH);
				player_move(player, map,game);}
				break;
			case SDLK_RIGHT:
				if (game->pause == 0){
				player_set_current_way(player, EAST);
				player_move(player, map,game);}
				break;
			case SDLK_LEFT:
				if (game->pause == 0){
				player_set_current_way(player, WEST);
				player_move(player, map,game);}
				break;
			case SDLK_SPACE:
				if (game->pause == 0){
				//Pour rajouter une bombe sur la case du joueur//
				if(player_get_nb_bomb( player)!=0)
				{
				game->list_of_bombs = bomb_list_add(game->list_of_bombs,game);
				player_dec_nb_bomb(player);
				map_set_cell_type(map,x,y, CELL_BOMB);
				}
				}

				break;

			case SDLK_p:
							if (game->pause == 0){
								game->pause = 1;

							}
							else
								game->pause = 0;
							break;
			default:
				break;
						}

						break;
					}
				}
				return 0;
			}
//fonctionalitées ajoutées: pouvoir quitter en cliquant sur n'importe quelle touche si on a gagné ou perdu.
int game_update(struct game* game) {
	SDL_Event event;
	if(game_over(game))
	{
		while (SDL_PollEvent(&event)) {
						switch (event.type) {
						case SDL_QUIT:
							return 1;
						case SDL_KEYDOWN:
								return 1;
								break;
							default:
								return 0;
							}
				}
		return 0;
	}
	else if(you_win(game))
		{
			while (SDL_PollEvent(&event)) {
							switch (event.type) {
							case SDL_QUIT:
								return 1;
							case SDL_KEYDOWN:
									return 1;
									break;
								default:
									return 0;
								}
					}
			return 0;
		}
	else {
		if (input_keyboard(game))

				return 1; // exit game

			return 0;
	}

}
