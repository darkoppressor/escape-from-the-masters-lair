/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"
#include "random_number_generator.h"
#include "world.h"
#include "message_log.h"

using namespace std;

void Creature::level_up(){
}

void Creature::gain_experience(short killed_experience_level){
    short experience_gained=random_range(killed_experience_level*14,killed_experience_level*18);

    experience+=experience_gained;

    ss.clear();ss.str("");ss<<"You gain ";ss<<experience_gained;ss<<" experience points!";msg=ss.str();

    update_text_log(msg.c_str(),is_player);

    //If experience reaches the current maximum experience.
    if(experience>=experience_max){
        //The creature levels up.
        level_up();
    }
}
