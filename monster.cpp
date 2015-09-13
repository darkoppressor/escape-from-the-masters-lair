/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "monster.h"
#include "enumerations.h"
#include "random_number_generator.h"
#include "world.h"
#include "ai_keys.h"
#include "render.h"
#include "dungeon.h"
#include "covering_conversions.h"
#include "starting_values.h"

using namespace std;

void Monster::set_inventory(){
    //Add some random gold.
    if(random_range(0,99)<50){
        give_item("gold piece",random_range(1,experience_level*10.0));
    }

    //Add some random bottles of water.
    if(templates.template_races[race].ai_trait_uses_items){
        give_item("bottle of water",random_range(1,2));
    }

    //Give the monster any race-specific items.
    for(int i=0;i<templates.template_races[race].inventory_items.size();i++){
        if(random_range(0,99)<experience_level){
            give_item(templates.template_races[race].inventory_items[i]);

            //If the new item is armor.
            if(inventory[inventory.size()-1].category==ITEM_ARMOR){
                if(equipment_slot_empty(inventory.size()-1,-1)){
                    equip_item(inventory.size()-1,-1);
                }
            }
            //If the new item is a weapon.
            else if(inventory[inventory.size()-1].category==ITEM_WEAPON){
                short slot=-1;

                //If the item is an ammo item.
                if(inventory[inventory.size()-1].launcher!=WEAPON_THROWN){
                    if(equipment_slot_empty(inventory.size()-1,EQUIP_QUIVER)){
                        slot=EQUIP_QUIVER;
                    }
                }
                //If the item is a melee weapon.
                else if(inventory[inventory.size()-1].weapon_category>=WEAPON_SHORT_BLADES && inventory[inventory.size()-1].weapon_category<=WEAPON_STAVES){
                    if(equipment_slot_empty(inventory.size()-1,EQUIP_HOLD_RIGHT)){
                        slot=EQUIP_HOLD_RIGHT;
                    }
                    else if(equipment_slot_empty(inventory.size()-1,EQUIP_HOLD_LEFT)){
                        slot=EQUIP_HOLD_LEFT;
                    }
                }
                //If the item is a ranged weapon.
                else if(inventory[inventory.size()-1].weapon_category>=WEAPON_BOWS && inventory[inventory.size()-1].weapon_category<=WEAPON_SLINGS){
                    if(equipment_slot_empty(inventory.size()-1,EQUIP_LAUNCHER_WEAPON)){
                        slot=EQUIP_LAUNCHER_WEAPON;
                    }
                }

                if(slot!=-1){
                    equip_item(inventory.size()-1,slot);
                }
            }
        }
    }
}

void Monster::set_base_stats(short pass_level){
    //The level on which the monster is spawning.
    short home_level=pass_level+1;

    // From creature: //

    health_max=templates.base_stats.health_max;
    health=health_max;

    mana_max=templates.base_stats.mana_max;
    mana=mana_max;

    base_damage_melee_min=templates.base_stats.base_damage_melee_min;
    base_damage_melee_max=templates.base_stats.base_damage_melee_max;

    base_damage_ranged_min=templates.base_stats.base_damage_ranged_min;
    base_damage_ranged_max=templates.base_stats.base_damage_ranged_max;

    base_damage_thrown_min=templates.base_stats.base_damage_thrown_min;
    base_damage_thrown_max=templates.base_stats.base_damage_thrown_max;

    movement_speed=templates.base_stats.movement_speed;
    next_move=movement_speed;

    carry_capacity=templates.base_stats.carry_capacity;

    for(int i=SKILL_BLADED_WEAPONS;i<SKILL_MAGIC_SUMMONING+1;i++){
        skills[i][SKILL_EXPERIENCE_MAX]+=templates.template_races[race].skills[i][SKILL_EXPERIENCE_MAX];
    }

    //Set the monster's focused skills.

    //If the player has an affiinity for one or more skills, choose those first.

    for(int i=SKILL_BLADED_WEAPONS;i<SKILL_MAGIC_SUMMONING+1;i++){
        //If the player has less than the standard max experience for this skill, he has a natural affinity for it.
        if(skills[i][SKILL_EXPERIENCE_MAX]+templates.template_races[race].skills[i][SKILL_EXPERIENCE_MAX]<STARTING_SKILL_EXPERIENCE_MAX){
            for(int n=0;n<3;n++){
                if(focused_skills[n]==-1){
                    focused_skills[n]=i;
                    break;
                }
            }
        }
    }

    //Then, choose 3 decent skills if they are not already chosen.

    bool armor_chosen=false;
    for(int n=0;n<3;n++){
        if(focused_skills[n]==SKILL_ARMOR){
            armor_chosen=true;
            break;
        }
    }
    if(!armor_chosen){
        for(int n=0;n<3;n++){
            if(focused_skills[n]==-1){
                focused_skills[n]=SKILL_ARMOR;
                break;
            }
        }
    }

    bool speed_chosen=false;
    for(int n=0;n<3;n++){
        if(focused_skills[n]==SKILL_SPEED){
            speed_chosen=true;
            break;
        }
    }
    if(!speed_chosen){
        for(int n=0;n<3;n++){
            if(focused_skills[n]==-1){
                focused_skills[n]=SKILL_SPEED;
                break;
            }
        }
    }

    bool bladed_weapons_chosen=false;
    for(int n=0;n<3;n++){
        if(focused_skills[n]==SKILL_BLADED_WEAPONS){
            bladed_weapons_chosen=true;
            break;
        }
    }
    if(!bladed_weapons_chosen){
        for(int n=0;n<3;n++){
            if(focused_skills[n]==-1){
                focused_skills[n]=SKILL_BLADED_WEAPONS;
                break;
            }
        }
    }

    //Finally, fill out our skills with random ones, if necessary.

    for(int i=0;i<3;){
        if(focused_skills[i]==-1){
            //Choose a random skill.
            ///short random_skill=random_range(SKILL_BLADED_WEAPONS,SKILL_MAGIC_SUMMONING);
            ///Disable some skills.
            short random_skill=random_range(SKILL_BLADED_WEAPONS,SKILL_ARMOR);
            while(random_skill==SKILL_SECURITY || random_skill==SKILL_STEALTH){
                random_skill=random_range(SKILL_BLADED_WEAPONS,SKILL_ARMOR);
            }
            ///

            //If the random skill is different from all of the focused skills.
            if(random_skill!=focused_skills[0] && random_skill!=focused_skills[1] && random_skill!=focused_skills[2]){
                focused_skills[i]=random_skill;
                i++;
            }
        }
        else{
            i++;
        }
    }

    //Once the focused skills are set, apply their initial bonuses to their corresponding skills.
    for(int i=0;i<3;i++){
        gain_skill_experience(focused_skills[i],skills[focused_skills[i]][SKILL_EXPERIENCE_MAX]-skills[focused_skills[i]][SKILL_EXPERIENCE],0,false,false);
    }

    //Level the monster up an appropriate number of times.

    int monster_experience_level_home=(DUNGEON_DEPTH+1)-home_level;

    int lowest_level=monster_experience_level_home-3;

    if(lowest_level<0){
        lowest_level=0;
    }

    int highest_level=monster_experience_level_home+3;

    int levels_to_gain=random_range(lowest_level,highest_level);

    for(int i=0;i<levels_to_gain;i++){
        gain_experience(experience_max-experience);
    }

    //Re-max the monster's health and mana.
    health=health_max;
    mana=mana_max;

    update_class_name();
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

        //Equip launcher weapon.
        else if(ai_keystates[AIK_EQUIP_LAUNCHER_WEAPON]){
            input_inventory=INVENTORY_COMMAND_EQUIP_LAUNCHER_WEAPON;
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

        //Quaff from fountain.
        else if(ai_keystates[AIK_QUAFF_FROM_FOUNTAIN]){
            command_standard=COMMAND_QUAFF_FROM_FOUNTAIN;
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

        if(input_inventory!=INVENTORY_COMMAND_UNEQUIPPED_NOW_DROP_ITEM){
            inventory_input_state=0;
        }

        command_standard=COMMAND_NONE;

        process_move();

        check_this_tile();

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
                    short render_color=color;

                    short temp_color=coverings_to_color(this);
                    if(temp_color!=COLOR_RAINBOW){
                        render_color=temp_color;
                    }

                    font.show((int)(return_absolute_x()-player.camera_x),(int)(return_absolute_y()-player.camera_y),appearance,render_color);

                    if(player.option_healthbars && return_health()<return_health_max()){
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
                        double health_bar_width=((double)((double)return_health()/(double)return_health_max())*100.0)/6.25;
                        render_rectangle((int)(return_absolute_x()-player.camera_x),(int)(return_absolute_y()+TILE_SIZE_Y-3-player.camera_y),health_bar_width,3,1.0,health_bar_color);
                    }

                    if(player.option_dev){
                        ss.clear();ss.str("");ss<<return_health();ss<<"/";ss<<return_health_max();msg=ss.str();
                        font_small.show((int)(return_absolute_x()-player.camera_x),(int)(return_absolute_y()-player.camera_y),msg,COLOR_GREEN);
                    }

                    tile_rendered->at(x)[y]=true;
                }
            }
        }
    }
}
