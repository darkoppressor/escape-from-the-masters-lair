/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "monster.h"
#include "enumerations.h"
#include "random_number_generator.h"
#include "world.h"
#include "ai_keys.h"

using namespace std;

Monster::Monster(){
}

void Monster::set_inventory(){
    create_money_item();

    //The maximum number of items.
    int max_items=random_range(1,12);

    for(int i=0;i<100;i++){
        //Randomly determine the item category.
        ///For now, equal chance for all categories.
        int random_item_category=random_range(ITEM_WEAPON,ITEM_OTHER);

        //Randomly select an item from the chosen category's template.
        ///For now, equal chance for all items within category.
        int random_item_template=random_range(0,templates.template_items[random_item_category].size()-1);

        //If the item is not spawnable.
        if(!templates.template_items[random_item_category][random_item_template].spawnable){
            //Skip this item.
            continue;
        }

        //Randomly select a stack size.
        int random_item_stack=1;
        //If the item is stackable.
        if(templates.template_items[random_item_category][random_item_template].stackable){
            random_item_stack=random_range(1,8);
        }

        //If the inventory is not full, or the item is money, add the item.
        if(inventory.size()<INVENTORY_MAX_SIZE || templates.template_items[random_item_category][random_item_template].inventory_letter=='$'){
            //Generate the item.
            Item temp_item;

            //Apply the selected template to the item.
            temp_item=templates.template_items[random_item_category][random_item_template];

            //Apply the randomly selected stack size.
            temp_item.stack=random_item_stack;

            //Check to see if there is an identical item already in the inventory.
            inventory_match match_check=check_for_inventory_match(&temp_item);

            //If there is already an identical item in the inventory, and the item is stackable.
            if(match_check.match_found && temp_item.stackable){
                inventory[match_check.inventory_slot].stack+=temp_item.stack;
            }
            //If there is no identical item in the inventory, or the item is not stackable.
            else{
                //Determine an inventory letter for the item.

                //Assign the item an available inventory number.
                temp_item.inventory_letter=assign_inventory_letter();

                //Add the item to the inventory items vector.
                inventory.push_back(temp_item);

                //Assign an identifier to the item.
                inventory[inventory.size()-1].assign_identifier();

                //Assign an owner identifier to the item.
                inventory[inventory.size()-1].owner=identifier;
            }
        }

        //If the number of items generated has exceeded the maximum.
        if(inventory.size()>max_items || inventory.size()>=INVENTORY_MAX_SIZE){
            break;
        }
    }
}

void Monster::set_base_stats(short pass_level){
    //The level on which the monster is spawning.
    short home_level=pass_level+1;

    // From creature: //

    health_max=random_range(templates.base_stats.health_max/4,templates.base_stats.health_max/2);
    health=health_max;

    mana_max=random_range(templates.base_stats.mana_max/4,templates.base_stats.mana_max/2);
    mana=mana_max;

    base_damage_melee_min=random_range(templates.base_stats.base_damage_melee_min/4,templates.base_stats.base_damage_melee_min/2);
    base_damage_melee_max=random_range(templates.base_stats.base_damage_melee_max/4,templates.base_stats.base_damage_melee_max/2);

    base_damage_ranged_min=random_range(templates.base_stats.base_damage_ranged_min/4,templates.base_stats.base_damage_ranged_min/2);
    base_damage_ranged_max=random_range(templates.base_stats.base_damage_ranged_max/4,templates.base_stats.base_damage_ranged_max/2);

    base_damage_thrown_min=random_range(templates.base_stats.base_damage_thrown_min/4,templates.base_stats.base_damage_thrown_min/2);
    base_damage_thrown_max=random_range(templates.base_stats.base_damage_thrown_max/4,templates.base_stats.base_damage_thrown_max/2);

    ///Not positive how I'll handle movement_speed yet.
    movement_speed=75;
    next_move=movement_speed;

    //Level the monster up an appropriate number of times.
    ///
}

void Monster::handle_ai(){
    if(alive){
        //Reset AI keystates.
        for(short i=0;i<256;i++){
            ai_keystates[i]=false;
        }

        //Select a random direction to move.
        int random=random_range(AIK_LEFT,AIK_LEFT_DOWN);
        ///ai_keystates[random]=true;

        short target_x=0;
        short target_y=0;

        target_x=player.x;
        target_y=player.y;

        ///target_x=vector_levels[current_level].items[0].x;
        ///target_y=vector_levels[current_level].items[0].y;

        ///if(inventory.size()==0){
            /**if(x!=target_x || y!=target_y){
                if(target_x<x && target_y<y){
                    ai_keystates[AIK_LEFT_UP]=true;
                }
                else if(target_x>x && target_y<y){
                    ai_keystates[AIK_RIGHT_UP]=true;
                }
                else if(target_x>x && target_y>y){
                    ai_keystates[AIK_RIGHT_DOWN]=true;
                }
                else if(target_x<x && target_y>y){
                    ai_keystates[AIK_LEFT_DOWN]=true;
                }
                else if(target_x<x){
                    ai_keystates[AIK_LEFT]=true;
                }
                else if(target_y<y){
                    ai_keystates[AIK_UP]=true;
                }
                else if(target_x>x){
                    ai_keystates[AIK_RIGHT]=true;
                }
                else if(target_y>y){
                    ai_keystates[AIK_DOWN]=true;
                }
            }
            /**else{
                ai_keystates[AIK_PICK_UP]=true;
            }*/
        ///}
        /**else{
            ai_keystates[AIK_DROP_ITEM]=true;
            inventory_input_state=inventory[0].inventory_letter;
        }*/
    }
}

void Monster::handle_input(){
    if(alive){
        //********************//
        // Standard Commands: //
        //********************//

        //Toggle light item.
        if(ai_keystates[AIK_TOGGLE_LIGHT]){
            check_command(COMMAND_TOGGLE_LIGHT);
        }

        //Go down stairs.
        else if(ai_keystates[AIK_DOWN_STAIRS]){
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

        //Remember the inventory_input_state in case it is needed for a directional inventory command.
        directional_inventory_input_state=inventory_input_state;

        inventory_input_state=0;

        command_standard=COMMAND_NONE;

        process_turn();
    }
}

void Monster::render(vector< vector<bool> >* tile_rendered){
    if(alive){
        //If nothing has been rendered here yet.
        if(!tile_rendered->at(x)[y]){
            //Render the monster if the monster is in the camera bounds:
            if(return_absolute_x()>=player.camera_x-TILE_SIZE && return_absolute_x()<=player.camera_x+player.camera_w && return_absolute_y()>=player.camera_y-TILE_SIZE && return_absolute_y()<=player.camera_y+player.camera_h){
                //If the monster's position is seen.
                if(vector_levels[current_level].fog[x][y]>FOG_FOG || player.option_dev){
                    font.show((int)(return_absolute_x()-player.camera_x),(int)(return_absolute_y()-player.camera_y),appearance,color);

                    tile_rendered->at(x)[y]=true;
                }
            }
        }
    }
}
