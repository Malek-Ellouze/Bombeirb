#include <map.h>
#include <game.h>
#include <constant.h>

//initializing and modifications on the list of bombs.
struct bomb* initialisation_bombe(int x, int y, int range);
struct liste_de_bombes* liste_de_bombes_init();
struct liste_de_bombes* bomb_list_remove(struct liste_de_bombes* list);
struct liste_de_bombes* bomb_list_add(struct liste_de_bombes* list, struct game* game);
//Bomb attribute
int bomb_get_x(struct bomb* bomb);
int bomb_get_y(struct bomb* bomb);
int bomb_get_portee(struct bomb* bomb);
int bomb_get_state(struct bomb* bomb);
//
void bomb_update_state( struct bomb* bomb, struct game* game);

// Display the bomb
struct bomb*  incrementation_du_compteur(struct bomb* bomb, struct game* game);
void display_bomb(struct bomb* bomb, struct game* game);
void  bomb_list_display(struct liste_de_bombes* list,struct game* game);
int conditions_of_explosion(struct map* map, int x,int y);
int scenery_stop_explosion1(struct map* map,int x,int y,int portee);
int scenery_stop_explosion2(struct map* map,int x,int y,int portee);
int scenery_stop_explosion3(struct map* map,int x,int y,int portee);
int scenery_stop_explosion4(struct map* map,int x,int y,int portee);
int box_stop_explosion1(struct map* map,int x,int y,int portee);
int box_stop_explosion2(struct map* map,int x,int y,int portee);
int box_stop_explosion3(struct map* map,int x,int y,int portee);
int box_stop_explosion4(struct map* map,int x,int y,int portee);

struct liste_de_bombes* bomb_list_new(void);
