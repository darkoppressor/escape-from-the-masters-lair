/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"
#include "message_log.h"

using namespace std;

void Creature::mix_items(int item_index_1,int item_index_2){
    string str_item="";

    bool first_item_used=false;

    //If the first item is a fuel refiller, and the second item uses fuel.
    if(inventory[item_index_1].possesses_effect(ITEM_EFFECT_FUEL) && inventory[item_index_2].fuel_max>0){
        //If the creature is the player.
        if(is_player){
            str_item="You fill the ";
            str_item+=inventory[item_index_2].return_full_name();
            str_item+=" with the ";
            str_item+=inventory[item_index_1].return_full_name(1);
            str_item+=".";
        }
        //If the creature is not the player.
        else{
            str_item="The ";
            str_item+=return_full_name();
            str_item+="fills the ";
            str_item+=inventory[item_index_2].return_full_name();
            str_item+=" with the ";
            str_item+=inventory[item_index_1].return_full_name(1);
            str_item+=".";
        }

        //Max out the second item's fuel supply.
        inventory[item_index_2].fuel=inventory[item_index_2].fuel_max;

        first_item_used=true;
    }

    //If the first item is a dye, and the second item is not a dye.
    if(inventory[item_index_1].possesses_effect(ITEM_EFFECT_DYE) && !inventory[item_index_2].possesses_effect(ITEM_EFFECT_DYE)){
        //If the creature is the player.
        if(is_player){
            str_item="You dye the ";
            str_item+=inventory[item_index_2].return_full_name();
            str_item+=" with the ";
            str_item+=inventory[item_index_1].return_full_name(1);
            str_item+=".";
        }
        //If the creature is not the player.
        else{
            str_item="The ";
            str_item+=return_full_name();
            str_item+="dyes the ";
            str_item+=inventory[item_index_2].return_full_name();
            str_item+=" with the ";
            str_item+=inventory[item_index_1].return_full_name(1);
            str_item+=".";
        }

        //Dye the second item with the first item's dye color.
        ///This should be improved later. Colors should mix and whatnot.
        ///Eventually, if you keep dying an item, it will end up dark brown or something.
        inventory[item_index_2].dye=inventory[item_index_1].color;

        first_item_used=true;
    }

    //If the first item is water.
    if(inventory[item_index_1].possesses_effect(ITEM_EFFECT_WATER)){
        //If the creature is the player.
        if(is_player){
            str_item="You pour the ";
            str_item+=inventory[item_index_1].return_full_name(1);
            str_item+=" on the ";
            str_item+=inventory[item_index_2].return_full_name(1);
            str_item+=".";
        }
        //If the creature is not the player.
        else{
            str_item="The ";
            str_item+=return_full_name();
            str_item+="pours the ";
            str_item+=inventory[item_index_1].return_full_name(1);
            str_item+=" on the ";
            str_item+=inventory[item_index_2].return_full_name(1);
            str_item+=".";
        }

        //Wash any dye off of the second item.
        inventory[item_index_2].dye=0;

        //If the item is lit, extinguish its light.
        inventory[item_index_2].light_on=false;

        first_item_used=true;
    }

    if(first_item_used){
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

    if(str_item.length()>0){
        update_text_log(str_item.c_str(),is_player);
    }
}

bool Creature::items_mixable(int item_index_1,int item_index_2){
    //If the first item is a fuel refiller, and the second item uses fuel.
    if(inventory[item_index_1].possesses_effect(ITEM_EFFECT_FUEL) && inventory[item_index_2].fuel_max>0){
        return true;
    }
    //If the first item is a dye, and the second item is not a dye.
    else if(inventory[item_index_1].possesses_effect(ITEM_EFFECT_DYE) && !inventory[item_index_2].possesses_effect(ITEM_EFFECT_DYE)){
        return true;
    }
    //If the first item is water.
    else if(inventory[item_index_1].possesses_effect(ITEM_EFFECT_WATER)){
        return true;
    }
    else{
        update_text_log("You aren't quite sure what to do with these.",is_player);
    }


    return false;
}
