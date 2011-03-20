/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"
#include "random_number_generator.h"
#include "world.h"
#include "message_log.h"

using namespace std;

void Creature::level_up(){
    //Increase the max experience.
    experience_max+=experience_max*1.1;

    //Increase the creature's experience level.
    experience_level++;

    //Increase max health.
    health_max*=1.075;

    //Increase max mana.
    mana_max*=1.075;

    ss.clear();ss.str("");ss<<"You have gained a level!";msg=ss.str();

    update_text_log(msg.c_str(),is_player);

    //If experience reaches the current maximum experience.
    if(experience>=experience_max){
        //The creature levels up.
        level_up();
    }
}

void Creature::gain_experience(int points_gained){
    experience+=points_gained;

    ss.clear();ss.str("");ss<<"You gain ";ss<<points_gained;ss<<" experience points!";msg=ss.str();

    update_text_log(msg.c_str(),is_player);

    //If experience reaches the current maximum experience.
    if(experience>=experience_max){
        //The creature levels up.
        level_up();
    }
}
