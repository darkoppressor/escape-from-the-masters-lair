/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "monster.h"
#include "enumerations.h"
#include "random_number_generator.h"
#include "world.h"
#include "ai_keys.h"
#include "render.h"

using namespace std;

void Monster::set_inventory(){
    give_item("gold piece",0,'$');

    //Add some random gold.
    give_item("gold piece",random_range(0,10));

    //Add some random bottles of water.
    give_item("bottle of water",random_range(1,2));

    //Add some random healing potions.
    if(random_range(0,1)==1){
        give_item("potion of slight healing");
    }

    for(int i=0;i<templates.template_races[race].inventory_items.size();i++){
        give_item(templates.template_races[race].inventory_items[i]);
    }
}

void Monster::set_base_stats(short pass_level){
    //The level on which the monster is spawning.
    short home_level=pass_level+1;

    // From creature: //

    health_max=random_range(templates.base_stats.health_max/2,templates.base_stats.health_max);
    health=health_max;

    mana_max=random_range(templates.base_stats.mana_max/2,templates.base_stats.mana_max);
    mana=mana_max;

    base_damage_melee_min=random_range(templates.base_stats.base_damage_melee_min/2,templates.base_stats.base_damage_melee_min);
    base_damage_melee_max=random_range(templates.base_stats.base_damage_melee_max/2,templates.base_stats.base_damage_melee_max);

    base_damage_ranged_min=random_range(templates.base_stats.base_damage_ranged_min/2,templates.base_stats.base_damage_ranged_min);
    base_damage_ranged_max=random_range(templates.base_stats.base_damage_ranged_max/2,templates.base_stats.base_damage_ranged_max);

    base_damage_thrown_min=random_range(templates.base_stats.base_damage_thrown_min/2,templates.base_stats.base_damage_thrown_min);
    base_damage_thrown_max=random_range(templates.base_stats.base_damage_thrown_max/2,templates.base_stats.base_damage_thrown_max);

    movement_speed=templates.base_stats.movement_speed;
    next_move=movement_speed;

    carry_capacity=templates.base_stats.carry_capacity;

    //Set the monster's focused skills.
    for(int i=0;i<3;){
        //Choose a random skill.
        short random_skill=random_range(SKILL_BLADED_WEAPONS,SKILL_MAGIC_SUMMONING);

        //If the random skill is different from all of the focused skills.
        if(random_skill!=focused_skills[0] && random_skill!=focused_skills[1] && random_skill!=focused_skills[2]){
            focused_skills[i]=random_skill;
            i++;
        }
    }

    //Once the focused skills are set, apply their initial bonuses to their corresponding skills.
    for(int i=0;i<3;i++){
        gain_skill_experience(focused_skills[i],skills[focused_skills[i]][SKILL_EXPERIENCE_MAX]-skills[focused_skills[i]][SKILL_EXPERIENCE],0,false);
    }

    //Level the monster up an appropriate number of times.

    int lowest_level=home_level-3;
    if(lowest_level<0){
        lowest_level=0;
    }

    int highest_level=home_level+3;

    int levels_to_gain=random_range(lowest_level,highest_level);

    for(int i=0;i<levels_to_gain;i++){
        gain_experience(experience_max-experience);
    }

    //Re-max the monster's health and mana.
    health=health_max;
    mana=mana_max;
}

void Monster::handle_input(){
    if(alive){
        //********************//
        // Standard Commands: //
        //********************//

        //Go down stairs.
        if(ai_keystates[AIK_DOWN_STAIRS]){
            check_command(COMMAND_GO_DOWN_STAIRS);
        }

        //Go up stairs.
        else if(ai_keystates[AIK_DOWN_STAIRS]){
            check_command(COMMAND_GO_UP_STAIRS);
        }

        //Pick up item.
        else if(ai_keystates[AIK_PICK_UP]){
            check_command(COMMAND_PICK_UP);
        }

        //Wait a turn.
        else if(ai_keystates[AIK_WAIT]){
            check_command(COMMAND_WAIT);
        }

        //Search.
        else if(ai_keystates[AIK_SEARCH]){
            check_command(COMMAND_SEARCH);
        }

        //***********************//
        // Directional Commands: //
        //***********************//

        //Open door.
        else if(ai_keystates[AIK_OPEN_DOOR]){
            input_directional=DIRECTIONAL_COMMAND_OPEN_DOOR;
        }

        //Close door.
        else if(ai_keystates[AIK_CLOSE_DOOR]){
            input_directional=DIRECTIONAL_COMMAND_CLOSE_DOOR;
        }

        //Fire item.
        else if(ai_keystates[AIK_FIRE_ITEM]){
            input_directional=DIRECTIONAL_COMMAND_FIRE_ITEM;
        }

        //Move left.
        else if(ai_keystates[AIK_LEFT]){
            move_state=LEFT;

            //If no directional command is waiting on a direction.
            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                input_directional=DIRECTIONAL_COMMAND_MOVE_LEFT;
            }
        }

        //Move up.
        else if(ai_keystates[AIK_UP]){
            move_state=UP;

            //If no directional command is waiting on a direction.
            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                input_directional=DIRECTIONAL_COMMAND_MOVE_UP;
            }
        }

        //Move right.
        else if(ai_keystates[AIK_RIGHT]){
            move_state=RIGHT;

            //If no directional command is waiting on a direction.
            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                input_directional=DIRECTIONAL_COMMAND_MOVE_RIGHT;
            }
        }

        //Move down.
        else if(ai_keystates[AIK_DOWN]){
            move_state=DOWN;

            //If no directional command is waiting on a direction.
            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                input_directional=DIRECTIONAL_COMMAND_MOVE_DOWN;
            }
        }

        //Move left up.
        else if(ai_keystates[AIK_LEFT_UP]){
            move_state=LEFT_UP;

            //If no directional command is waiting on a direction.
            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                input_directional=DIRECTIONAL_COMMAND_MOVE_LEFT_UP;
            }
        }

        //Move right up.
        else if(ai_keystates[AIK_RIGHT_UP]){
            move_state=RIGHT_UP;

            //If no directional command is waiting on a direction.
            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                input_directional=DIRECTIONAL_COMMAND_MOVE_RIGHT_UP;
            }
        }

        //Move right down.
        else if(ai_keystates[AIK_RIGHT_DOWN]){
            move_state=RIGHT_DOWN;

            //If no directional command is waiting on a direction.
            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                input_directional=DIRECTIONAL_COMMAND_MOVE_RIGHT_DOWN;
            }
        }

        //Move left down.
        else if(ai_keystates[AIK_LEFT_DOWN]){
            move_state=LEFT_DOWN;

            //If no directional command is waiting on a direction.
            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                input_directional=DIRECTIONAL_COMMAND_MOVE_LEFT_DOWN;
            }
        }

        //*********************//
        // Inventory Commands: //
        //*********************//

        //Drop item.
        else if(ai_keystates[AIK_DROP_ITEM]){
            input_inventory=INVENTORY_COMMAND_DROP_ITEM;
        }

        //Equip item in right hand.
        else if(ai_keystates[AIK_EQUIP_RIGHT_HAND]){
            input_inventory=INVENTORY_COMMAND_EQUIP_RIGHT_HAND;
        }

        //Equip item in left hand.
        else if(ai_keystates[AIK_EQUIP_LEFT_HAND]){
            input_inventory=INVENTORY_COMMAND_EQUIP_LEFT_HAND;
        }

        //Quiver item.
        else if(ai_keystates[AIK_QUIVER_ITEM]){
            input_inventory=INVENTORY_COMMAND_QUIVER_ITEM;
        }

        //Equip armor.
        else if(ai_keystates[AIK_EQUIP_ARMOR]){
            input_inventory=INVENTORY_COMMAND_EQUIP_ARMOR;
        }

        //Unequip item.
        else if(ai_keystates[AIK_UNEQUIP_ITEM]){
            input_inventory=INVENTORY_COMMAND_UNEQUIP_ITEM;
        }

        //Throw item.
        else if(ai_keystates[AIK_THROW_ITEM]){
            input_inventory=INVENTORY_COMMAND_THROW_ITEM;
        }

        //Quaff item.
        else if(ai_keystates[AIK_QUAFF_ITEM]){
            input_inventory=INVENTORY_COMMAND_QUAFF_ITEM;
        }

        //Use item.
        else if(ai_keystates[AIK_USE_ITEM]){
            input_inventory=INVENTORY_COMMAND_USE_ITEM;
        }

        //Mix items.
        else if(ai_keystates[AIK_MIX_ITEMS]){
            input_inventory=INVENTORY_COMMAND_MIX_ITEMS_1;
        }

        //Read item.
        else if(ai_keystates[AIK_READ_ITEM]){
            input_inventory=INVENTORY_COMMAND_READ_ITEM;
        }

        //If a directional command has been given.
        if(input_directional!=DIRECTIONAL_COMMAND_NONE && move_state!=NONE){
            check_command_directional(move_state);
        }

        //If a inventory command has been given.
        if(input_inventory!=INVENTORY_COMMAND_NONE && inventory_input_state!=0){
            check_command_inventory(inventory_input_state);
        }
    }
}

void Monster::move(){
    if(alive){
        ///update_fov();

        if(input_directional!=DIRECTIONAL_COMMAND_NONE && move_state!=NONE){
            execute_command_directional(move_state);
        }

        else if(input_inventory!=INVENTORY_COMMAND_NONE && inventory_input_state!=0){
            execute_command_inventory(inventory_input_state);
        }

        else if(command_standard!=COMMAND_NONE){
            execute_command(command_standard);
        }

        //Movement has been handled, now clear the move state.
        move_state=NONE;

        //Remember the inventory_input_state in case it is needed for a two part inventory command.
        two_part_inventory_input_state=inventory_input_state;

        inventory_input_state=0;

        command_standard=COMMAND_NONE;

        process_move();

        ///update_fov();
    }
}

void Monster::render(vector< vector<bool> >* tile_rendered){
    if(alive){
        //If nothing has been rendered here yet.
        if(!tile_rendered->at(x)[y]){
            //Render the monster if the monster is in the camera bounds:
            if(return_absolute_x()>=player.camera_x-TILE_SIZE_X && return_absolute_x()<=player.camera_x+player.camera_w && return_absolute_y()>=player.camera_y-TILE_SIZE_Y && return_absolute_y()<=player.camera_y+player.camera_h){
                //If the monster's position is seen.
                if(vector_levels[current_level].fog[x][y]>FOG_FOG || player.option_dev){
                    font.show((int)(return_absolute_x()-player.camera_x),(int)(return_absolute_y()-player.camera_y),appearance,color);

                    if(player.option_healthbars){
                        short health_bar_color=COLOR_GREEN;
                        if(return_health()>=return_health_max()*0.75){
                            health_bar_color=COLOR_GREEN;
                        }
                        else if(return_health()>=return_health_max()*0.50 && return_health()<return_health_max()*0.75){
                            health_bar_color=COLOR_YELLOW;
                        }
                        else if(return_health()>=return_health_max()*0.25 && return_health()<return_health_max()*0.50){
                            health_bar_color=COLOR_ORANGE;
                        }
                        else{
                            health_bar_color=COLOR_RED;
                        }
                        double health_bar_width=((double)((double)health/(double)health_max)*100)/6.25;
                        render_rectangle((int)(return_absolute_x()-player.camera_x),(int)(return_absolute_y()-player.camera_y),health_bar_width,5,0.75,health_bar_color);
                    }

                    if(player.option_dev){
                        ss.clear();ss.str("");ss<<return_health();ss<<"/";ss<<return_health_max();msg=ss.str();
                        font.show((int)(return_absolute_x()-player.camera_x),(int)(return_absolute_y()-player.camera_y),msg,COLOR_GREEN);
                    }

                    tile_rendered->at(x)[y]=true;
                }
            }
        }
    }
}
