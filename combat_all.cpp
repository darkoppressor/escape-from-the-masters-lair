/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "combat_all.h"

using namespace std;

int determine_damage_reduction(Creature* target){
    int damage_reduction=0;

    //Add in target's armor damage reduction.

    //Look through all of the target's armor slots.
    for(int i=EQUIP_HEAD;i<EQUIP_FINGER_LEFT+1;i++){
        //If this slot has an item equipped.
        if(target->equipment[i]!=0){
            //Determine the identifier for the item equipped in this slot.
            int item_identifier=target->slot_equipped_with_what_item(target->equipment[i]);

            //Determine the base amount of damage absorbed by this item.
            int armor_absorption=target->inventory[item_identifier].defense;

            //Apply the armor skill.
            armor_absorption+=armor_absorption*(target->skills[SKILL_ARMOR]/10);

            damage_reduction+=armor_absorption;
        }
    }

    //Apply the hardiness bonus.
    damage_reduction+=damage_reduction*(target->attributes[ATTRIBUTE_HARDINESS]/4);

    return damage_reduction;
}
