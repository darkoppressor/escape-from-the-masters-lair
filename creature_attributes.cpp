/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"
#include "world.h"

using namespace std;

int Creature::return_attribute_strength(){
    double attribute=attributes[ATTRIBUTE_STRENGTH];

    short thirst_state=return_thirst_state();

    //If the creature's thirst is satiated.
    if(thirst_state==THIRST_SATIATED){
        attribute+=attribute*0.10;
    }
    //If the creature is thirsty.
    else if(thirst_state==THIRST_THIRSTY){
        attribute-=attribute*0.10;
    }
    //If the creature is weak.
    else if(thirst_state==THIRST_WEAK){
        attribute-=attribute*0.25;
    }
    //If the creature is fainting.
    else if(thirst_state==THIRST_FAINTING){
        attribute-=attribute*0.25;
    }
    //If the creature is dying.
    else if(thirst_state==THIRST_DEATH){
        attribute-=attribute*0.25;
    }

    //Apply the racial modifier.
    attribute+=templates.template_races[race].attributes[ATTRIBUTE_STRENGTH];

    if(attribute<1.0){
        attribute=1.0;
    }

    return (int)attribute;
}

int Creature::return_attribute_agility(){
    double attribute=attributes[ATTRIBUTE_AGILITY];

    //If the creature is anything aside from unburdened.
    if(return_encumbrance_state()!=ENCUMBRANCE_UNBURDENED){
        //Apply the encumbrance penalty.
        attribute-=(return_inventory_weight()-return_carry_capacity())*0.025;
    }

    short thirst_state=return_thirst_state();

    //If the creature is satiated.
    if(thirst_state==THIRST_SATIATED){
        //Apply a satiated bonus.
        attribute+=attribute*0.10;
    }
    //If the creature is fainting.
    else if(thirst_state==THIRST_FAINTING){
        //Apply a fainting penalty.
        attribute-=attribute*0.25;
    }
    //If the creature is dying.
    else if(thirst_state==THIRST_DEATH){
        //Apply a dying penalty.
        attribute-=attribute*0.25;
    }

    //Apply the racial modifier.
    attribute+=templates.template_races[race].attributes[ATTRIBUTE_AGILITY];

    if(attribute<1.0){
        attribute=1.0;
    }

    return (int)attribute;
}

int Creature::return_attribute_hardiness(){
    double attribute=attributes[ATTRIBUTE_HARDINESS];

    //Apply the racial modifier.
    attribute+=templates.template_races[race].attributes[ATTRIBUTE_HARDINESS];

    if(attribute<1.0){
        attribute=1.0;
    }

    return (int)attribute;
}

/**int Creature::return_attribute_comprehension(){
    double attribute=attributes[ATTRIBUTE_COMPREHENSION];

    //Apply the racial modifier.
    attribute+=templates.template_races[race].attributes[ATTRIBUTE_COMPREHENSION];

    if(attribute<1.0){
        attribute=1.0;
    }

    return (int)attribute;
}

int Creature::return_attribute_acumen(){
    double attribute=attributes[ATTRIBUTE_ACUMEN];

    //Apply the racial modifier.
    attribute+=templates.template_races[race].attributes[ATTRIBUTE_ACUMEN];

    if(attribute<1.0){
        attribute=1.0;
    }

    return (int)attribute;
}

int Creature::return_attribute_luck(){
    double attribute=attributes[ATTRIBUTE_LUCK];

    //Apply the racial modifier.
    attribute+=templates.template_races[race].attributes[ATTRIBUTE_LUCK];

    if(attribute<1.0){
        attribute=1.0;
    }

    return (int)attribute;
}*/
