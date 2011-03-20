/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"

using namespace std;

int Creature::return_health(){
    double number=health;

    //Apply the hardiness bonus.
    number+=number*((double)return_attribute_hardiness()/100.0);

    return (int)number;
}

int Creature::return_health_max(){
    double number=health_max;

    //Apply the hardiness bonus.
    number+=number*((double)return_attribute_hardiness()/100.0);

    return (int)number;
}

int Creature::return_mana(){
    double number=mana;

    //Apply the comprehension bonus.
    number+=number*((double)return_attribute_comprehension()/100.0);

    //Apply the acumen bonus.
    number+=number*(((double)return_attribute_acumen()/2.0)/100.0);

    return (int)number;
}

int Creature::return_mana_max(){
    double number=mana_max;

    //Apply the comprehension bonus.
    number+=number*((double)return_attribute_comprehension()/100.0);

    //Apply the acumen bonus.
    number+=number*(((double)return_attribute_acumen()/2.0)/100.0);

    return (int)number;
}

unsigned short Creature::return_movement_speed(){
    double speed=movement_speed;

    //Subtract the agility bonus.
    speed-=return_attribute_agility()*2;

    //Subtract the hardiness bonus.
    speed-=return_attribute_hardiness();

    //Subtract the speed skill bonus.
    speed-=return_skill_speed()*3;

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

    return (short)speed;
}
