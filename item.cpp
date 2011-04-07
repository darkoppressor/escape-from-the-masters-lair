/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "item.h"
#include "world.h"
#include "message_log.h"
#include "material_properties.h"
#include "age_limits.h"
#include "covering_conversions.h"

using namespace std;

Item::Item(){
    owner=0;

    inventory_letter=0;

    equipped=false;

    race=0;

    dye=0;

    move_direction=NONE;

    momentum=0;

    stack=1;

    stackable=false;

    spawnable=true;

    writing=" ";

    plural_name=" ";

    monetary_value=1;

    category=ITEM_WEAPON;

    material=MATERIAL_STONE;

    damage_min_melee=1;
    damage_max_melee=1;

    damage_min_thrown=1;
    damage_max_thrown=1;

    damage_min_ranged=0;
    damage_max_ranged=0;

    //Randomly choose a facing direction.
    int random=random_range(FOV_EAST,FOV_SOUTHEAST);
    facing=(fov_direction_type)random;
    fov_radius=LIGHT_NONE;
    beam=false;
    fov_angle=90.0f;
    light_on=false;
    source_data.permanent=false;

    age=0;

    //Weapon-specific//

    governing_skill_weapon=SKILL_BLADED_WEAPONS;

    weapon_category=WEAPON_SHORT_BLADES;

    launcher=WEAPON_THROWN;

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

    fuel=0;

    fuel_max=0;

    is_skeleton=false;
}

void Item::setup(){
    //If the item has more than 0 fuel_max.
    if(fuel_max>0){
        //Determine the item's fuel.
        fuel=random_range(1,fuel_max);
    }
}

bool Item::possesses_effect(short effect){
    for(int i=0;i<effects.size();i++){
        if(effects[i]==effect){
            return true;
        }
    }

    return false;
}

/**Creature* Item::determine_owner_address(){
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
}*/

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

        //Check for the player.
        if(player.alive && check_x==player.x && check_y==player.y){
            //Attack the player.
            attack(&player);
            combat_occurred=true;
        }

        //If an attack has not yet occurred.
        if(!combat_occurred){
            //Check for a monster.
            for(int i=0;i<vector_levels[current_level].monsters.size();i++){
                if(vector_levels[current_level].monsters[i].alive && check_x==vector_levels[current_level].monsters[i].x && check_y==vector_levels[current_level].monsters[i].y){
                    //Attack the monster.
                    attack(&vector_levels[current_level].monsters[i]);
                    combat_occurred=true;
                    break;
                }
            }
        }

        //Move the creature.
        x=check_x;
        y=check_y;

        //If no attack occurred.
        if(!combat_occurred){
            //The item loses one momentum.
            momentum--;
        }
        //If an attack occurred.
        else{
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

        //If the item is no longer moving.
        if(momentum==0){
            //It doesn't matter what caused the item to move, since it has stopped moving.
            clear_owner_data_all();
        }
    }

    //If the item is not moving.
    //This check occurs whether the item just stopped moving or has already been stationary.
    if(momentum==0){
        check_this_tile();
    }
}

void Item::process_turn(){
    for(int i=0;i<coverings.size();i++){
        if(!coverings[i].process_turn()){
            coverings.erase(coverings.begin()+i);
            i--;
        }
    }

    //If the item has a light radius.
    if(fov_radius!=LIGHT_NONE){
        //If the light item is on.
        if(light_on){
            //Decrement the item's fuel.
            fuel--;

            //If the item is out of fuel.
            if(fuel<=0){
                //Ensure the item has exactly 0 fuel.
                fuel=0;

                //Turn off the item's light.
                light_on=false;

                //Set up a light turning off message.

                string message="";

                message="The ";
                message+=return_full_name(1);
                message+=" has run out of fuel.";

                update_text_log(message.c_str(),true);
            }
        }
    }

    //If the item is a corpse.
    if(is_corpse){
        //Age the item.
        age++;

        //If the corpse reaches its decay age.
        if(age>=AGE_CORPSE_DECAY){
            //The corpse becomes a skeleton.

            //Create a temporary skeleton item.
            Item temp_item=templates.template_items[ITEM_OTHER][0];

            double temp_item_size=0.0;

            //Set the item's info to the corpse's race.
            string temp_name=templates.template_races[race].name;
            temp_name+=" ";
            temp_item.name=temp_name+temp_item.name;

            temp_item.weight=templates.template_races[race].weight;

            temp_item_size=(temp_item.weight*2)/specific_gravities[temp_item.material];

            temp_item.monetary_value=(temp_item.weight*values[temp_item.material])/2.0;
            temp_item.monetary_value+=(temp_item.effects.size()*values[temp_item.material])/4.0;
            if(temp_item.category==ITEM_WEAPON){
                temp_item.monetary_value+=(6*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_ARMOR){
                temp_item.monetary_value+=(5*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_FOOD){
                temp_item.monetary_value+=(1*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_DRINK){
                temp_item.monetary_value+=(3*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_SCROLL){
                temp_item.monetary_value+=(4*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_BOOK){
                temp_item.monetary_value+=(4*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_CONTAINER){
                temp_item.monetary_value+=(2*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_OTHER){
                temp_item.monetary_value+=(1*values[temp_item.material])/4.0;
            }

            temp_item.damage_min_melee=1;
            temp_item.damage_max_melee=(temp_item_size*temp_item.weight)/6.0;

            if(temp_item.damage_min_melee<1){
                temp_item.damage_min_melee=1;
            }
            if(temp_item.damage_max_melee<1){
                temp_item.damage_max_melee=1;
            }

            temp_item.damage_min_thrown=1;
            temp_item.damage_max_thrown=temp_item.damage_max_melee/2.0;

            if(temp_item.damage_min_thrown<1){
                temp_item.damage_min_thrown=1;
            }
            if(temp_item.damage_max_thrown<1){
                temp_item.damage_max_thrown=1;
            }

            name=temp_item.name;

            color=temp_item.color;

            material=temp_item.material;

            weight=temp_item.weight;

            monetary_value=temp_item.monetary_value;

            damage_min_melee=temp_item.damage_min_melee;
            damage_max_melee=temp_item.damage_max_melee;

            damage_min_thrown=temp_item.damage_min_thrown;
            damage_max_thrown=temp_item.damage_max_thrown;

            is_corpse=false;
            is_skeleton=true;
        }
    }
}

void Item::check_this_tile(){
    //If the tile the item is on is liquid.
    if(vector_levels[current_level].tiles[x][y].type==TILE_TYPE_LIQUID){
        //If the liquid is water and not covered in ice.
        if(vector_levels[current_level].tiles[x][y].material==MATERIAL_WATER && !vector_levels[current_level].tiles[x][y].has_covering(COVERING_ICE)){
            add_covering(COVERING_WATER);
        }
        //If the liquid is lava.
        else if(vector_levels[current_level].tiles[x][y].material==MATERIAL_LAVA){
            alive=false;
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
        if(return_absolute_x()>=player.camera_x-TILE_SIZE_X && return_absolute_x()<=player.camera_x+player.camera_w && return_absolute_y()>=player.camera_y-TILE_SIZE_Y && return_absolute_y()<=player.camera_y+player.camera_h){
            //If the item's position is seen.
            if(vector_levels[current_level].fog[x][y]>FOG_FOG || player.option_dev){
                short render_color=color;

                //If the item is dyed.
                if(dye!=0){
                    render_color=dye;
                }

                short temp_color=coverings_to_color(this);
                if(temp_color!=COLOR_RAINBOW){
                    render_color=temp_color;
                }

                font.show((int)(return_absolute_x()-player.camera_x),(int)(return_absolute_y()-player.camera_y),appearance,render_color);

                tile_rendered->at(x)[y]=true;
            }
        }
    }
}
