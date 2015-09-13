/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "combat_all.h"

using namespace std;

int determine_damage_reduction(Creature* target){
    int damage_reduction=0;

    //Exercise the target's armor skill.
    int points_gained=1;

    double inventory_weight=target->return_inventory_weight(ITEM_ARMOR);

    //Apply the encumbrance bonus to the armor skill increase.
    points_gained+=inventory_weight/75.0;
    target->gain_skill_experience(SKILL_ARMOR,points_gained);

    //Add in target's armor damage reduction.

    damage_reduction=target->return_armor()/2.0;

    return damage_reduction;
}
