/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "item.h"
#include "world.h"

using namespace std;

Item::Item(){
    inventory_letter=0;

    equipped=false;

    race=0;

    dye=0;

    stack=1;

    stackable=false;

    spawnable=true;

    writing="";

    plural_name="";

    weight=1;

    monetary_value=1;

    category=ITEM_WEAPON;

    material=MATERIAL_STONE;

    damage_min_melee=1;
    damage_max_melee=1;

    damage_min_thrown=1;
    damage_max_thrown=1;

    damage_min_ranged=1;
    damage_max_ranged=1;

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
