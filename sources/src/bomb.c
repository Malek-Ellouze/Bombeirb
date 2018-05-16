#include <SDL/SDL_image.h>
#include <assert.h>
#include<game.h>
#include<player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <map.h>
#include <bomb.h>
#include <time.h>
#include <monster.h>


struct bomb {
  int x,y;
  int time;
  int portee;
  enum bomb_state etat;
};
struct liste_de_bombes{
	struct bomb* bomb;
	struct liste_de_bombes* next_bomb;
};
// Initialize to NULL the list of bombs.
struct liste_de_bombes* liste_de_bombes_init(){
	struct liste_de_bombes* bomb_list = NULL;
	return bomb_list;
}
//Add a bomb at the beginning of the list of bombs.
struct liste_de_bombes* bomb_list_add(struct liste_de_bombes* list, struct game* game){
	struct player* player = game_get_player(game);
	struct liste_de_bombes* new_list = malloc(sizeof(*new_list));
	new_list->bomb = initialisation_bombe(player_get_x(player),player_get_y(player),player_get_portee(player));
	if(list==NULL)
		new_list->next_bomb=NULL;
	else
	new_list->next_bomb = list;
	return new_list;
}
// Create an empty list of bombs
struct liste_de_bombes* bomb_list_new(void){
	return NULL;
}
//Remove a bomb from the end of the list of bombs.
struct liste_de_bombes* bomb_list_remove(struct liste_de_bombes* list){
	if (list==NULL)
	{

		return bomb_list_new();
	}
	if (list->next_bomb==NULL){
		free(list);
		list=NULL;
		return bomb_list_new();

	}
	struct liste_de_bombes* temp=list;
	struct liste_de_bombes* before=list;
	while(temp->next_bomb!=NULL)
	{
		before=temp;
		temp=temp->next_bomb;

	}
	before->next_bomb=NULL;
	free(temp);
	temp=NULL;
	return list;




}
//Initialize a bomb
struct bomb* initialisation_bombe(int x,int y, int portee){
  struct bomb* bomb= malloc(sizeof(*bomb));
  bomb->x=x;
  bomb->y=y;
  bomb->portee=portee;
  bomb->time=SDL_GetTicks();
  bomb->etat=BOMB_4_SEC;
  return(bomb);
}
int bomb_get_x(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->x;
}

int bomb_get_y(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->y;
}
int bomb_get_portee(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->portee;
}
//Get the state of the bomb. Depending on this state, we choose the bomb image to display.
int bomb_get_state(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->etat;
}
//SET the state of the bomb. Depending on this state, we choose the bomb image to display.
void bomb_update_state(struct bomb* bomb, struct game* game){


		switch ( bomb->etat ) {
		case BOMB_4_SEC:
			bomb->etat = BOMB_3_SEC;
			break;
		case BOMB_3_SEC:
			bomb->etat = BOMB_2_SEC;
			break;
		case BOMB_2_SEC:
			bomb->etat = BOMB_1_SEC;
			break;
		case BOMB_1_SEC:
			bomb->etat = EXPLOSION;
			break;
		case EXPLOSION:
					bomb->etat = END;
					break;
		default:
			break;
				}

}

//Updating the time of the bomb to generate an explosion after 4 seconds.
struct bomb* incrementation_du_compteur(struct bomb* bomb, struct game* game){
  if (bomb!=NULL){
    int actualTime=SDL_GetTicks();
    int currentTime=bomb->time;
    if (bomb->etat!=END){
      if ((actualTime-currentTime)> 500){
        bomb_update_state(bomb,game);
        bomb->time = SDL_GetTicks();
      }
    }
  }
return(bomb);
}
//Fonction used to display the bomb and its consequences
void display_bomb(struct bomb* bomb,struct game* game){
	struct map* map=game_get_current_map(game);
	struct player* player=game_get_player(game);
	int x=bomb_get_x(bomb);
	int y=bomb_get_y(bomb);
	struct list_of_monsters* list= game_get_list_of_monsters(game);
	struct liste_de_bombes* list1=game_get_list_of_bombs(game);
	struct monster* monster=monster_init(0,0);
	int save_bonus=0;
	int portee=bomb_get_portee(bomb);
	int i,j;
	int pause=game_get_pause(game);
	if (pause==0){ //we do not set the time of the bomb unless the game is not on pause
	incrementation_du_compteur(bomb, game);
	}
  if (bomb!=NULL){
    if (bomb->etat==EXPLOSION){

    	window_display_image(sprite_get_explosion(), x* SIZE_BLOC, y* SIZE_BLOC ); // get the different sprites depending on the state of the bomb
    	game_set_list_of_monsters(monster_list_remove(list,x,y), game);
    	 map_set_cell_type(map,x,y,CELL_EMPTY);
      if((player_get_x(player)==x)&&(player_get_y(player)==y)&&(player_get_stop_bomb(player)==0))
      {
          	  player_dec_life(player); // decrease the life of the player if he is on a cell bomb
          	 player_set_stop_bomb( player,1); // to avoid that the lifes of player keep decreasin. we decrease the player life only once.
      }



      for (i=1;i<portee+1;i++){

      if((conditions_of_explosion(map,x-i,y)!=0)&&(conditions_of_explosion(map,x-i,y)!=2)&&(map_is_inside(map, x-i, y))&&(conditions_of_explosion(map,x-i,y)!=3)&&(conditions_of_explosion(map,x-i,y)!=5)&&scenery_stop_explosion2( map, x-i, y,i)){
    	  //we do not display the sprite of explosion unless the blox is no cell scenery, no cell box, no cell monster and if there is no stone in the way of explosion.
      window_display_image(sprite_get_explosion(), (x-i)* SIZE_BLOC, y* SIZE_BLOC);
//      map_set_cell_type(map,x-i,y,CELL_BOMB);
      if((player_get_x(player)==x-i)&&(player_get_y(player)==y)&&(player_get_stop_bomb(player)==0))
      {
          	  player_dec_life(player);
          	 player_set_stop_bomb( player,1);
      }
      }

      else if ((conditions_of_explosion(map,x-i,y)==2)&&(box_stop_explosion2( map, x-i, y, i))){
    	  //if the cell appears to be a cell box

    	  switch(map_get_cell_compose_type(map,x-i,y)){

    	  	  	  	  	  	  	  	  	case CELL_BOX_RANGEINC:
    	  	  	  	  	  	  	  	   map_set_cell_compose_type(map, x-i, y, CELL_BONUS_RANGEINC); //we show the bonus inside the cell
    	  	  	  	  	  	  	  	   player_set_stop_bonus(player,1,x-i,y); //to avoid that the bonus explodes.
    	            	  					break;
    	            	  				case CELL_BOX_RANGEDEC:
    	            	  				map_set_cell_compose_type(map, x-i, y, CELL_BONUS_RANGEDEC);
    	            	  				player_set_stop_bonus(player,1,x-i,y);


    	            	  					break;
    	            	  				case CELL_BOX_BOMBINC:
    	            	  				map_set_cell_compose_type(map, x-i, y, CELL_BONUS_BOMBINC);
    	            	  				player_set_stop_bonus(player,1,x-i,y);

    	            	  					break;
    	            	  				case CELL_BOX_BOMBDEC:
    	            	  				map_set_cell_compose_type(map, x-i, y, CELL_BONUS_BOMBDEC);
    	            	  				player_set_stop_bonus(player,1,x-i,y);
    	            	  					break;
    	            	  				case CELL_BOX_LIFE:
    	            	  				map_set_cell_compose_type(map, x-i, y, CELL_BONUS_LIFE);
    	            	  				player_set_stop_bonus(player,1,x-i,y);
    	            	  					break;
    	            	  				case CELL_BOX_MONSTER:
    	            	  					map_set_cell_type(map,x-i,y,CELL_MONSTER);
    	            	  				    monster=monster_init(x-i,y);
    	            	  					game_set_list_of_monsters(monster_list_add(list,monster), game); // we add a monster in the list of monsters which is automatically displayed
    	            	  					map_set_cell_type(map,x-i,y,CELL_EMPTY);


    	            	  					break;
    	            	  				default:
    	            	  					map_set_cell_type(map,x-i,y,CELL_EMPTY);
    	            	  					break;
    	  				}
      }
      else if (conditions_of_explosion(map, x-i,y) == 3){
    	  // if the cell is a cell monster we kill it
    	  window_display_image(sprite_get_explosion(), (x-i)* SIZE_BLOC, y* SIZE_BLOC );
    	  map_set_cell_type(map,x-i,y,CELL_EMPTY);

			 game_set_list_of_monsters(monster_list_remove(list,x-i,y), game);
      }
      else if ((conditions_of_explosion(map, x-i,y) == 5)&&(player_get_stop_bonus(player,x-i,y)==0)){
          	  window_display_image(sprite_get_explosion(), (x-i)* SIZE_BLOC, y* SIZE_BLOC );  //if the case if a bonus not a cell box bonus, we explode it
          	  map_set_cell_type(map,x-i,y,CELL_EMPTY);
            }
      }
      for (i=1;i<portee+1;i++){

      if((conditions_of_explosion(map,x+i,y)!=0)&&(conditions_of_explosion(map,x+i,y)!=2)&&(map_is_inside(map, x+i, y))&&(conditions_of_explosion(map,x+i,y)!=3)&&(conditions_of_explosion(map,x+i,y)!=5)&&(scenery_stop_explosion1( map, x+i, y, i)))
      {
      window_display_image(sprite_get_explosion(), (x+i)* SIZE_BLOC, y* SIZE_BLOC);
//      map_set_cell_type(map,x+i,y,CELL_BOMB);
      if((player_get_x(player)==x+i)&&(player_get_y(player)==y)&&(player_get_stop_bomb(player)==0))
      {
          	  player_dec_life(player);
          	 player_set_stop_bomb( player,1);
      }
      }
      else if ((conditions_of_explosion(map,x+i,y)==2)&&(box_stop_explosion1( map, x+i, y, i))){

          	  switch(map_get_cell_compose_type(map,x+i,y)){

          	case CELL_BOX_RANGEINC:
          	          	  				map_set_cell_compose_type(map, x+i, y, CELL_BONUS_RANGEINC);
          	          	  			player_set_stop_bonus(player,1,x+i,y);
          	          	  					break;
          	          	  				case CELL_BOX_RANGEDEC:
          	          	  				map_set_cell_compose_type(map, x+i, y, CELL_BONUS_RANGEDEC);
          	          	  			player_set_stop_bonus(player,1,x+i,y);
          	          	  					break;
          	          	  				case CELL_BOX_BOMBINC:
          	          	  				map_set_cell_compose_type(map, x+i, y, CELL_BONUS_BOMBINC);
          	          	  			player_set_stop_bonus(player,1,x+i,y);
          	          	  					break;
          	          	  				case CELL_BOX_BOMBDEC:
          	          	  				map_set_cell_compose_type(map, x+i, y, CELL_BONUS_BOMBDEC);
          	          	  			player_set_stop_bonus(player,1,x+i,y);
          	          	  					break;
          	          	  				case CELL_BOX_LIFE:
          	          	  				map_set_cell_compose_type(map, x+i, y, CELL_BONUS_LIFE);
          	          	  			player_set_stop_bonus(player,1,x+i,y);
          	          	  					break;
          	          	  				case CELL_BOX_MONSTER:
          	          	  				 map_set_cell_type(map, x+i, y, CELL_MONSTER);
          	          	  			     monster=monster_init(x+i,y);
          	          	  			     game_set_list_of_monsters(monster_list_add(list,monster), game);
          	          	  			     map_set_cell_type(map,x+i,y,CELL_EMPTY);

          	          	  					break;
          	          	  				default:
          	          	  				 map_set_cell_type(map,x+i,y,CELL_EMPTY);
          	          	  					break;
          	  				}
            }
      else if (conditions_of_explosion(map, x+i,y) == 3){
    	  window_display_image(sprite_get_explosion(), (x+i)* SIZE_BLOC, y* SIZE_BLOC );
    	  map_set_cell_type(map,x+i,y,CELL_EMPTY);
      			game_set_list_of_monsters(monster_list_remove(list,x+i,y), game);
            }
      else if ((conditions_of_explosion(map, x+i,y) == 5)&&(player_get_stop_bonus(player,x+i,y)==0)){
                	  window_display_image(sprite_get_explosion(), (x+i)* SIZE_BLOC, y* SIZE_BLOC );
                	  map_set_cell_type(map,x+i,y,CELL_EMPTY);
                  }
      }
      for (i=1;i<portee+1;i++){

      if((conditions_of_explosion(map,x,y-i)!=0)&&(conditions_of_explosion(map,x,y-i)!=2)&&(map_is_inside(map, x, y-i))&&(conditions_of_explosion(map,x,y-i)!=3)&&(conditions_of_explosion(map,x,y-i)!=5 )&&(scenery_stop_explosion4( map, x, y-i, i))){
      window_display_image(sprite_get_explosion(), x* SIZE_BLOC, (y-i)* SIZE_BLOC);
//      map_set_cell_type(map,x,y-i,CELL_BOMB);
      if((player_get_x(player)==x)&&(player_get_y(player)==y-i)&&(player_get_stop_bomb(player)==0))
      {
          	  player_dec_life(player);
          	 player_set_stop_bomb( player,1);
      }
      }
      else if ((conditions_of_explosion(map,x,y-i)==2)&&(box_stop_explosion4( map, x, y-i, i))){


          	  switch(map_get_cell_compose_type(map,x,y-i)){

          	  				case CELL_BOX_RANGEINC:
          	  				map_set_cell_compose_type(map, x, y-i, CELL_BONUS_RANGEINC);
          	  			player_set_stop_bonus(player,1,x,y-i);

          	  					break;
          	  				case CELL_BOX_RANGEDEC:
          	  				map_set_cell_compose_type(map, x, y-i, CELL_BONUS_RANGEDEC);
          	  			player_set_stop_bonus(player,1,x,y-i);
          	  					break;
          	  				case CELL_BOX_BOMBINC:
          	  				map_set_cell_compose_type(map, x, y-i, CELL_BONUS_BOMBINC);
          	  			player_set_stop_bonus(player,1,x,y-i);
          	  					break;
          	  				case CELL_BOX_BOMBDEC:
          	  				map_set_cell_compose_type(map, x, y-i, CELL_BONUS_BOMBDEC);
          	  			player_set_stop_bonus(player,1,x,y-i);
          	  					break;
          	  				case CELL_BOX_LIFE:
          	  				map_set_cell_compose_type(map, x, y-i, CELL_BONUS_LIFE);
          	  			player_set_stop_bonus(player,1,x,y-i);
          	  					break;
          	  				case CELL_BOX_MONSTER:
          	  				map_set_cell_type(map,x,y-i,CELL_MONSTER);
          	  				monster=monster_init(x,y-i);
          	  				game_set_list_of_monsters(monster_list_add(list,monster), game);
          	  				map_set_cell_type(map,x,y-i,CELL_EMPTY);

          	  					break;
          	  				default:
          	  				map_set_cell_type(map,x,y-i,CELL_EMPTY);
          	  					break;
          	  				}
            }
      else if (conditions_of_explosion(map, x,y-i) == 3){
    	  window_display_image(sprite_get_explosion(), x* SIZE_BLOC, (y-i)* SIZE_BLOC );
    	  map_set_cell_type(map,x,y-i,CELL_EMPTY);
      			game_set_list_of_monsters(monster_list_remove(list,x,y-i), game);
            }
      else if ((conditions_of_explosion(map, x,y-i) == 5)&&(player_get_stop_bonus(player,x,y-i)==0)){
                	  window_display_image(sprite_get_explosion(), (x)* SIZE_BLOC, (y-i)* SIZE_BLOC );
                	  map_set_cell_type(map,x,y-i,CELL_EMPTY);
                  }
      }
      for (i=1;i<portee+1;i++){

      if((conditions_of_explosion(map,x,y+i)!=0)&&(conditions_of_explosion(map,x,y+i)!=2)&&(map_is_inside(map, x, y+i))&&(conditions_of_explosion(map,x,y+i)!=3)&&(conditions_of_explosion(map,x,y+i)!=5)&&(scenery_stop_explosion3( map, x, y+i, i))){
      window_display_image(sprite_get_explosion(), x* SIZE_BLOC, (y+i)* SIZE_BLOC);
//      map_set_cell_type(map,x,y,CELL_BOMB);
      if((player_get_x(player)==x)&&(player_get_y(player)==y+i)&&(player_get_stop_bomb(player)==0))
      {
          	  player_dec_life(player);
          	 player_set_stop_bomb( player,1);
      }
      }
      else if ((conditions_of_explosion(map,x,y+i)==2)&&(box_stop_explosion3( map, x, y+i, i))){

          	  switch(map_get_cell_compose_type(map,x,y+i)){

          	case CELL_BOX_RANGEINC:
          	          	  				map_set_cell_compose_type(map, x, y+i, CELL_BONUS_RANGEINC);
          	          	  			player_set_stop_bonus(player,1,x,y+i);
          	          	  					break;
          	          	  				case CELL_BOX_RANGEDEC:
          	          	  				map_set_cell_compose_type(map, x, y+i, CELL_BONUS_RANGEDEC);
          	          	  			player_set_stop_bonus(player,1,x,y+i);
          	          	  					break;
          	          	  				case CELL_BOX_BOMBINC:
          	          	  				map_set_cell_compose_type(map, x, y+i, CELL_BONUS_BOMBINC);
          	          	  			player_set_stop_bonus(player,1,x,y+i);
          	          	  					break;
          	          	  				case CELL_BOX_BOMBDEC:
          	          	  				map_set_cell_compose_type(map, x, y+i, CELL_BONUS_BOMBDEC);
          	          	  			player_set_stop_bonus(player,1,x,y+i);
          	          	  					break;
          	          	  				case CELL_BOX_LIFE:
          	          	  				map_set_cell_compose_type(map, x, y+i, CELL_BONUS_LIFE);
          	          	  			player_set_stop_bonus(player,1,x,y+i);
          	          	  					break;
          	          	  				case CELL_BOX_MONSTER:
          	          	  				map_set_cell_type(map, x, y+i, CELL_MONSTER);
          	          	  				monster=monster_init(x,y+i);
          	          	  			    game_set_list_of_monsters(monster_list_add(list,monster), game);
          	          	  			    map_set_cell_type(map,x,y+i,CELL_EMPTY);

          	          	  					break;
          	          	  				default:
          	          	  					map_set_cell_type(map,x,y+i,CELL_EMPTY);
          	          	  					break;
          	  				}
            }
      else if (conditions_of_explosion(map, x,y+i) == 3){
    	  window_display_image(sprite_get_explosion(), x* SIZE_BLOC, (y+i)* SIZE_BLOC );
    	  	  	 map_set_cell_type(map,x,y+i,CELL_EMPTY);
      			game_set_list_of_monsters(monster_list_remove(list,x,y+i), game);
            }
      else if ((conditions_of_explosion(map, x,y+i) == 5)&&(player_get_stop_bonus(player,x,y+i)==0)){
                      	  window_display_image(sprite_get_explosion(), (x)* SIZE_BLOC, (y+i)* SIZE_BLOC );
                      	  map_set_cell_type(map,x,y+i,CELL_EMPTY);
                        }
      }
    }
    else if (bomb->etat==BOMB_1_SEC)  {
    	if(player_get_nb_bomb(player)==0)
    	player_inc_nb_bomb(player);

      window_display_image(sprite_get_bomb_1(),x* SIZE_BLOC, y* SIZE_BLOC );
    }
    else if (bomb->etat==BOMB_2_SEC)  {

          window_display_image(sprite_get_bomb_2(),x* SIZE_BLOC, y* SIZE_BLOC );
        }
    else if (bomb->etat==BOMB_3_SEC)  {


          window_display_image(sprite_get_bomb_3(),x* SIZE_BLOC, y* SIZE_BLOC );
        }
    else if (bomb->etat==BOMB_4_SEC)  {

    	if (player_get_nb_bomb(player)==1)
    	    	  player_dec_nb_bomb_case_0(player);

          window_display_image(sprite_get_bomb_4(),x* SIZE_BLOC, y* SIZE_BLOC );
        }
    else if (bomb->etat==END)  {
    	player_set_stop_bomb(player,0); // reset the stop bomb to zero
    	for (i = 0; i < 12; i++) {
    			for (j = 0; j < 12; j++) {
   				player_set_stop_bonus(player,0,i,j);
    							}} // reset all the stop bonus to 0


    	game_set_list_of_bombs(bomb_list_remove(list1), game); // we remove the bomb that exploded from the list

            }
  }
}
// displays the list of bombs.
void  bomb_list_display(struct liste_de_bombes* list,struct game* game){
	struct liste_de_bombes* list_finale = list;
	while(list_finale != NULL){
		display_bomb(list_finale->bomb,game);
		list_finale = list_finale->next_bomb;
	}
}
// returns a different integer for each cell type. this function is useful in bomb display.
int conditions_of_explosion(struct map* map, int x,int y){
	if(!map_is_inside(map, x, y))
			return 0;
	switch (map_get_cell_type(map,x,y)) {
		case CELL_SCENERY :
			return 0;
			break;
		case CELL_DOOR:
			return 0;
			break;
		case CELL_KEY:
			return 0;
			break;
		case CELL_BONUS:
			return 5;
			break;
		case CELL_MONSTER:
			return 3;
			break;
		case CELL_BOX:
			return 2;
			break;
		default:
			return 1;
			break;
		}

	}
// to stop the explosion after a cell scenery in one direction
int scenery_stop_explosion1(struct map* map,int x,int y,int portee){
int j;
int condition=1;
for(j=1;j<portee+1;j++){

	if ((conditions_of_explosion(map,x-j,y)==0))
		condition=0;

}
	return condition;
}

int scenery_stop_explosion2(struct map* map,int x,int y,int portee){
int j;
int condition=1;
for(j=1;j<portee+1;j++){
	if ((conditions_of_explosion(map,x+j,y)==0))
		condition=0;

}
	return condition;
}


int scenery_stop_explosion3(struct map* map,int x,int y,int portee){
int j;
int condition=1;
for(j=1;j<portee+1;j++){

	if ((conditions_of_explosion(map,x,y-j)==0))
		condition=0;

}
	return condition;
}


int scenery_stop_explosion4(struct map* map,int x,int y,int portee){
int j;
int condition=1;
for(j=1;j<portee+1;j++){

	if ((conditions_of_explosion(map,x,y+j)==0))
		condition=0;

}
	return condition;
}



// to avoid the explosion of two cell cases in the same direction.
int box_stop_explosion1(struct map* map,int x,int y,int portee){
int j;
int condition=1;
for(j=1;j<portee+1;j++){

	if ((conditions_of_explosion(map,x-j,y)==2)||(conditions_of_explosion(map,x-j,y)==5))
		condition=0;

}
	return condition;
}

int box_stop_explosion2(struct map* map,int x,int y,int portee){
int j;
int condition=1;
for(j=1;j<portee+1;j++){
	if ((conditions_of_explosion(map,x+j,y)==2)||(conditions_of_explosion(map,x+j,y)==5))
		condition=0;

}
	return condition;
}


int box_stop_explosion3(struct map* map,int x,int y,int portee){
int j;
int condition=1;
for(j=1;j<portee+1;j++){

	if ((conditions_of_explosion(map,x,y-j)==2)||(conditions_of_explosion(map,x,y-j)==5))
		condition=0;

}
	return condition;
}


int box_stop_explosion4(struct map* map,int x,int y,int portee){
int j;
int condition=1;
for(j=1;j<portee+1;j++){

	if ((conditions_of_explosion(map,x,y+j)==2)||(conditions_of_explosion(map,x,y+j)==5))
		condition=0;

}
	return condition;
}

