/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"

using namespace std;

short Creature::return_attribute_strength(){
    short attribute=attributes[ATTRIBUTE_STRENGTH];

    //If the creature's thirst is satiated.
    if(thirst>=THIRST_SATIATED && thirst<THIRST_NOT_THIRSTY){
        ///Strength increased slightly.
    }
    //If the creature is thirsty.
    else if(thirst>=THIRST_THIRSTY && thirst<THIRST_WEAK){
        ///Strength decreased slightly.
    }
    //If the creature is weak.
    else if(thirst>=THIRST_WEAK && thirst<THIRST_FAINTING){
        ///Strength decreased.
    }
    //If the creature is fainting.
    else if(thirst>=THIRST_FAINTING && thirst<THIRST_DEATH){
        ///Strength decreased (identical to weak).
    }

    return attribute;
}

short Creature::return_attribute_agility(){
    short attribute=attributes[ATTRIBUTE_AGILITY];

    //If the creature is satiated.
    if(thirst>=THIRST_SATIATED && thirst<THIRST_NOT_THIRSTY){
        ///Agility increased slightly.
    }
    //If the creature is fainting.
    else if(thirst>=THIRST_FAINTING && thirst<THIRST_DEATH){
        ///Agility decreased.
    }

    return attribute;
}

short Creature::return_attribute_hardiness(){
    short attribute=attributes[ATTRIBUTE_HARDINESS];

    ///

    return attribute;
}

short Creature::return_attribute_comprehension(){
    short attribute=attributes[ATTRIBUTE_COMPREHENSION];

    ///

    return attribute;
}

short Creature::return_attribute_acumen(){
    short attribute=attributes[ATTRIBUTE_ACUMEN];

    ///

    return attribute;
}

short Creature::return_attribute_luck(){
    short attribute=attributes[ATTRIBUTE_LUCK];

    ///

    return attribute;
}
