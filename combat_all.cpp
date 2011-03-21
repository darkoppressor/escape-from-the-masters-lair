/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "combat_all.h"

using namespace std;

int determine_damage_reduction(Creature* target){
    int damage_reduction=0;

    //Exercise the target's dodging skill.
    target->gain_skill_experience(SKILL_DODGING,1);

    //Exercise the target's armor skill.
    int points_gained=1;
    double inventory_weight=target->return_inventory_weight(ITEM_ARMOR);
    //Apply the encumbrance bonus to the armor skill increase.
    points_gained+=inventory_weight/75.0;
    target->gain_skill_experience(SKILL_ARMOR,points_gained);

    //Add in target's armor damage reduction.

    //Look through all of the target's armor slots.
    for(int i=EQUIP_HEAD;i<EQUIP_FINGER_LEFT+1;i++){
        //If this slot has an item equipped.
        if(target->equipment[i]!=0){
            //Determine the identifier for the item equipped in this slot.
            int item_identifier=target->index_of_item_in_slot(i);

            //Determine the base amount of damage absorbed by this item.
            int armor_absorption=target->inventory[item_identifier].defense;

            //Apply the armor skill.
            armor_absorption+=armor_absorption*(target->return_skill_armor()/10);

            damage_reduction+=armor_absorption;
        }
    }

    //Apply the hardiness bonus.
    damage_reduction+=damage_reduction*(target->return_attribute_hardiness()/4);

    return damage_reduction;
}
