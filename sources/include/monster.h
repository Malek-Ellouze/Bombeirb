#include <SDL/SDL_image.h>
#include <assert.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <bomb.h>
#include <time.h>
#include <player.h>
struct monster;
// Creates a new monster or list of monsters
struct monster* monster_init(int x, int y);
struct list_of_monsters* monster_list_remove(struct list_of_monsters* list, int x,int y);
struct list_of_monsters* list_of_monsters_init(struct map* map);
struct list_of_monsters* monster_list_add(struct list_of_monsters* list, struct monster* monster);
struct list_of_monsters* list_of_monsters_init(struct map* map);
void monster_display(struct monster* monster, struct game* game);

// monster set and get
void monster_free(struct monster* monster);
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);
enum direction monster_get_direction(struct monster* monster);

// set the monster moves.
static int monster_move_aux(struct monster* monster, struct game* game, int x, int y);
void monster_hit_player(struct player* player, struct list_of_monsters* list);
int monster_move(struct monster* monster, struct game* game);
void monster_set_current_way(struct monster* monster, enum direction way);
struct monster* make_monster_move(struct monster* monster, struct game* game);
// displays the list of monsters.
void  monster_list_display(struct list_of_monsters* list,struct game* game);
struct list_of_monsters* monster_list_new(void);



