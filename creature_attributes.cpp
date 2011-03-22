/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"

using namespace std;

short Creature::return_attribute_strength(){
    double attribute=attributes[ATTRIBUTE_STRENGTH];

    //If the creature's thirst is satiated.
    if(thirst>=THIRST_SATIATED && thirst<THIRST_NOT_THIRSTY){
        attribute+=attribute*0.10;
    }
    //If the creature is thirsty.
    else if(thirst>=THIRST_THIRSTY && thirst<THIRST_WEAK){
        attribute-=attribute*0.10;
    }
    //If the creature is weak.
    else if(thirst>=THIRST_WEAK && thirst<THIRST_FAINTING){
        attribute-=attribute*0.25;
    }
    //If the creature is fainting.
    else if(thirst>=THIRST_FAINTING && thirst<THIRST_DEATH){
        attribute-=attribute*0.25;
    }

    if(attribute<1.0){
        attribute=1.0;
    }

    return (short)attribute;
}

short Creature::return_attribute_agility(){
    double attribute=attributes[ATTRIBUTE_AGILITY];

    //If the creature is anything aside from unencumbered.
    if(!(return_inventory_weight()<=return_carry_capacity())){
        //Apply the encumbrance penalty.
        attribute-=(return_inventory_weight()-return_carry_capacity())*0.025;
    }

    //If the creature is satiated.
    if(thirst>=THIRST_SATIATED && thirst<THIRST_NOT_THIRSTY){
        //Apply a satiated bonus.
        attribute+=attribute*0.10;
    }
    //If the creature is fainting.
    else if(thirst>=THIRST_FAINTING && thirst<THIRST_DEATH){
        //Apply a fainting penalty.
        attribute-=attribute*0.25;
    }

    if(attribute<1.0){
        attribute=1.0;
    }

    return (short)attribute;
}

short Creature::return_attribute_hardiness(){
    double attribute=attributes[ATTRIBUTE_HARDINESS];

    ///

    if(attribute<1.0){
        attribute=1.0;
    }

    return (short)attribute;
}

short Creature::return_attribute_comprehension(){
    double attribute=attributes[ATTRIBUTE_COMPREHENSION];

    ///

    if(attribute<1.0){
        attribute=1.0;
    }

    return (short)attribute;
}

short Creature::return_attribute_acumen(){
    double attribute=attributes[ATTRIBUTE_ACUMEN];

    ///

    if(attribute<1.0){
        attribute=1.0;
    }

    return (short)attribute;
}

short Creature::return_attribute_luck(){
    double attribute=attributes[ATTRIBUTE_LUCK];

    ///

    if(attribute<1.0){
        attribute=1.0;
    }

    return (short)attribute;
}
