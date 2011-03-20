/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"

using namespace std;

int Creature::return_health(){
    double number=health;

    ///

    return (int)number;
}

int Creature::return_health_max(){
    double number=health_max;

    ///

    return (int)number;
}

int Creature::return_mana(){
    double number=mana;

    ///

    return (int)number;
}

int Creature::return_mana_max(){
    double number=mana_max;

    ///

    return (int)number;
}

unsigned short Creature::return_movement_speed(){
    double speed=movement_speed;

    //Subtract the agility bonus.
    speed-=return_attribute_agility()*3;

    //Subtract the hardiness bonus.
    speed-=return_attribute_hardiness();

    //If the creature is bloated.
    if(thirst<=THIRST_BLOATED){
        //Being bloated causes a penalty to speed.
        speed+=speed*0.25;
    }

    if(speed<1.0){
        speed=1.0;
    }

    return (unsigned short)speed;
}

short Creature::return_next_move(){
    double speed=next_move;

    //Subtract the agility bonus.
    speed-=return_attribute_agility()*3;

    //Subtract the hardiness bonus.
    speed-=return_attribute_hardiness();

    //If the creature is bloated.
    if(thirst<=THIRST_BLOATED){
        //Being bloated causes a penalty to speed.
        speed+=speed*0.25;
    }

    if(speed<1.0){
        speed=1.0;
    }

    return (short)speed;
}
