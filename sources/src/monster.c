#include <SDL/SDL_image.h>
#include <assert.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <monster.h>
#include <bomb.h>
#include <time.h>
#include <map.h>
#include <player.h>

struct monster{
  int x,y;
  enum direction direction;
  int last_move_time;

};
struct monster* monster_init(int x, int y){
  struct monster* monster= malloc(sizeof(*monster));
  if(!monster)
		error("Memory error");
  monster->x=x;
  monster->y=y;
  monster->direction=SOUTH;
  monster->last_move_time=SDL_GetTicks();


  return monster;
}
//generates a list of monster
struct list_of_monsters{
	struct monster* monster;
	struct list_of_monsters* next_monster;

};
//initializes the list of monster by reading the different cells of the map.
struct list_of_monsters* list_of_monsters_init(struct map* map){
	assert(map);
		struct list_of_monsters* monster_list = NULL;
		int h = map_get_height(map);
		int w = map_get_width(map);
		int i, j;
		for (i = 0; i < w; i++) {
			for (j = 0; j < h; j++) {
				if (map_get_cell_type(map, i, j) == CELL_MONSTER){
					struct monster* monster = monster_init(i,j);
					monster_list = monster_list_add(monster_list,monster);
				}
			}
		}

		return monster_list;
}
// Add a monster at the beginning of the list.
struct list_of_monsters* monster_list_add(struct list_of_monsters* list, struct monster* monster){

	struct list_of_monsters* new_list = malloc(sizeof(*new_list));
	new_list->monster = monster;
	if(list==NULL)
			new_list->next_monster=NULL;
		else
	new_list->next_monster = list;
	return new_list;
}
struct list_of_monsters* monster_list_new(void){
	return NULL;
}
// Remove a monster from the list of monnsters knowing its position.
struct list_of_monsters* monster_list_remove(struct list_of_monsters* list,int x, int y){

	if(list == NULL)
			return NULL;



	 else if (monster_get_x(list->monster)==x && monster_get_y(list->monster)==y){

			struct list_of_monsters * list_finale = list->next_monster;
			free(list);
			list_finale = monster_list_remove(list_finale, x, y);
			return list_finale;
			 }

		else{
			list->next_monster = monster_list_remove(list->next_monster,x,y);
			return list;
		}
	}



void monster_free(struct monster* monster) {
	assert(monster);
	free(monster);
}

int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

enum direction monster_get_direction(struct monster* monster) {
	assert(monster != NULL);
	return monster->direction;
}
// Set conditions on the moves of the monster.
static int monster_move_aux(struct monster* monster, struct game* game, int x, int y) {
	struct map* map=game_get_current_map(game);
	struct player* player= game_get_player(game);
	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_BOX:
		return 0;
		break;

	case CELL_BONUS:
    return 0;
		break;

	case CELL_MONSTER:
    return 0;
		break;
  case CELL_KEY:
    return 0;
    break;
  case CELL_DOOR:
    return 0;
    break;
	case CELL_BOMB:
//			map_set_cell_type(map,x,y,CELL_EMPTY);
//			break;
	case CELL_PLAYER:

		break;


	default:
		break;
	}

	// Monster has moved
	return 1;
}


// setting the changes in the map when a move occurs.
int monster_move(struct monster* monster, struct game* game) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;
	struct map* map=game_get_current_map(game);
	switch (monster->direction) {
	case NORTH:
		if (monster_move_aux(monster, game, x, y - 1)) {
			if(map_is_inside(map,x,y-2)&& map_get_cell_type(map,x,y-2) != CELL_DOOR )
						{
			monster->y--;
			move = 1;
		}
		}
		break;

	case SOUTH:
		if (monster_move_aux(monster, game, x, y + 1)) {
			if(map_is_inside(map,x,y+2)&& map_get_cell_type(map,x,y+2) != CELL_DOOR )
						{
			monster->y++;
			move = 1;
		}
		}
		break;

	case WEST:
		if (monster_move_aux(monster, game, x - 1, y)) {
			if(map_is_inside(map,x-2,y)&& map_get_cell_type(map,x-2,y) != CELL_DOOR )
						{
			monster->x--;
			move = 1;
		}
		}
		break;

	case EAST:
		if (monster_move_aux(monster, game, x + 1, y)) {
			if(map_is_inside(map,x+2,y)&& map_get_cell_type(map,x+2,y) != CELL_DOOR )
			{
			monster->x++;
			move = 1;
			}
		}
		break;

	}


	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
	}
	return move;
}
void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}
//Calculates the time of the last move and deduct the time of the new move and make the monster move.
struct monster* make_monster_move(struct monster* monster, struct game* game){
  if (monster!=NULL){
struct list_of_monsters* list=game_get_list_of_monsters(game);
struct player* player=game_get_player(game);
    int actualTime=SDL_GetTicks();
    int currentTime=monster->last_move_time;

    if ((actualTime-currentTime)>player_get_vitesse(player)){
      monster_set_current_way(monster,rand()%4);
      monster_move(monster,game);
      monster->last_move_time=SDL_GetTicks();
    }
  }
  return monster;
}
//Displaying the monster
void monster_display(struct monster* monster, struct game* game) {
	assert(monster);
	   int pause=game_get_pause(game);
	if(pause==0){
	make_monster_move(monster,game);
	}

	window_display_image(sprite_get_monster(monster->direction),monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}
// displaying the list of monsters.
void  monster_list_display(struct list_of_monsters* list,struct game* game){
	struct list_of_monsters* list_finale = list;
	while(list_finale != NULL){
		monster_display(list_finale->monster,game);
		list_finale = list_finale->next_monster;
	}
}
void monster_hit_player(struct player* player, struct list_of_monsters* list){
	int px=0;
	int py=0;
	int mx=0,my=0;
	while (list!=NULL){
		 px=player_get_x(player);
		 py=player_get_y(player);
		 mx=monster_get_x(list->monster);
		 my=monster_get_y(list->monster);
		 if ((px=mx)&&(py=my)&&(player_get_inv(player)==0))
		 {
			 player_dec_life(player);
			 player_set_invulnerability_on(player);
		 	 list=list->next_monster;
		 }
		 else
			 list=list->next_monster;
	}

}
