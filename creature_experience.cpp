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

    ///Determine whether 1, 2, or 3 attributes are to be improved this level.
    ///int attributes_to_improve=;

    ///The creature must now select its attributes to improve.

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

void Creature::level_up_skill(short skill,int experience_gained){
    //If this skill is a focused skill.
    if(skill==focused_skills[0] || skill==focused_skills[1] || skill==focused_skills[2]){
        //Apply the focused skill bonus to creature experience gain.
        experience_gained*=2.0;
    }

    //The creature gains experience for gaining a skill level.
    gain_experience(experience_gained);

    //Increase the skill's max experience.
    skills[skill][SKILL_EXPERIENCE_MAX]+=skills[skill][SKILL_EXPERIENCE_MAX]*0.25;

    //Increase the skill's experience level.
    skills[skill][SKILL_EXPERIENCE_LEVEL]++;

    //Increment the appropriate attribute bonus.

    //If the skill is governed by strength.
    if(skill==SKILL_BLADED_WEAPONS || skill==SKILL_BLUNT_WEAPONS || skill==SKILL_STABBING_WEAPONS || skill==SKILL_UNARMED){
        attribute_level_bonuses[ATTRIBUTE_STRENGTH]++;
    }
    //If the skill is governed by agility.
    else if(skill==SKILL_SECURITY || skill==SKILL_STEALTH || skill==SKILL_LAUNCHER_WEAPONS || skill==SKILL_THROWN_WEAPONS ||
            skill==SKILL_DUAL_WIELDING || skill==SKILL_SPEED || skill==SKILL_FIGHTING || skill==SKILL_DODGING){
        attribute_level_bonuses[ATTRIBUTE_AGILITY]++;
    }
    //If the skill is governed by hardiness.
    else if(skill==SKILL_ARMOR){
        attribute_level_bonuses[ATTRIBUTE_HARDINESS]++;
    }
    //If the skill is governed by comprehension.
    else if(skill==SKILL_MAGIC_AIR || skill==SKILL_MAGIC_FIRE || skill==SKILL_MAGIC_WATER || skill==SKILL_MAGIC_COLD){
        attribute_level_bonuses[ATTRIBUTE_COMPREHENSION]++;
    }
    //If the skill is governed by acumen.
    else if(skill==SKILL_MAGIC_EARTH || skill==SKILL_MAGIC_CONJURATION || skill==SKILL_MAGIC_ENCHANTMENT || skill==SKILL_MAGIC_SUMMONING){
        attribute_level_bonuses[ATTRIBUTE_ACUMEN]++;
    }

    ///ss.clear();ss.str("");ss<<"You have gained a skill level!";msg=ss.str();

    ///update_text_log(msg.c_str(),is_player);

    //If the skill's experience reaches its current maximum experience.
    if(skills[skill][SKILL_EXPERIENCE]>=skills[skill][SKILL_EXPERIENCE_MAX]){
        //The skill levels up.
        level_up_skill(skill,experience_gained);
    }
}

void Creature::gain_skill_experience(short skill,int points_gained,int experience_gained){
    //If this skill is a focused skill.
    if(skill==focused_skills[0] || skill==focused_skills[1] || skill==focused_skills[2]){
        //Apply the focused skill bonus to this skill's experience gain.
        points_gained*=2.0;
    }

    //The creature gains experience for gaining skill experience.
    gain_experience(experience_gained);

    //Add points_gained to the skill's experience.
    skills[skill][SKILL_EXPERIENCE]+=points_gained;

    ///ss.clear();ss.str("");ss<<"You gain ";ss<<points_gained;ss<<" experience points!";msg=ss.str();

    ///update_text_log(msg.c_str(),is_player);

    //If the skill's experience reaches its current maximum experience.
    if(skills[skill][SKILL_EXPERIENCE]>=skills[skill][SKILL_EXPERIENCE_MAX]){
        //The skill levels up.
        level_up_skill(skill,experience_gained*5.0);
    }
}
