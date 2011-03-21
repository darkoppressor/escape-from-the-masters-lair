/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"
#include "message_log.h"

using namespace std;

void Creature::mix_items(int item_index_1,int item_index_2){
    string str_item="";

    //If the first item is a fuel refiller, and the second item uses fuel.
    if(inventory[item_index_1].possesses_effect(ITEM_EFFECT_FUEL) && inventory[item_index_2].fuel_max>0){
        //If the creature is the player.
        if(is_player){
            str_item="You fill the ";
            str_item+=inventory[item_index_2].return_full_name();
            str_item+=" with the ";
            str_item+=inventory[item_index_1].return_full_name(1);
        }
        //If the creature is not the player.
        else{
            str_item="The ";
            str_item+=return_full_name();
            str_item+="fills the ";
            str_item+=inventory[item_index_2].return_full_name();
            str_item+=" with the ";
            str_item+=inventory[item_index_1].return_full_name(1);
        }

        //Max out the second item's fuel supply.
        inventory[item_index_2].fuel=inventory[item_index_2].fuel_max;

        //The first item is used up.

        //If the stack is greater than 1.
        if(inventory[item_index_1].stack>1){
            //Simply decrement the stack by 1.
            inventory[item_index_1].stack--;
        }
        //If the stack is exactly 1.
        else{
            //Return the item's inventory letter.
            return_inventory_letter(inventory[item_index_1].inventory_letter);

            //Return the item's identifier.
            inventory[item_index_1].return_identifier();

            //Remove the item from the inventory items vector.
            inventory.erase(inventory.begin()+item_index_1);
        }
    }
    else{
        str_item="You aren't quite sure what to do with these.";
    }

    update_text_log(str_item.c_str(),is_player);
}
