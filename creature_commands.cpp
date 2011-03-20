/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"
#include "world.h"
#include "message_log.h"
#include "random_chance.h"

using namespace std;

///********************///
/// Standard Commands: ///
///********************///

void Creature::check_command(short command){
    if(command==COMMAND_GO_DOWN_STAIRS){
        //
        //If creature is located on: Down stairs.
        //
        if(vector_levels[current_level].tiles[x][y].type==TILE_TYPE_DOWN_STAIRS){
            command_standard=command;
            initiate_move=true;
        }
        //
        //If creature is located on: Anything else.
        //
        else{
            //No down stairs here.
            update_text_log(string_down_stairs_not_here.c_str(),is_player);
        }
    }

    else if(command==COMMAND_GO_UP_STAIRS){
        //
        //If creature is located on: Up stairs.
        //
        if(vector_levels[current_level].tiles[x][y].type==TILE_TYPE_UP_STAIRS){
            command_standard=command;
            initiate_move=true;
        }
        //
        //If creature is located on: Anything else.
        //
        else{
            //No up stairs here.
            update_text_log(string_up_stairs_not_here.c_str(),is_player);
        }
    }

    else if(command==COMMAND_PICK_UP){
        //
        //Check all items in the dungeon for items located at the creature's position.
        //

        //Set to true if at least one item is found on the current tile.
        bool item_here=false;

        //Search through all the items on this dungeon level.
        for(int i=0;i<vector_levels[current_level].items.size();i++){
            //We've found an item at the creature's position.
            if(vector_levels[current_level].items[i].x==x && vector_levels[current_level].items[i].y==y){
                //At least one item has been found on the current tile.
                item_here=true;

                //If the inventory is not full, or the item is money.
                if(inventory.size()<INVENTORY_MAX_SIZE || vector_levels[current_level].items[i].inventory_letter=='$'){
                    //Due to the above if() statements, there is at least one item that can definitely be picked up on this tile.
                    command_standard=command;
                    initiate_move=true;

                    //There is no need to continue checking, so we return from this function.
                    return;
                }
            }
        }

        //If the preceding for() loop was cleared through without returning,
        //we can't pick up any items.

        //There are no items on the current tile.
        if(!item_here){
            update_text_log(string_no_item_this_tile.c_str(),is_player);

            return;
        }

        //Inventory is full.
        if(inventory.size()>=INVENTORY_MAX_SIZE){
            update_text_log(string_inventory_full.c_str(),is_player);

            return;
        }
    }

    else if(command==COMMAND_WAIT){
        //If the creature can wait.
        ///This if() may be unnecessary, as there may be no case in which a creature cannot wait.
        if(true){
            command_standard=command;
            initiate_move=true;
        }
        //If the creature cannot wait.
        else{
            ///Handle this, if necessary.
        }
    }

    else if(command==COMMAND_SEARCH){
        //If the creature can search.
        ///This if() may be unnecessary, as there may be no case in which a creature cannot search.
        if(true){
            command_standard=command;
            initiate_move=true;
        }
        //If the creature cannot search.
        else{
            ///Handle this, if necessary.
        }
    }
}

void Creature::execute_command(short command){
    //************************************//
    // Command: WARP TO DOWN STAIR (TEST) //
    //************************************//
    if(command==-2){
        for(int int_x=0;int_x<vector_levels[current_level].level_x;int_x++){
            for(int int_y=0;int_y<vector_levels[current_level].level_y;int_y++){
                if(vector_levels[current_level].tiles[int_x][int_y].type==TILE_TYPE_DOWN_STAIRS){
                    x=vector_levels[current_level].tiles[int_x][int_y].x;
                    y=vector_levels[current_level].tiles[int_x][int_y].y;
                }
            }
        }
    }

    //******************************//
    // Command: DESTROY WALL (TEST) //
    //******************************//
    else if(command==-1){
        //
        if(x>0){
            vector_levels[current_level].tiles[x-1][y].type=TILE_TYPE_FLOOR;
        }
    }

    else if(command==COMMAND_GO_DOWN_STAIRS){
        //If the creature is the player.
        if(is_player){
            game.change_level(DOWN);

            update_text_log("You make your way down the stairs.",true);
        }
        //If the creature is a monster.
        else{
            ///Handle this.
        }
    }

    else if(command==COMMAND_GO_UP_STAIRS){
        //If the creature is the player.
        if(is_player){
            game.change_level(UP);

            update_text_log("You make your way up the stairs.",true);
        }
        //If the creature is a monster.
        else{
            ///Handle this.
        }
    }

    else if(command==COMMAND_PICK_UP){
        //
        //Check all items in the dungeon for items located at the creature's position.
        //Any items at the creature's position will be handled accordingly.
        //
        //Search through all the items on this dungeon level.
        for(int i=0;i<vector_levels[current_level].items.size();i++){
            //We've found an item at the creature's position.
            if(vector_levels[current_level].items[i].x==x && vector_levels[current_level].items[i].y==y){
                //If the inventory is not full, or the item is money, pick up the item.
                if(inventory.size()<INVENTORY_MAX_SIZE || vector_levels[current_level].items[i].inventory_letter=='$'){
                    //The inventory index of the item being picked up.
                    int picked_up_item_index=-1;

                    //Check to see if there is an identical item already in the inventory.
                    inventory_match match_check=check_for_inventory_match(&vector_levels[current_level].items[i]);

                    //If there is already an identical item in the inventory, and the item is stackable.
                    if(match_check.match_found && vector_levels[current_level].items[i].stackable){
                        inventory[match_check.inventory_slot].stack+=vector_levels[current_level].items[i].stack;

                        picked_up_item_index=match_check.inventory_slot;
                    }
                    //If there is no identical item in the inventory, or the item is not stackable.
                    else{
                        //Determine an inventory letter for the item.

                        //If the item already has an inventory letter, this means the player assigned one to it, and then dropped it.
                        //So we only do this check for the player.
                        if(is_player && vector_levels[current_level].items[i].inventory_letter!=0){
                            //If the item's inventory letter is available.
                            if(check_inventory_letter_availability(vector_levels[current_level].items[i].inventory_letter)){
                                //Remove the item's inventory letter from the inventory letters list.
                                assign_inventory_letter(vector_levels[current_level].items[i].inventory_letter);
                            }
                            //If the item's inventory letter is unavailable.
                            else{
                                //Assign the item an available inventory number.
                                vector_levels[current_level].items[i].inventory_letter=assign_inventory_letter();
                            }
                        }
                        //If the creature is not the player, or the item has no inventory letter already assigned.
                        else{
                            //Assign the item an available inventory number.
                            vector_levels[current_level].items[i].inventory_letter=assign_inventory_letter();
                        }

                        //Add the item to the inventory items vector.
                        inventory.push_back(vector_levels[current_level].items[i]);

                        //Assign an identifier to the item.
                        inventory[inventory.size()-1].assign_identifier();

                        //Assign an owner identifier to the new item.
                        inventory[inventory.size()-1].owner=identifier;

                        picked_up_item_index=inventory.size()-1;
                    }

                    //Setup a pick up message.

                    string grab_item="";

                    //If the creature is the player.
                    if(is_player){
                        grab_item="You pick up the ";
                    }
                    else{
                        grab_item="The ";
                        grab_item+=return_full_name();
                        grab_item+=" picks up the ";
                    }

                    grab_item+=vector_levels[current_level].items[i].return_full_name();

                    if(is_player){
                        grab_item+=" - ";
                        grab_item+=inventory[picked_up_item_index].inventory_letter;
                    }

                    grab_item+=".";

                    update_text_log(grab_item.c_str(),true);

                    //Return the item's identifier.
                    vector_levels[current_level].items[i].return_identifier();

                    //Remove the item from the dungeon items vector.
                    vector_levels[current_level].items.erase(vector_levels[current_level].items.begin()+i);

                    //For now, we break out of the for() loop,
                    //meaning we only pick up one item.
                    break;
                }
            }
        }
    }

    else if(command==COMMAND_WAIT){
        ///Handle waiting.

        update_text_log("You wait around.",true);
    }

    else if(command==COMMAND_SEARCH){
        search();

        update_text_log("You search the area.",true);
    }
}

///***********************///
/// Directional commands: ///
///***********************///

Creature::directional_check_coordinates Creature::determine_direction(short direction){
    directional_check_coordinates coords;

    if(direction==LEFT){
        coords.check_x=x-1;
        coords.check_y=y;
    }
    else if(direction==UP){
        coords.check_x=x;
        coords.check_y=y-1;
    }
    else if(direction==RIGHT){
        coords.check_x=x+1;
        coords.check_y=y;
    }
    else if(direction==DOWN){
        coords.check_x=x;
        coords.check_y=y+1;
    }
    else if(direction==LEFT_UP){
        coords.check_x=x-1;
        coords.check_y=y-1;
    }
    else if(direction==RIGHT_UP){
        coords.check_x=x+1;
        coords.check_y=y-1;
    }
    else if(direction==RIGHT_DOWN){
        coords.check_x=x+1;
        coords.check_y=y+1;
    }
    else if(direction==LEFT_DOWN){
        coords.check_x=x-1;
        coords.check_y=y+1;
    }

    return coords;
}

int Creature::determine_momentum(double item_weight,bool fired){
    //Base momentum is 6.
    double momentum=6.0;

    //An item cannot have a weight of 0 for throwing calculations.
    if(item_weight==0.0){
        item_weight=0.1;
    }

    double firing_bonus=1.0;

    if(fired){
        firing_bonus=2.0;
    }

    double momentum_bonus=0.0;
    double momentum_penalty=0.0;

    //If the creature's strength is greater than the item's throwing adjusted weight.
    if((double)return_attribute_strength()>=item_weight/4.0){
        momentum_bonus=(double)return_attribute_strength()/(item_weight*1.5);
    }
    //If the creature's strength is less than the item's throwing adjusted weight.
    else{
        momentum_penalty=item_weight/(double)return_attribute_strength();
    }

    momentum+=momentum_bonus*firing_bonus;
    momentum-=momentum_penalty/firing_bonus;

    //Minimum momentum is 0.
    if(momentum<0.0){
        momentum=0.0;
    }

    return (int)momentum;
}

void Creature::check_command_directional(short direction){
    directional_check_coordinates coords=determine_direction(direction);

    short command=input_directional;

    //Handle command target being outside level bounds.
    if(coords.check_x<0 || coords.check_x>vector_levels[current_level].level_x-1 || coords.check_y<0 || coords.check_y>vector_levels[current_level].level_y-1){
        if(command==DIRECTIONAL_COMMAND_OPEN_DOOR || command==DIRECTIONAL_COMMAND_CLOSE_DOOR){
            update_text_log(string_door_not_there.c_str(),is_player);

            //No directional command will be executed.
            input_directional=0;
            move_state=0;

            return;
        }
        else if(command>=DIRECTIONAL_COMMAND_MOVE_LEFT && command<=DIRECTIONAL_COMMAND_MOVE_LEFT_DOWN){

            //No directional command will be executed.
            input_directional=0;
            move_state=0;

            return;
        }
    }

    if(command==DIRECTIONAL_COMMAND_OPEN_DOOR){
        //
        //Target: Closed door.
        //
        if(vector_levels[current_level].tiles[coords.check_x][coords.check_y].type==TILE_TYPE_DOOR_CLOSED){
            initiate_move=true;
        }
        //
        //Target: Open door.
        //
        else if(vector_levels[current_level].tiles[coords.check_x][coords.check_y].type==TILE_TYPE_DOOR_OPEN){
            //Door already open.
            update_text_log(string_door_already_open.c_str(),is_player);

            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
        //
        //Target: Anything else.
        //
        else{
            //No door there.
            update_text_log(string_door_not_there.c_str(),is_player);

            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
    }

    else if(command==DIRECTIONAL_COMMAND_CLOSE_DOOR){
        //
        //Target: Open door.
        //
        if(vector_levels[current_level].tiles[coords.check_x][coords.check_y].type==TILE_TYPE_DOOR_OPEN){
            initiate_move=true;
        }
        //
        //Target: Closed door.
        //
        else if(vector_levels[current_level].tiles[coords.check_x][coords.check_y].type==TILE_TYPE_DOOR_CLOSED){
            //Door already closed.
            update_text_log(string_door_already_closed.c_str(),is_player);

            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
        //
        //Target: Anything else.
        //
        else{
            //No door there.
            update_text_log(string_door_not_there.c_str(),is_player);

            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
    }

    else if(command==DIRECTIONAL_COMMAND_THROW_ITEM){
        //If the item can be thrown.
        if(true){
            initiate_move=true;
        }
        //If the item cannot be thrown.
        else{
            ///update_text_log("",is_player);

            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
    }

    else if(command==DIRECTIONAL_COMMAND_FIRE_ITEM){
        //If an item is quivered.
        if(!equipment_slot_empty(-1,EQUIP_QUIVER) &&
           //And if a launcher is being wielded in either the right or left hand.
           ((!equipment_slot_empty(-1,EQUIP_HOLD_RIGHT) && inventory[index_of_item_in_slot(EQUIP_HOLD_RIGHT)].category==ITEM_WEAPON && inventory[index_of_item_in_slot(EQUIP_HOLD_RIGHT)].governing_skill_weapon==SKILL_LAUNCHER_WEAPONS) ||
            (!equipment_slot_empty(-1,EQUIP_HOLD_LEFT) && inventory[index_of_item_in_slot(EQUIP_HOLD_LEFT)].category==ITEM_WEAPON && inventory[index_of_item_in_slot(EQUIP_HOLD_LEFT)].governing_skill_weapon==SKILL_LAUNCHER_WEAPONS))){
            initiate_move=true;
        }
        //If no item is quivered.
        else if(equipment_slot_empty(-1,EQUIP_QUIVER)){
            update_text_log("You have nothing in your quiver.",is_player);

            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
        //If no launcher is being wielded.
        else{
            update_text_log("You are not wielding a launcher weapon.",is_player);

            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_LEFT){
        bool move_check=check_movement(x-1,y);

        if(move_check){
            initiate_move=true;
        }
        else{
            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_UP){
        bool move_check=check_movement(x,y-1);

        if(move_check){
            initiate_move=true;
        }
        else{
            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_RIGHT){
        bool move_check=check_movement(x+1,y);

        if(move_check){
            initiate_move=true;
        }
        else{
            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_DOWN){
        bool move_check=check_movement(x,y+1);

        if(move_check){
            initiate_move=true;
        }
        else{
            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_LEFT_UP){
        bool move_check=check_movement(x-1,y-1);

        if(!move_check){
            move_check=check_movement(x-1,y);
        }
        if(!move_check){
            move_check=check_movement(x,y-1);
        }

        if(move_check){
            initiate_move=true;
        }
        else{
            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_RIGHT_UP){
        bool move_check=check_movement(x+1,y-1);

        if(!move_check){
            move_check=check_movement(x+1,y);
        }
        if(!move_check){
            move_check=check_movement(x,y-1);
        }

        if(move_check){
            initiate_move=true;
        }
        else{
            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_RIGHT_DOWN){
        bool move_check=check_movement(x+1,y+1);

        if(!move_check){
            move_check=check_movement(x+1,y);
        }
        if(!move_check){
            move_check=check_movement(x,y+1);
        }

        if(move_check){
            initiate_move=true;
        }
        else{
            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_LEFT_DOWN){
        bool move_check=check_movement(x-1,y+1);

        if(!move_check){
            move_check=check_movement(x-1,y);
        }
        if(!move_check){
            move_check=check_movement(x,y+1);
        }

        if(move_check){
            initiate_move=true;
        }
        else{
            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
    }
}

void Creature::execute_command_directional(short direction){
    directional_check_coordinates coords=determine_direction(direction);

    short command=input_directional;

    input_directional=0;

    int inventory_item_index=0;

    //If a directional inventory command has been given.
    if(directional_inventory_input_state!=0){
        //Determine the index of the inventory item represented by directional_inventory_input_state.

        for(int i=0;i<inventory.size();i++){
            if(inventory[i].inventory_letter==directional_inventory_input_state){
                inventory_item_index=i;
                break;
            }
        }

        directional_inventory_input_state=0;
    }

    if(command==DIRECTIONAL_COMMAND_OPEN_DOOR){
        //If the target tile is what was expected.
        if(vector_levels[current_level].tiles[coords.check_x][coords.check_y].type==TILE_TYPE_DOOR_CLOSED){
            //Try to open door.
            if(rc_open_door()){
                vector_levels[current_level].tiles[coords.check_x][coords.check_y].type=TILE_TYPE_DOOR_OPEN;
                update_text_log(string_door_opens.c_str(),is_player);
            }
            else{
                update_text_log(string_door_wont_move.c_str(),is_player);
            }
        }
        //If the target tile is not what was expected, it must have changed between the time
        //the command was checked, and the time it was executed in move().
        else{
            ///Do something here.
        }
    }

    else if(command==DIRECTIONAL_COMMAND_CLOSE_DOOR){
        //If the target tile is what was expected.
        if(vector_levels[current_level].tiles[coords.check_x][coords.check_y].type==TILE_TYPE_DOOR_OPEN){
            //Try to close door.
            if(rc_close_door()){
                vector_levels[current_level].tiles[coords.check_x][coords.check_y].type=TILE_TYPE_DOOR_CLOSED;
                update_text_log(string_door_closes.c_str(),is_player);
            }
            else{
                update_text_log(string_door_wont_move.c_str(),is_player);
            }
        }
        //If the target tile is not what was expected, it must have changed between the time
        //the command was checked, and the time it was executed in move().
        else{
            ///Do something here.
        }
    }

    else if(command==DIRECTIONAL_COMMAND_THROW_ITEM){
        //Setup a throw message.

        string throw_item="";

        //If the creature is the player.
        if(is_player){
            throw_item="You throw the ";
        }
        //If the creature is not the player.
        else{
            throw_item="The ";
            throw_item+=return_full_name();
            throw_item+=" throws the ";
        }

        throw_item+=inventory[inventory_item_index].return_full_name(1);

        throw_item+=".";

        update_text_log(throw_item.c_str(),true);

        //If the item's stack is not 0.
        if(inventory[inventory_item_index].stack!=0){
            //If the item's stack is 1 and the item is not money.
            if(inventory[inventory_item_index].stack==1 && inventory[inventory_item_index].inventory_letter!='$'){
                //Return the item's inventory letter.
                return_inventory_letter(inventory[inventory_item_index].inventory_letter);

                //If the creature is not the player.
                if(!is_player){
                    //Reset the item's inventory letter.
                    //We only do this for monsters. The player leaves the letter attached to the item.
                    inventory[inventory_item_index].inventory_letter=0;
                }
            }

            //Set the item's position to the creature's current position.
            inventory[inventory_item_index].x=x;
            inventory[inventory_item_index].y=y;

            //Add the item to the dungeon items vector.
            vector_levels[current_level].items.push_back(inventory[inventory_item_index]);

            //Assign an identifier to the newly thrown item.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].assign_identifier();

            //Set the thrown item's direction.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].move_direction=direction;

            //Set the thrown item's momentum.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].momentum=determine_momentum((double)inventory[inventory_item_index].weight,false);

            //Set the thrown item's special data.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].assign_owner_data_thrown(this);

            //If the item's stack is greater than 1, or the item is money.
            //We just subtract 1 from the stack instead of removing the item from the inventory.
            if(inventory[inventory_item_index].stack>1 || inventory[inventory_item_index].inventory_letter=='$'){
                //Set the newly created item's stack to 1.
                vector_levels[current_level].items[vector_levels[current_level].items.size()-1].stack=1;

                //Subtract 1 from the inventory item's stack.
                inventory[inventory_item_index].stack--;
            }
            //If the item's stack is 1 and the item is not money.
            //We remove the item from the inventory.
            else{
                //Return the item's identifier.
                inventory[inventory_item_index].return_identifier();

                //Remove the item from the inventory items vector.
                inventory.erase(inventory.begin()+inventory_item_index);
            }
        }
    }

    else if(command==DIRECTIONAL_COMMAND_FIRE_ITEM){
        //Determine the index of the quivered item.
        int quivered_item=index_of_item_in_slot(EQUIP_QUIVER);

        //Determine the index of the launcher item.
        int launcher_item=-1;
        //If a launcher is being wielded in the right hand.
        if(!equipment_slot_empty(-1,EQUIP_HOLD_RIGHT) && inventory[index_of_item_in_slot(EQUIP_HOLD_RIGHT)].category==ITEM_WEAPON && inventory[index_of_item_in_slot(EQUIP_HOLD_RIGHT)].governing_skill_weapon==SKILL_LAUNCHER_WEAPONS){
            launcher_item=index_of_item_in_slot(EQUIP_HOLD_RIGHT);
        }
        //If a launcher is being wielded in the left hand.
        else if(!equipment_slot_empty(-1,EQUIP_HOLD_LEFT) && inventory[index_of_item_in_slot(EQUIP_HOLD_LEFT)].category==ITEM_WEAPON && inventory[index_of_item_in_slot(EQUIP_HOLD_LEFT)].governing_skill_weapon==SKILL_LAUNCHER_WEAPONS){
            launcher_item=index_of_item_in_slot(EQUIP_HOLD_LEFT);
        }

        //Setup a fire message.

        string fire_item="";

        //If the creature is the player.
        if(is_player){
            fire_item="You fire the ";
        }
        //If the creature is not the player.
        else{
            fire_item="The ";
            fire_item+=return_full_name();
            fire_item+=" fires the ";
        }

        fire_item+=inventory[quivered_item].return_full_name(1);

        fire_item+=" from the ";
        fire_item+=inventory[launcher_item].return_full_name();

        fire_item+=".";
        update_text_log(fire_item.c_str(),true);

        //If the quivered item's stack is not 0.
        if(inventory[quivered_item].stack!=0){
            //If the item's stack is 1 and the item is not money.
            if(inventory[quivered_item].stack==1 && inventory[quivered_item].inventory_letter!='$'){
                //Return the item's inventory letter.
                return_inventory_letter(inventory[quivered_item].inventory_letter);

                //If the creature is not the player.
                if(!is_player){
                    //Reset the item's inventory letter.
                    //We only do this for monsters. The player leaves the letter attached to the item.
                    inventory[quivered_item].inventory_letter=0;
                }

                //Unequip the item.
                unequip_item(quivered_item);
            }

            //Set the item's position to the creature's current position.
            inventory[quivered_item].x=x;
            inventory[quivered_item].y=y;

            //Add the item to the dungeon items vector.
            vector_levels[current_level].items.push_back(inventory[quivered_item]);

            //Regardless of whether or not the item was unequipped above, the new item in the dungeon MUST not be set to equipped.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].equipped=false;

            //Assign an identifier to the newly fired item.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].assign_identifier();

            //Set the fired item's direction.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].move_direction=direction;

            //Set the fired item's momentum.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].momentum=determine_momentum((double)inventory[quivered_item].weight,true);

            //Set the fired item's special data.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].assign_owner_data_fired(this);

            //If the item's stack is greater than 1, or the item is money.
            //We just subtract 1 from the stack instead of removing the item from the inventory.
            if(inventory[quivered_item].stack>1 || inventory[quivered_item].inventory_letter=='$'){
                //Set the newly created item's stack to 1.
                vector_levels[current_level].items[vector_levels[current_level].items.size()-1].stack=1;

                //Subtract 1 from the inventory item's stack.
                inventory[quivered_item].stack--;
            }
            //If the item's stack is 1 and the item is not money.
            //We remove the item from the inventory.
            else{
                //Return the item's identifier.
                inventory[quivered_item].return_identifier();

                //Remove the item from the inventory items vector.
                inventory.erase(inventory.begin()+quivered_item);
            }
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_LEFT){
        bool move_check=check_movement(x-1,y);

        //If the target tile is what was expected.
        if(move_check){
            facing=FOV_WEST;

            execute_movement(x-1,y);
        }
        //If the target tile is not what was expected, it must have changed between the time
        //the command was checked, and the time it was executed in move().
        else{
            ///Do something here.
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_UP){
        bool move_check=check_movement(x,y-1);

        //If the target tile is what was expected.
        if(move_check){
            facing=FOV_NORTH;

            execute_movement(x,y-1);
        }
        //If the target tile is not what was expected, it must have changed between the time
        //the command was checked, and the time it was executed in move().
        else{
            ///Do something here.
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_RIGHT){
        bool move_check=check_movement(x+1,y);

        //If the target tile is what was expected.
        if(move_check){
            facing=FOV_EAST;

            execute_movement(x+1,y);
        }
        //If the target tile is not what was expected, it must have changed between the time
        //the command was checked, and the time it was executed in move().
        else{
            ///Do something here.
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_DOWN){
        bool move_check=check_movement(x,y+1);

        //If the target tile is what was expected.
        if(move_check){
            facing=FOV_SOUTH;

            execute_movement(x,y+1);
        }
        //If the target tile is not what was expected, it must have changed between the time
        //the command was checked, and the time it was executed in move().
        else{
            ///Do something here.
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_LEFT_UP){
        bool move_check=check_movement(x-1,y-1);

        if(move_check){
            facing=FOV_NORTHWEST;

            execute_movement(x-1,y-1);

            return;
        }
        else{
            move_check=check_movement(x-1,y);
        }

        if(move_check){
            facing=FOV_WEST;

            execute_movement(x-1,y);

            return;
        }
        else{
            move_check=check_movement(x,y-1);
        }

        if(move_check){
            facing=FOV_NORTH;

            execute_movement(x,y-1);

            return;
        }
        //The creature should have moved by now.
        //Since it hasn't, the target tile must have changed between the time the command was checked,
        //and the time it was executed in move().
        else{
            ///Do something here.
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_RIGHT_UP){
        bool move_check=check_movement(x+1,y-1);

        if(move_check){
            facing=FOV_NORTHEAST;

            execute_movement(x+1,y-1);

            return;
        }
        else{
            move_check=check_movement(x+1,y);
        }

        if(move_check){
            facing=FOV_EAST;

            execute_movement(x+1,y);

            return;
        }
        else{
            move_check=check_movement(x,y-1);
        }

        if(move_check){
            facing=FOV_NORTH;

            execute_movement(x,y-1);

            return;
        }
        //The creature should have moved by now.
        //Since it hasn't, the target tile must have changed between the time the command was checked,
        //and the time it was executed in move().
        else{
            ///Do something here.
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_RIGHT_DOWN){
        bool move_check=check_movement(x+1,y+1);

        if(move_check){
            facing=FOV_SOUTHEAST;

            execute_movement(x+1,y+1);

            return;
        }
        else{
            move_check=check_movement(x+1,y);
        }

        if(move_check){
            facing=FOV_EAST;

            execute_movement(x+1,y);

            return;
        }
        else{
            move_check=check_movement(x,y+1);
        }

        if(move_check){
            facing=FOV_SOUTH;

            execute_movement(x,y+1);

            return;
        }
        //The creature should have moved by now.
        //Since it hasn't, the target tile must have changed between the time the command was checked,
        //and the time it was executed in move().
        else{
            ///Do something here.
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_LEFT_DOWN){
        bool move_check=check_movement(x-1,y+1);

        if(move_check){
            facing=FOV_SOUTHWEST;

            execute_movement(x-1,y+1);

            return;
        }
        else{
            move_check=check_movement(x-1,y);
        }

        if(move_check){
            facing=FOV_WEST;

            execute_movement(x-1,y);

            return;
        }
        else{
            move_check=check_movement(x,y+1);
        }

        if(move_check){
            facing=FOV_SOUTH;

            execute_movement(x,y+1);

            return;
        }
        //The creature should have moved by now.
        //Since it hasn't, the target tile must have changed between the time the command was checked,
        //and the time it was executed in move().
        else{
            ///Do something here.
        }
    }
}

///*********************///
/// Inventory commands: ///
///*********************///

void Creature::check_command_inventory(char inventory_letter){
    short command=input_inventory;

    //Determine the index of the inventory item represented by inventory_letter.

    //If this value remains -1, no item is associated with inventory_letter.
    int inventory_item_index=-1;

    for(int i=0;i<inventory.size();i++){
        if(inventory[i].inventory_letter==inventory_letter){
            inventory_item_index=i;
            break;
        }
    }

    //If the item does not exist.
    if(inventory_item_index==-1){
        update_text_log("There is no such item.",is_player);

        //No inventory command will be executed.
        input_inventory=0;
        inventory_input_state=0;

        return;
    }

    if(command==INVENTORY_COMMAND_DROP_ITEM){
        //If the item is unequipped.
        if(!inventory[inventory_item_index].equipped &&
        //And if the item is either not money, or is money with at least 1 piece.
        ((inventory[inventory_item_index].inventory_letter=='$' && inventory[inventory_item_index].stack>0) || (inventory[inventory_item_index].inventory_letter!='$'))){
            initiate_move=true;
        }
        //If the item is money but the stack is 0.
        else if(inventory[inventory_item_index].inventory_letter=='$' && inventory[inventory_item_index].stack==0){
            update_text_log("You don't have any money.",is_player);

            //No inventory command will be executed.
            input_inventory=0;
            inventory_input_state=0;
        }
        //If the item is equipped.
        else{
            update_text_log("You must unequip the item first.",is_player);

            //No inventory command will be executed.
            input_inventory=0;
            inventory_input_state=0;
        }
    }

    else if(command==INVENTORY_COMMAND_EQUIP_RIGHT_HAND || command==INVENTORY_COMMAND_EQUIP_LEFT_HAND ||
            command==INVENTORY_COMMAND_QUIVER_ITEM || command==INVENTORY_COMMAND_EQUIP_ARMOR){
        //Determine the equipment slot to check.
        //If this remains -1, we are checking an armor slot.
        short equip_slot=-1;
        if(command==INVENTORY_COMMAND_EQUIP_RIGHT_HAND){
            equip_slot=EQUIP_HOLD_RIGHT;
        }
        else if(command==INVENTORY_COMMAND_EQUIP_LEFT_HAND){
            equip_slot=EQUIP_HOLD_LEFT;
        }
        else if(command==INVENTORY_COMMAND_QUIVER_ITEM){
            equip_slot=EQUIP_QUIVER;
        }

        //If the item is not already equipped.
        if(!inventory[inventory_item_index].equipped &&
        //And if the corresponding equipment slot is open.
        equipment_slot_empty(inventory_item_index,equip_slot)){
            initiate_move=true;
        }
        //If the item is already equipped.
        else if(inventory[inventory_item_index].equipped){
            update_text_log("That item is already equipped.",is_player);

            //No inventory command will be executed.
            input_inventory=0;
            inventory_input_state=0;
        }
        //If the item is not armor, but the creature tried to wear it.
        else if(command==INVENTORY_COMMAND_EQUIP_ARMOR && inventory[inventory_item_index].category!=ITEM_ARMOR){
            string message="You can't wear a ";
            message+=inventory[inventory_item_index].return_full_name(1);
            message+="!";
            update_text_log(message.c_str(),is_player);

            //No inventory command will be executed.
            input_inventory=0;
            inventory_input_state=0;
        }
        //If there is no equipment slot for the item.
        else{
            update_text_log("There is already an item equipped in that slot.",is_player);

            //No inventory command will be executed.
            input_inventory=0;
            inventory_input_state=0;
        }
    }

    else if(command==INVENTORY_COMMAND_UNEQUIP_ITEM){
        //If the item is equipped.
        if(inventory[inventory_item_index].equipped){
            initiate_move=true;
        }
        //If the item is not equipped.
        else{
            update_text_log("That item is not equipped.",is_player);

            //No inventory command will be executed.
            input_inventory=0;
            inventory_input_state=0;
        }
    }

    else if(command==INVENTORY_COMMAND_THROW_ITEM){
        //If either the item is NOT equipped or it is equipped but has more than 1 stack.
        if((!inventory[inventory_item_index].equipped || (inventory[inventory_item_index].equipped && inventory[inventory_item_index].stack>1)) &&
        //And if the item is either not money, or is money with at least 1 piece.
        ((inventory[inventory_item_index].inventory_letter=='$' && inventory[inventory_item_index].stack>0) || (inventory[inventory_item_index].inventory_letter!='$'))){
            initiate_move=true;
        }
        //If the item is money but the stack is 0.
        else if(inventory[inventory_item_index].inventory_letter=='$' && inventory[inventory_item_index].stack==0){
            update_text_log("You don't have any money.",is_player);

            //No inventory command will be executed.
            input_inventory=0;
            inventory_input_state=0;
        }
        //If the item is equipped.
        else{
            update_text_log("You must unequip the item first.",is_player);

            //No inventory command will be executed.
            input_inventory=0;
            inventory_input_state=0;
        }
    }

    else if(command==INVENTORY_COMMAND_QUAFF_ITEM){
        //If either the item is NOT equipped or it is equipped but has more than 1 stack.
        if((!inventory[inventory_item_index].equipped || (inventory[inventory_item_index].equipped && inventory[inventory_item_index].stack>1)) &&
           //And if the item can quench thirst.
           inventory[inventory_item_index].thirst_quenched>0){
            initiate_move=true;
        }
        //If the item is equipped and its stack size is 1.
        else if(inventory[inventory_item_index].equipped && inventory[inventory_item_index].stack==1){
            update_text_log("You must unequip the item first.",is_player);

            //No inventory command will be executed.
            input_inventory=0;
            inventory_input_state=0;
        }
        //If the item cannot quench thirst.
        else{
            update_text_log("You can't drink that!",is_player);

            //No inventory command will be executed.
            input_inventory=0;
            inventory_input_state=0;
        }
    }

    else if(command==INVENTORY_COMMAND_USE_ITEM){
        //If the item can be used.
        if(inventory[inventory_item_index].use==ITEM_USE_LIGHT && inventory[inventory_item_index].fuel>0){
            initiate_move=true;
        }
        //If the item is a light source with no fuel.
        else if(inventory[inventory_item_index].use==ITEM_USE_LIGHT && inventory[inventory_item_index].fuel==0){
            string message="The ";
            message+=inventory[inventory_item_index].return_full_name(1);
            message+=" is out of fuel.";
            update_text_log(message.c_str(),is_player);

            //No inventory command will be executed.
            input_inventory=0;
            inventory_input_state=0;
        }
        //If the item cannot be used.
        else{
            string message="You have no idea what to do with the ";
            message+=inventory[inventory_item_index].return_full_name(1);
            message+=".";
            update_text_log(message.c_str(),is_player);

            //No inventory command will be executed.
            input_inventory=0;
            inventory_input_state=0;
        }
    }
}

void Creature::execute_command_inventory(char inventory_letter){
    short command=input_inventory;

    input_inventory=0;

    //Determine the index of the inventory item represented by inventory_letter.

    int inventory_item_index=0;

    for(int i=0;i<inventory.size();i++){
        if(inventory[i].inventory_letter==inventory_letter){
            inventory_item_index=i;
            break;
        }
    }

    if(command==INVENTORY_COMMAND_DROP_ITEM){
        //Setup a drop message.

        string drop_item="";

        //If the creature is the player.
        if(is_player){
            drop_item="You drop the ";
        }
        //If the creature is not the player.
        else{
            drop_item="The ";
            drop_item+=return_full_name();
            drop_item+=" drops the ";
        }

        drop_item+=inventory[inventory_item_index].return_full_name();

        drop_item+=".";

        update_text_log(drop_item.c_str(),true);

        //If the item is not money.
        if(inventory[inventory_item_index].inventory_letter!='$'){
            //Return the item's inventory letter.
            return_inventory_letter(inventory[inventory_item_index].inventory_letter);

            //If the creature is not the player.
            if(!is_player){
                //Reset the item's inventory letter.
                //We only do this for monsters. The player leaves the letter attached to the item.
                inventory[inventory_item_index].inventory_letter=0;
            }

            //Set the item's position to the creature's current position.
            inventory[inventory_item_index].x=x;
            inventory[inventory_item_index].y=y;

            //Add the item to the dungeon items vector.
            vector_levels[current_level].items.push_back(inventory[inventory_item_index]);

            //Assign an identifier to the new item.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].assign_identifier();

            //Return the inventory item's identifier.
            inventory[inventory_item_index].return_identifier();

            //Remove the item from the inventory items vector.
            inventory.erase(inventory.begin()+inventory_item_index);
        }
        //If the item is money.
        else{
            //Set the item's position to the creature's current position.
            inventory[inventory_item_index].x=x;
            inventory[inventory_item_index].y=y;

            //Add the item to the dungeon items vector.
            vector_levels[current_level].items.push_back(inventory[inventory_item_index]);

            //Assign an identifier to the new item.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].assign_identifier();

            //Assign an owner identifier to the new item.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].owner=identifier;

            //Zero out the money count.
            inventory[inventory_item_index].stack=0;
        }
    }

    else if(command==INVENTORY_COMMAND_EQUIP_RIGHT_HAND || command==INVENTORY_COMMAND_EQUIP_LEFT_HAND ||
            command==INVENTORY_COMMAND_QUIVER_ITEM || command==INVENTORY_COMMAND_EQUIP_ARMOR){
        //Setup an equip message.

        string str_equip_item="";

        //If the creature is the player.
        if(is_player){
            if(command==INVENTORY_COMMAND_EQUIP_RIGHT_HAND || command==INVENTORY_COMMAND_EQUIP_LEFT_HAND){
                str_equip_item="You wield the ";
            }
            else if(command==INVENTORY_COMMAND_QUIVER_ITEM){
                str_equip_item="You place the ";
            }
            else if(command==INVENTORY_COMMAND_EQUIP_ARMOR){
                str_equip_item="You put on the ";
            }
        }
        //If the creature is not the player.
        else{
            if(command==INVENTORY_COMMAND_EQUIP_RIGHT_HAND || command==INVENTORY_COMMAND_EQUIP_LEFT_HAND){
                str_equip_item="The ";
                str_equip_item+=return_full_name();
                str_equip_item+=" wields the ";
            }
            else if(command==INVENTORY_COMMAND_QUIVER_ITEM){
                str_equip_item="The ";
                str_equip_item+=return_full_name();
                str_equip_item+=" places the ";
            }
            else if(command==INVENTORY_COMMAND_EQUIP_ARMOR){
                str_equip_item="The ";
                str_equip_item+=return_full_name();
                str_equip_item+=" puts on the ";
            }
        }

        if(command==INVENTORY_COMMAND_EQUIP_RIGHT_HAND || command==INVENTORY_COMMAND_EQUIP_LEFT_HAND){
            str_equip_item+=inventory[inventory_item_index].return_full_name(1);
        }
        else{
            str_equip_item+=inventory[inventory_item_index].return_full_name();
        }

        if(command==INVENTORY_COMMAND_EQUIP_RIGHT_HAND){
            if(is_player){
                str_equip_item+=" in your right hand";
            }
            else{
                str_equip_item+=" in its right hand";
            }
        }
        else if(command==INVENTORY_COMMAND_EQUIP_LEFT_HAND){
            if(is_player){
                str_equip_item+=" in your left hand";
            }
            else{
                str_equip_item+=" in its left hand";
            }
        }
        else if(command==INVENTORY_COMMAND_QUIVER_ITEM){
            if(is_player){
                str_equip_item+=" into your quiver";
            }
            else{
                str_equip_item+=" into its quiver";
            }
        }

        str_equip_item+=".";

        update_text_log(str_equip_item.c_str(),true);

        //Determine the equipment slot.
        //If this remains -1, we are equipping a piece of armor.
        short equip_slot=-1;
        if(command==INVENTORY_COMMAND_EQUIP_RIGHT_HAND){
            equip_slot=EQUIP_HOLD_RIGHT;
        }
        else if(command==INVENTORY_COMMAND_EQUIP_LEFT_HAND){
            equip_slot=EQUIP_HOLD_LEFT;
        }
        else if(command==INVENTORY_COMMAND_QUIVER_ITEM){
            equip_slot=EQUIP_QUIVER;
        }

        //Equip the item.
        equip_item(inventory_item_index,equip_slot);
    }

    else if(command==INVENTORY_COMMAND_UNEQUIP_ITEM){
        //Determine what slot the item is equipped in.
        short equip_slot=item_equipped_in_which_slot(inventory_item_index);

        //Setup an equip message.

        string str_unequip_item="";

        //If the creature is the player.
        if(is_player){
            str_unequip_item="You unequip the ";
        }
        //If the creature is not the player.
        else{
            str_unequip_item="The ";
            str_unequip_item+=return_full_name();
            str_unequip_item+=" unequips the ";
        }

        if(equip_slot==EQUIP_QUIVER){
            str_unequip_item+=inventory[inventory_item_index].return_full_name();
        }
        else{
            str_unequip_item+=inventory[inventory_item_index].return_full_name(1);
        }

        str_unequip_item+=".";

        update_text_log(str_unequip_item.c_str(),true);

        //Unequip the item.
        unequip_item(inventory_item_index);
    }

    else if(command==INVENTORY_COMMAND_THROW_ITEM){
        //Setup a throw message.

        string throw_item="";

        //If the creature is the player.
        if(is_player){
            throw_item="Throw in what direction?";
        }

        update_text_log(throw_item.c_str(),is_player);

        input_directional=DIRECTIONAL_COMMAND_THROW_ITEM;
    }

    else if(command==INVENTORY_COMMAND_QUAFF_ITEM){
        //Setup a message.

        string str_quaff_item="";

        //If the creature is the player.
        if(is_player){
            str_quaff_item="You quaff the ";
        }
        //If the creature is not the player.
        else{
            str_quaff_item="The ";
            str_quaff_item+=return_full_name();
            str_quaff_item+=" quaffs the ";
        }

        str_quaff_item+=inventory[inventory_item_index].return_full_name(1);

        str_quaff_item+=".";

        update_text_log(str_quaff_item.c_str(),true);

        //Quaff the item.

        //Reduce the creature's thirst by the item's thirst_quenched value.
        change_thirst(false,inventory[inventory_item_index].thirst_quenched);

        //If the stack is greater than 1.
        if(inventory[inventory_item_index].stack>1){
            //Simply decrement the stack by 1.
            inventory[inventory_item_index].stack--;
        }
        //If the stack is exactly 1.
        else{
            //Return the item's inventory letter.
            return_inventory_letter(inventory[inventory_item_index].inventory_letter);

            //Return the item's identifier.
            inventory[inventory_item_index].return_identifier();

            //Remove the item from the inventory items vector.
            inventory.erase(inventory.begin()+inventory_item_index);
        }
    }

    else if(command==INVENTORY_COMMAND_USE_ITEM){
        //Setup a message.

        string str_item="";

        if(inventory[inventory_item_index].use==ITEM_USE_LIGHT){
            //If the creature is the player.
            if(is_player){
                if(inventory[inventory_item_index].light_on){
                    str_item="You snuff out the ";
                }
                else{
                    str_item="You light the ";
                }
            }
            //If the creature is not the player.
            else{
                if(inventory[inventory_item_index].light_on){
                    str_item="The ";
                    str_item+=return_full_name();
                    str_item+=" snuffs out the ";
                }
                else{
                    str_item="The ";
                    str_item+=return_full_name();
                    str_item+=" lights the ";
                }
            }

            str_item+=inventory[inventory_item_index].return_full_name(1);
            str_item+=".";
        }

        update_text_log(str_item.c_str(),true);

        //Use the item.

        if(inventory[inventory_item_index].use==ITEM_USE_LIGHT){
            inventory[inventory_item_index].light_on=!inventory[inventory_item_index].light_on;
        }
    }
}
