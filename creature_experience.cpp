/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"
#include "random_number_generator.h"
#include "world.h"
#include "message_log.h"

using namespace std;

///This function will need to be updated for the new game to take into account all attributes.
void Creature::update_class_name(){
    class_name="";

    if(return_attribute_strength()==return_attribute_agility()==return_attribute_hardiness()){
        if(return_attribute_strength()==1){
            class_name="beginner";
        }
        else{
            class_name="jack of all trades";
        }

        return;
    }

    //Determine the highest attribute.
    short largest_attribute=ATTRIBUTE_STRENGTH;

    if(return_attribute_agility()>return_attribute_strength()){
        largest_attribute=ATTRIBUTE_AGILITY;
    }
    if(return_attribute_hardiness()>return_attribute_agility()){
        largest_attribute=ATTRIBUTE_HARDINESS;
    }

    if(largest_attribute==ATTRIBUTE_STRENGTH){
        if(return_attribute_strength()<5){
           class_name="scuffler";
        }
        else if(return_attribute_strength()>=5 && return_attribute_strength()<10){
           class_name="street tough";
        }
        else if(return_attribute_strength()>=10 && return_attribute_strength()<25){
           class_name="fighter";
        }
        else if(return_attribute_strength()>=25 && return_attribute_strength()<50){
           class_name="soldier";
        }
        else if(return_attribute_strength()>=50 && return_attribute_strength()<75){
           class_name="warrior-in-training";
        }
        else if(return_attribute_strength()>=75 && return_attribute_strength()<100){
           class_name="warrior";
        }
        else if(return_attribute_strength()>=100){
           class_name="knight";
        }
    }
    else if(largest_attribute==ATTRIBUTE_AGILITY){
        if(return_attribute_agility()<5){
           class_name="circus performer";
        }
        else if(return_attribute_agility()>=5 && return_attribute_agility()<10){
           class_name="acrobat";
        }
        else if(return_attribute_agility()>=10 && return_attribute_agility()<25){
           class_name="speedster";
        }
        else if(return_attribute_agility()>=25 && return_attribute_agility()<50){
           class_name="chucker";
        }
        else if(return_attribute_agility()>=50 && return_attribute_agility()<75){
           class_name="hurler";
        }
        else if(return_attribute_agility()>=75 && return_attribute_agility()<100){
           class_name="sharpshooter";
        }
        else if(return_attribute_agility()>=100){
           class_name="olympic gold medalist";
        }
    }
    else if(largest_attribute==ATTRIBUTE_HARDINESS){
        if(return_attribute_hardiness()<5){
           class_name="tourist";
        }
        else if(return_attribute_hardiness()>=5 && return_attribute_hardiness()<10){
           class_name="outdoorsy type";
        }
        else if(return_attribute_hardiness()>=10 && return_attribute_hardiness()<25){
           class_name="survivor";
        }
        else if(return_attribute_hardiness()>=25 && return_attribute_hardiness()<50){
           class_name="hunter";
        }
        else if(return_attribute_hardiness()>=50 && return_attribute_hardiness()<75){
           class_name="ranger";
        }
        else if(return_attribute_hardiness()>=75 && return_attribute_hardiness()<100){
           class_name="forester";
        }
        else if(return_attribute_hardiness()>=100){
           class_name="master of survival";
        }
    }
}

bool Creature::levelup_is_selected_attribute(short attribute){
    //Look through all of the selected attributes.
    for(int i=0;i<levelup_attributes.size();i++){
        if(attribute==levelup_attributes[i]){
            return true;
        }
    }

    return false;
}

bool Creature::levelup_all_attributes_set(){
    //Look through all of the selected attributes.
    for(int i=0;i<levelup_attributes.size();i++){
        //If the attribute is not set.
        if(levelup_attributes[i]==-1){
            return false;
        }
    }

    return true;
}

void Creature::level_up(){
    //Increase the max experience.
    experience_max+=experience_max*0.05;

    //Reset experience.
    experience=0;

    //Increase the creature's experience level.
    experience_level++;

    //Increase max health.

    int health_gain=0;

    //Apply the base increase.
    health_gain+=random_range(templates.base_stats.levelup_hp_min,templates.base_stats.levelup_hp_max);

    //Apply the racial modifier.
    //If the race has a levelup health bonus.
    if(templates.template_races[race].levelup_hp_max>=0){
        health_gain+=random_range(templates.template_races[race].levelup_hp_min,templates.template_races[race].levelup_hp_max);
    }
    //If the race has a levelup health penalty.
    else{
        health_gain-=random_range(abs(templates.template_races[race].levelup_hp_min),abs(templates.template_races[race].levelup_hp_max));
    }

    if(health_gain<0){
        health_gain=0;
    }

    health_max+=health_gain;

    //Increase max mana.

    int mana_gain=0;

    //Apply the base increase.
    mana_gain+=random_range(templates.base_stats.levelup_mana_min,templates.base_stats.levelup_mana_max);

    //Apply the racial modifier.
    //If the race has a levelup mana bonus.
    if(templates.template_races[race].levelup_mana_max>=0){
        mana_gain+=random_range(templates.template_races[race].levelup_mana_min,templates.template_races[race].levelup_mana_max);
    }
    //If the race has a levelup mana penalty.
    else{
        mana_gain-=random_range(templates.template_races[race].levelup_mana_min,templates.template_races[race].levelup_mana_max);
    }

    if(mana_gain<0){
        mana_gain=0;
    }

    mana_max+=mana_gain;

    //Determine whether 1, 2, or 3 attributes are to be improved this level.
    int attributes_to_improve=(experience_level%3)+1;

    //Prepare the level up attributes vector for this level.
    levelup_attributes.clear();
    for(int i=0;i<attributes_to_improve;i++){
        levelup_attributes.push_back(-1);
    }

    //Don't allow any attribute bonus to pass the maximum.
    for(int i=ATTRIBUTE_STRENGTH;i<ATTRIBUTE_LUCK+1;i++){
        if(attribute_level_bonuses[i]>=1 && attribute_level_bonuses[i]<=4){
            attribute_level_bonuses[i]=1;
        }
        else if(attribute_level_bonuses[i]>=5 && attribute_level_bonuses[i]<=7){
            attribute_level_bonuses[i]=2;
        }
        else if(attribute_level_bonuses[i]>=8 && attribute_level_bonuses[i]<=9){
            attribute_level_bonuses[i]=3;
        }
        else if(attribute_level_bonuses[i]>=10){
            attribute_level_bonuses[i]=4;
        }
    }

    ss.clear();ss.str("");ss<<"Welcome to experience level ";ss<<experience_level;ss<<".";msg=ss.str();

    update_text_log(msg.c_str(),is_player);

    if(is_player){
        player.current_window=WINDOW_LEVELUP;
    }
    else{
        //Select the monster's attributes to improve and improve them.

        ///Right now this just prioritizes attributes based on their order, but the monster should be somewhat intelligent about this.
        //Determine the attributes to improve.
        for(int i=0;i<levelup_attributes.size();i++){
            levelup_attributes[i]=i;
        }

        //Apply the selected attribute bonuses.
        for(int i=0;i<levelup_attributes.size();i++){
            attributes[levelup_attributes[i]]+=1+attribute_level_bonuses[levelup_attributes[i]];
        }

        //Clear the attribute bonuses.
        for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
            attribute_level_bonuses[i]=0;
        }

        //If experience reaches the current maximum experience.
        if(experience>=experience_max){
            //The creature levels up again.
            level_up();
        }
    }
}

void Creature::gain_experience(int points_gained){
    experience+=points_gained;

    //If experience reaches the current maximum experience.
    if(experience>=experience_max){
        //The creature levels up.
        level_up();
    }
}

void Creature::level_up_skill(short skill,int experience_gained){
    //If this skill is a focused skill.
    if(is_focused_skill(skill)){
        //Apply the focused skill bonus to creature experience gain.
        experience_gained*=2.0;
    }

    //The creature gains experience for gaining a skill level.
    gain_experience(experience_gained);

    //Increase the skill's max experience.
    skills[skill][SKILL_EXPERIENCE_MAX]+=skills[skill][SKILL_EXPERIENCE_MAX]*0.025;

    //Reset the skill's experience.
    skills[skill][SKILL_EXPERIENCE]=0;

    //Increase the skill's experience level.
    skills[skill][SKILL_EXPERIENCE_LEVEL]++;

    //Increment the appropriate attribute bonus.

    //If the skill is governed by strength.
    if(skill==SKILL_BLADED_WEAPONS || skill==SKILL_BLUNT_WEAPONS || skill==SKILL_STABBING_WEAPONS || skill==SKILL_UNARMED){
        attribute_level_bonuses[ATTRIBUTE_STRENGTH]++;
    }
    //If the skill is governed by agility.
    else if(skill==SKILL_SECURITY || skill==SKILL_STEALTH || skill==SKILL_LAUNCHER_WEAPONS || skill==SKILL_THROWN_WEAPONS ||
            skill==SKILL_DUAL_WIELDING || skill==SKILL_SPEED){
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

    //If the skill's experience reaches its current maximum experience.
    if(skills[skill][SKILL_EXPERIENCE]>=skills[skill][SKILL_EXPERIENCE_MAX]){
        //The skill levels up.
        level_up_skill(skill,experience_gained);
    }
}

void Creature::gain_skill_experience(short skill,int points_gained,int experience_gained,bool allow_focused_bonus,bool enable_pacing){
    if(enable_pacing){
        //Set the pace of skill leveling.
        points_gained*=3.0;
    }

    //As long as at least one point is being gained.
    if(points_gained>0){
        //If this skill is a focused skill.
        if(allow_focused_bonus && is_focused_skill(skill)){
            //Apply the focused skill bonus to this skill's experience gain.
            points_gained*=2.0;
        }

        //The creature gains experience for gaining skill experience.
        gain_experience(experience_gained);

        //Add points_gained to the skill's experience.
        skills[skill][SKILL_EXPERIENCE]+=points_gained;

        //If the skill's experience reaches its current maximum experience.
        if(skills[skill][SKILL_EXPERIENCE]>=skills[skill][SKILL_EXPERIENCE_MAX]){
            //The skill levels up.
            level_up_skill(skill,experience_gained*5.0);
        }
    }
}
