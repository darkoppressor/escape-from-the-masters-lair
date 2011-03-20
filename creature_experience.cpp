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

    ///ss.clear();ss.str("");ss<<"You gain ";ss<<points_gained;ss<<" experience points!";msg=ss.str();

    ///update_text_log(msg.c_str(),is_player);

    //If experience reaches the current maximum experience.
    if(experience>=experience_max){
        //The creature levels up.
        level_up();
    }
}

void Creature::level_up_skill(short skill){
    //The creature gains experience for gaining a skill level.
    gain_experience(5);

    //Increase the skill's max experience.
    skills[skill][SKILL_EXPERIENCE_MAX]+=skills[skill][SKILL_EXPERIENCE_MAX]*0.5;

    //Increase the skill's experience level.
    skills[skill][SKILL_EXPERIENCE_LEVEL]++;

    ///ss.clear();ss.str("");ss<<"You have gained a level!";msg=ss.str();

    ///update_text_log(msg.c_str(),is_player);

    //If the skill's experience reaches its current maximum experience.
    if(skills[skill][SKILL_EXPERIENCE]>=skills[skill][SKILL_EXPERIENCE_MAX]){
        //The skill levels up.
        level_up_skill(skill);
    }
}

void Creature::gain_skill_experience(short skill,int points_gained){
    //The creature gains experience for gaining skill experience.
    gain_experience(1);

    //Add points_gained to the skill's experience.
    skills[skill][SKILL_EXPERIENCE]+=points_gained;

    ///ss.clear();ss.str("");ss<<"You gain ";ss<<points_gained;ss<<" experience points!";msg=ss.str();

    ///update_text_log(msg.c_str(),is_player);

    //If the skill's experience reaches its current maximum experience.
    if(skills[skill][SKILL_EXPERIENCE]>=skills[skill][SKILL_EXPERIENCE_MAX]){
        //The skill levels up.
        level_up_skill(skill);
    }
}
