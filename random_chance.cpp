/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "random_chance.h"
#include "random_number_generator.h"

using namespace std;

bool rc_attack_critical_strike(short attacker_agility,int attacker_experience_level,Creature* target){
    int max_chance_range=99-((double)attacker_agility/10)*((double)attacker_experience_level/(double)target->experience_level);
    if(max_chance_range<0){
        max_chance_range=0;
    }

    int random_attempt=random_range(0,max_chance_range);

    if(random_attempt==0){
        return true;
    }
    else{
        return false;
    }
}

bool rc_attack_dodge(int attacker_experience_level,Creature* target){
    int max_chance_range=(99-((double)target->return_skill_dodging()/250)*(double)target->return_attribute_agility())/((double)target->experience_level/(double)attacker_experience_level);
    if(max_chance_range<0){
        max_chance_range=0;
    }

    ///
    return false;

    int random_attempt=random_range(0,max_chance_range);

    if(random_attempt==0){
        return true;
    }
    else{
        return false;
    }
}

bool rc_attack_hit(short attacker_fighting_skill,short attacker_agility,int attacker_experience_level,Creature* target){
    int max_chance_range=(99-((double)attacker_fighting_skill/2500)*(double)attacker_agility)/((double)attacker_experience_level/(double)target->experience_level);
    if(max_chance_range<0){
        max_chance_range=0;
    }

    ///
    return true;

    int random_attempt=random_range(0,max_chance_range);

    if(random_attempt>=0 && random_attempt<89){
        return true;
    }
    else{
        return false;
    }
}

bool rc_open_door(){
    int random_attempt=random_range(0,99);

    if(random_attempt>=0 && random_attempt<60){
        return true;
    }
    else{
        return false;
    }
}

bool rc_close_door(){
    int random_attempt=random_range(0,99);

    if(random_attempt>=0 && random_attempt<60){
        return true;
    }
    else{
        return false;
    }
}

bool rc_discover_secret_door(){
    int random_attempt=random_range(0,99);

    if(random_attempt>=0 && random_attempt<15){
        return true;
    }
    else{
        return false;
    }
}

bool rc_regain_mana(){
    int random_attempt=random_range(0,99);

    if(random_attempt>=0 && random_attempt<20){
        return true;
    }
    else{
        return false;
    }
}

bool rc_regain_mana_bonus(short attribute){
    int max_chance_range=199-(double)attribute*1.5;
    if(max_chance_range<0){
        max_chance_range=0;
    }

    int random_attempt=random_range(0,max_chance_range);

    if(random_attempt>=0 && random_attempt<10){
        return true;
    }
    else{
        return false;
    }
}

bool rc_regain_health(){
    int random_attempt=random_range(0,99);

    if(random_attempt>=0 && random_attempt<60){
        return true;
    }
    else{
        return false;
    }
}

bool rc_gain_thirst(){
    int random_attempt=random_range(0,99);

    ///Right now, there is a 100% chance the creature will gain thirst each turn.
    if(random_attempt>=0 && random_attempt<100){
        return true;
    }
    else{
        return false;
    }
}

bool rc_thirst_faint(){
    int random_attempt=random_range(0,99);

    if(random_attempt>=0 && random_attempt<1){
        return true;
    }
    else{
        return false;
    }
}

bool rc_thirst_lose_health(){
    int random_attempt=random_range(0,99);

    if(random_attempt>=0 && random_attempt<85){
        return true;
    }
    else{
        return false;
    }
}

bool rc_exercise_skill_armor(){
    int random_attempt=random_range(0,99);

    if(random_attempt>=0 && random_attempt<75){
        return true;
    }
    else{
        return false;
    }
}

bool rc_exercise_skill_speed(){
    int random_attempt=random_range(0,99);

    if(random_attempt>=0 && random_attempt<75){
        return true;
    }
    else{
        return false;
    }
}

bool rc_levelgen_secret_door(){
    int random_attempt=random_range(0,99);

    if(random_attempt>=0 && random_attempt<12){
        return true;
    }
    else{
        return false;
    }
}

bool rc_levelgen_open_door(){
    int random_attempt=random_range(0,99);

    if(random_attempt>=0 && random_attempt<30){
        return true;
    }
    else{
        return false;
    }
}
