#include "random_chance.h"
#include "random_number_generator.h"

using namespace std;

bool rc_attack_critical_strike(Creature* attacker,Creature* target){
    int max_chance_range=99-(attacker->attributes[ATTRIBUTE_AGILITY]/10)*(attacker->experience_level/target->experience_level);
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

bool rc_attack_dodge(Creature* attacker,Creature* target){
    int max_chance_range=(99-(target->skills[SKILL_DODGING]/250)*target->attributes[ATTRIBUTE_AGILITY])/(target->experience_level/attacker->experience_level);
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

bool rc_attack_hit(Creature* attacker,Creature* target){
    int max_chance_range=(99-(attacker->skills[SKILL_FIGHTING]/2500)*attacker->attributes[ATTRIBUTE_AGILITY])/(attacker->experience_level/target->experience_level);
    if(max_chance_range<0){
        max_chance_range=0;
    }

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

    if(random_attempt>=0 && random_attempt<5){
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
