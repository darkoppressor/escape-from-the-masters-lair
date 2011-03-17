/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "item.h"
#include "world.h"

using namespace std;

Item::Item(){
    owner=0;

    inventory_letter=0;

    equipped=false;

    race=0;

    dye=0;

    move_direction=NONE;

    momentum=0;

    movement_cause=ITEM_MOVEMENT_CAUSE_NONE;

    damage=0;

    stack=1;

    stackable=false;

    spawnable=true;

    writing="";

    plural_name="";

    monetary_value=1;

    category=ITEM_WEAPON;

    material=MATERIAL_STONE;

    damage_min_melee=1;
    damage_max_melee=1;

    damage_min_thrown=1;
    damage_max_thrown=1;

    damage_min_ranged=1;
    damage_max_ranged=1;

    //Randomly choose a facing direction.
    int random=random_range(FOV_EAST,FOV_SOUTHEAST);
    facing=(fov_direction_type)random;
    fov_radius=LIGHT_NONE;
    beam=false;
    fov_angle=90.0f;
    light_on=false;
    source_data.permanent=false;

    //Weapon-specific//

    governing_skill_weapon=SKILL_BLADED_WEAPONS;

    weapon_category=WEAPON_SHORT_BLADES;

    //Armor-specific//

    armor_category=ARMOR_CHEST;

    defense=0;

    //Food-specific//

    is_corpse=false;

    //Drink-specific//

    thirst_quenched=0;

    //Scroll-specific//

    //Book-specific//

    //Container-specific//

    //Other-specific//
}

Creature* Item::determine_owner_address(){
    Creature* ptr_owner;

    //If the player is the owner.
    if(owner!=0 && owner==player.identifier){
        ptr_owner=&player;

        return ptr_owner;
    }
    //If the player is not the owner.
    else{
        //Look through all of the levels.
        for(int i=0;i<vector_levels.size()-1;i++){
            //Look through all of the monsters on this level.
            for(int n=0;n<vector_levels[i].monsters.size()-1;n++){
                //If this monster is the owner.
                if(owner!=0 && owner==vector_levels[i].monsters[n].identifier){
                    ptr_owner=&vector_levels[i].monsters[n];

                    return ptr_owner;
                }
            }
        }
    }

    ///fprintf(stderr,"Error: Item::determine_owner_address() returned a NULL pointer.\n");

    return NULL;
}

void Item::assign_identifier(){
    identifier=game.assign_identifier(OBJECT_ITEM);
}

void Item::return_identifier(){
    game.return_identifier(OBJECT_ITEM,identifier);
    identifier=0;
}

bool Item::check_movement(short check_x,short check_y){
    //Keep the creature in the level map's bounds.
    if(check_x<0 || check_x>vector_levels[current_level].level_x-1){
        return false;
    }
    if(check_y<0 || check_y>vector_levels[current_level].level_y-1){
        return false;
    }

    switch(vector_levels[current_level].tiles[check_x][check_y].type){
        //If no special tiles are encountered.
        default:
            return true;
            break;

        case TILE_TYPE_WALL: case TILE_TYPE_SOLID: case TILE_TYPE_DOOR_CLOSED: case TILE_TYPE_SECRET_DOOR:
            return false;
            break;
    }
}

void Item::execute_movement(short check_x,short check_y){
    bool clear_for_move=false;

    switch(vector_levels[current_level].tiles[check_x][check_y].type){
        //If no special tiles are encountered.
        default:
            clear_for_move=true;
            break;

        case TILE_TYPE_WALL: case TILE_TYPE_SOLID: case TILE_TYPE_DOOR_CLOSED: case TILE_TYPE_SECRET_DOOR:
            ///
            break;
    }

    //If the tile is clear for movement, check for something to attack before moving to it.
    if(clear_for_move){
        bool combat_occurred=false;

        //Setup a pointer to the item's owner.
        Creature* ptr_owner=determine_owner_address();

        //If the item has an owner.
        if(ptr_owner!=NULL){
            //Check for the player.
            if(player.alive && check_x==player.x && check_y==player.y){
                //Attack the player.
                ptr_owner->attack_thrown(&player,this);
                combat_occurred=true;
            }
            //Check for a monster.
            for(int i=0;i<vector_levels[current_level].monsters.size();i++){
                if(vector_levels[current_level].monsters[i].alive && check_x==vector_levels[current_level].monsters[i].x && check_y==vector_levels[current_level].monsters[i].y){
                    //Attack the monster.
                    ptr_owner->attack_thrown(&vector_levels[current_level].monsters[i],this);
                    combat_occurred=true;
                    break;
                }
            }
        }

        //If no attack occurred, move the creature.
        if(!combat_occurred){
            x=check_x;
            y=check_y;

            //The item loses one momentum.
            momentum--;
        }
        //If an attack occurred.
        else{
            x=check_x;
            y=check_y;

            //The item loses all momentum and thus becomes still.
            momentum=0;
        }
    }
}

void Item::move(){
    //If the item has momentum.
    if(momentum>0){
        if(move_direction==LEFT){
            //If the target tile can be moved into.
            if(check_movement(x-1,y)){
                facing=FOV_WEST;

                execute_movement(x-1,y);
            }
            //If the target tile cannot be moved into.
            else{
                //The item loses all momentum and thus becomes still.
                momentum=0;
            }
        }
        else if(move_direction==UP){
            //If the target tile can be moved into.
            if(check_movement(x,y-1)){
                facing=FOV_NORTH;

                execute_movement(x,y-1);
            }
            //If the target tile cannot be moved into.
            else{
                //The item loses all momentum and thus becomes still.
                momentum=0;
            }
        }
        else if(move_direction==RIGHT){
            //If the target tile can be moved into.
            if(check_movement(x+1,y)){
                facing=FOV_EAST;

                execute_movement(x+1,y);
            }
            //If the target tile cannot be moved into.
            else{
                //The item loses all momentum and thus becomes still.
                momentum=0;
            }
        }
        else if(move_direction==DOWN){
            //If the target tile can be moved into.
            if(check_movement(x,y+1)){
                facing=FOV_SOUTH;

                execute_movement(x,y+1);
            }
            //If the target tile cannot be moved into.
            else{
                //The item loses all momentum and thus becomes still.
                momentum=0;
            }
        }
        else if(move_direction==LEFT_UP){
            //If the target tile can be moved into.
            if(check_movement(x-1,y-1)){
                facing=FOV_NORTHWEST;

                execute_movement(x-1,y-1);
            }
            //If the target tile cannot be moved into.
            else{
                //The item loses all momentum and thus becomes still.
                momentum=0;
            }
        }
        else if(move_direction==RIGHT_UP){
            //If the target tile can be moved into.
            if(check_movement(x+1,y-1)){
                facing=FOV_NORTHEAST;

                execute_movement(x+1,y-1);
            }
            //If the target tile cannot be moved into.
            else{
                //The item loses all momentum and thus becomes still.
                momentum=0;
            }
        }
        else if(move_direction==RIGHT_DOWN){
            //If the target tile can be moved into.
            if(check_movement(x+1,y+1)){
                facing=FOV_SOUTHEAST;

                execute_movement(x+1,y+1);
            }
            //If the target tile cannot be moved into.
            else{
                //The item loses all momentum and thus becomes still.
                momentum=0;
            }
        }
        else if(move_direction==LEFT_DOWN){
            //If the target tile can be moved into.
            if(check_movement(x-1,y+1)){
                facing=FOV_SOUTHWEST;

                execute_movement(x-1,y+1);
            }
            //If the target tile cannot be moved into.
            else{
                //The item loses all momentum and thus becomes still.
                momentum=0;
            }
        }
    }
}

string Item::return_full_name(int override_stack){
    //If the stack number is not being overridden.
    if(override_stack==-1){
        override_stack=stack;
    }

    string full_name="";

    if(override_stack==1){
        full_name=name;
    }
    else{
        //Create a temporary char array to hold override_stack.
        char temp_char[10];

        //Set temp_char to hold the value stored in override_stack.
        sprintf(temp_char,"%d",override_stack);

        full_name=temp_char;
        full_name+=" ";
        full_name+=plural_name;
    }

    return full_name;
}

void Item::render(vector< vector<bool> >* tile_rendered){
    //If nothing has been rendered here yet.
    if(!tile_rendered->at(x)[y]){
        //If the item is in camera bounds.
        if(return_absolute_x()>=player.camera_x-TILE_SIZE && return_absolute_x()<=player.camera_x+player.camera_w && return_absolute_y()>=player.camera_y-TILE_SIZE && return_absolute_y()<=player.camera_y+player.camera_h){
            //If the item's position is seen.
            if(vector_levels[current_level].fog[x][y]>FOG_FOG || player.option_dev){
                short render_color=color;

                //If the item is dyed.
                if(dye!=0){
                    render_color=dye;
                }

                font.show((int)(return_absolute_x()-player.camera_x),(int)(return_absolute_y()-player.camera_y),appearance,render_color);

                tile_rendered->at(x)[y]=true;
            }
        }
    }
}
