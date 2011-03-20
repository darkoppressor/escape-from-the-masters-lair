/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"

using namespace std;

short Creature::return_skill_bladed_weapons(){
    double skill=skills[SKILL_BLADED_WEAPONS];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_blunt_weapons(){
    double skill=skills[SKILL_BLUNT_WEAPONS];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_stabbing_weapons(){
    double skill=skills[SKILL_STABBING_WEAPONS];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_unarmed(){
    double skill=skills[SKILL_UNARMED];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_security(){
    double skill=skills[SKILL_SECURITY];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_stealth(){
    double skill=skills[SKILL_STEALTH];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_launcher_weapons(){
    double skill=skills[SKILL_LAUNCHER_WEAPONS];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_thrown_weapons(){
    double skill=skills[SKILL_THROWN_WEAPONS];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_dual_wielding(){
    double skill=skills[SKILL_DUAL_WIELDING];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_speed(){
    double skill=skills[SKILL_SPEED];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_fighting(){
    double skill=skills[SKILL_FIGHTING];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_dodging(){
    double skill=skills[SKILL_DODGING];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_armor(){
    double skill=skills[SKILL_ARMOR];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_magic_air(){
    double skill=skills[SKILL_MAGIC_AIR];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_magic_fire(){
    double skill=skills[SKILL_MAGIC_FIRE];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_magic_water(){
    double skill=skills[SKILL_MAGIC_WATER];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_magic_cold(){
    double skill=skills[SKILL_MAGIC_COLD];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_magic_earth(){
    double skill=skills[SKILL_MAGIC_EARTH];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_magic_conjuration(){
    double skill=skills[SKILL_MAGIC_CONJURATION];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_magic_enchantment(){
    double skill=skills[SKILL_MAGIC_ENCHANTMENT];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}

short Creature::return_skill_magic_summoning(){
    double skill=skills[SKILL_MAGIC_SUMMONING];

    ///

    if(skill<1.0){
        skill=1.0;
    }

    return (short)skill;
}
