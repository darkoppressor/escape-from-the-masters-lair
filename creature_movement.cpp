/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"

using namespace std;

unsigned short Creature::return_movement_speed(){
    double speed=movement_speed;

    //If the creature is bloated.
    if(thirst<=THIRST_BLOATED){
        speed+=speed*0.25;
    }

    if(speed<0.0){
        speed=0.0;
    }

    return (unsigned short)speed;
}

short Creature::return_next_move(){
    double nextmove=next_move;

    //If the creature is bloated.
    if(thirst<=THIRST_BLOATED){
        nextmove+=nextmove*0.25;
    }

    if(nextmove<0.0){
        nextmove=0.0;
    }

    return (short)nextmove;
}
