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

    else if(command==COMMAND_TOGGLE_LIGHT){
        //If the light can be toggled.
        ///This needs to be updated when lights are fully implemented.
        if(true){
            command_standard=command;
            initiate_move=true;
        }
        //If the light cannot be toggled.
        else{
            ///update_text_log(string_command_stairs_not_here.c_str(),is_player);
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
                    grab_item+=".";
                    update_text_log(grab_item.c_str(),true);

                    //Check to see if there is an identical item already in the inventory.
                    inventory_match match_check=check_for_inventory_match(&vector_levels[current_level].items[i]);

                    //If there is already an identical item in the inventory, and the item is stackable.
                    if(match_check.match_found && vector_levels[current_level].items[i].stackable){
                        inventory[match_check.inventory_slot].stack+=vector_levels[current_level].items[i].stack;
                    }
                    //If there is no identical item in the inventory, or the item is not stackable.
                    else{
                        //Determine an inventory letter for the item.

                        //If the item already has an inventory letter, this means the player assigned one to it, and then dropped it.
                        //So we only do this check for the player.
                        if(is_player && vector_levels[current_level].items[i].inventory_letter!=0){
                            //If the item's inventory letter is available.
                            if(check_inventory_letter_availability(vector_levels[current_level].items[i].inventory_letter)){
                                //Leave the item's inventory letter alone.
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
                    }

                    //Remove the item from the dungeon items vector.
                    vector_levels[current_level].items.erase(vector_levels[current_level].items.begin()+i);

                    //For now, we break out of the for() loop,
                    //meaning we only pick up one item.
                    break;
                }
            }
        }
    }

    else if(command==COMMAND_TOGGLE_LIGHT){
        light_on=!light_on;

        if(light_on){
            update_text_log("You turn your light on.",true);
        }
        else{
            update_text_log("You turn your light off.",true);
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
            //
            ///update_text_log("",is_player);

            //No directional command will be executed.
            input_directional=0;
            move_state=0;
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_LEFT){
        bool move_check=false;

        move_check=check_movement(x-1,y);

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
        bool move_check=false;

        move_check=check_movement(x,y-1);

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
        bool move_check=false;

        move_check=check_movement(x+1,y);

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
        bool move_check=false;

        move_check=check_movement(x,y+1);

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
        bool move_check=false;

        move_check=check_movement(x-1,y-1);
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
        bool move_check=false;

        move_check=check_movement(x+1,y-1);
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
        bool move_check=false;

        move_check=check_movement(x+1,y+1);
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
        bool move_check=false;

        move_check=check_movement(x-1,y+1);
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

            //If the item's stack is greater than 1, or the item is money.
            //We just subtract 1 from the stack instead of removing the item from the inventory.
            if(inventory[inventory_item_index].stack>1 || inventory[inventory_item_index].inventory_letter=='$'){
                //Add the item to the dungeon items vector.
                vector_levels[current_level].items.push_back(inventory[inventory_item_index]);

                //Set the newly created item's stack to 1.
                vector_levels[current_level].items[vector_levels[current_level].items.size()-1].stack=1;

                //Subtract 1 from the inventory item's stack.
                inventory[inventory_item_index].stack--;
            }
            //If the item's stack is 1 and the item is not money.
            //We remove the item from the inventory.
            else{
                //Add the item to the dungeon items vector.
                vector_levels[current_level].items.push_back(inventory[inventory_item_index]);

                //Remove the item from the inventory items vector.
                inventory.erase(inventory.begin()+inventory_item_index);
            }
        }
    }

    else if(command==DIRECTIONAL_COMMAND_MOVE_LEFT){
        bool move_check=false;

        move_check=check_movement(x-1,y);

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
        bool move_check=false;

        move_check=check_movement(x,y-1);

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
        bool move_check=false;

        move_check=check_movement(x+1,y);

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
        bool move_check=false;

        move_check=check_movement(x,y+1);

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
        bool move_check=false;

        move_check=check_movement(x-1,y-1);

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
        bool move_check=false;

        move_check=check_movement(x+1,y-1);

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
        bool move_check=false;

        move_check=check_movement(x+1,y+1);

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
        bool move_check=false;

        move_check=check_movement(x-1,y+1);

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

    else if(command==INVENTORY_COMMAND_EQUIP_ITEM){
        //If the item is not already equipped.
        if(!inventory[inventory_item_index].equipped &&
        //And if the item is not stackable.
        !inventory[inventory_item_index].stackable &&
        //And if a corresponding equipment slot is open.
        ((inventory[inventory_item_index].category!=ITEM_ARMOR && equipment_slot_empty(inventory[inventory_item_index].category,0)) ||
        (inventory[inventory_item_index].category==ITEM_ARMOR && equipment_slot_empty(ITEM_ARMOR,inventory[inventory_item_index].armor_category)))){
            initiate_move=true;
        }
        //If the item is already equipped.
        else if(inventory[inventory_item_index].equipped){
            update_text_log("That item is already equipped.",is_player);

            //No inventory command will be executed.
            input_inventory=0;
            inventory_input_state=0;
        }
        //If the item is stackable, and thus not equippable.
        else if(inventory[inventory_item_index].stackable){
            update_text_log("Stackable items cannot be equipped.",is_player);

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

            //Zero out the money count.
            inventory[inventory_item_index].stack=0;
        }
    }

    else if(command==INVENTORY_COMMAND_EQUIP_ITEM){
        //Setup an equip message.

        string str_equip_item="";

        //If the creature is the player.
        if(is_player){
            str_equip_item="You equip the ";
        }
        //If the creature is not the player.
        else{
            str_equip_item="The ";
            str_equip_item+=return_full_name();
            str_equip_item+=" equips the ";
        }

        str_equip_item+=inventory[inventory_item_index].return_full_name();
        str_equip_item+=".";
        update_text_log(str_equip_item.c_str(),true);

        //Equip the item.
        equip_item(inventory_item_index);
    }

    else if(command==INVENTORY_COMMAND_UNEQUIP_ITEM){
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

        str_unequip_item+=inventory[inventory_item_index].return_full_name();
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
}
