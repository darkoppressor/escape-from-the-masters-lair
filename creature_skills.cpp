/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"
#include "world.h"

using namespace std;

bool Creature::is_focused_skill(short skill){
    //If this skill is a focused skill.
    if(skill==focused_skills[0] || skill==focused_skills[1] || skill==focused_skills[2]){
        return true;
    }

    return false;
}

int Creature::return_skill_bladed_weapons(){
    double skill=skills[SKILL_BLADED_WEAPONS][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_BLADED_WEAPONS][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_blunt_weapons(){
    double skill=skills[SKILL_BLUNT_WEAPONS][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_BLUNT_WEAPONS][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_stabbing_weapons(){
    double skill=skills[SKILL_STABBING_WEAPONS][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_STABBING_WEAPONS][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_unarmed(){
    double skill=skills[SKILL_UNARMED][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_UNARMED][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

/**int Creature::return_skill_security(){
    double skill=skills[SKILL_SECURITY][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_SECURITY][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}*/

/**int Creature::return_skill_stealth(){
    double skill=skills[SKILL_STEALTH][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_STEALTH][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}*/

int Creature::return_skill_launcher_weapons(){
    double skill=skills[SKILL_LAUNCHER_WEAPONS][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_LAUNCHER_WEAPONS][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_thrown_weapons(){
    double skill=skills[SKILL_THROWN_WEAPONS][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_THROWN_WEAPONS][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_dual_wielding(){
    double skill=skills[SKILL_DUAL_WIELDING][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_DUAL_WIELDING][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_speed(){
    double skill=skills[SKILL_SPEED][SKILL_EXPERIENCE_LEVEL];

    //If the creature is anything aside from unencumbered.
    if(!(return_inventory_weight()<=return_carry_capacity())){
        //Apply the encumbrance penalty.
        skill-=(return_inventory_weight()-return_carry_capacity())*0.1;
    }

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_SPEED][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

/**int Creature::return_skill_fighting(){
    double skill=skills[SKILL_FIGHTING][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_FIGHTING][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_dodging(){
    double skill=skills[SKILL_DODGING][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_DODGING][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}*/

int Creature::return_skill_armor(){
    double skill=skills[SKILL_ARMOR][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_ARMOR][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

/**int Creature::return_skill_magic_air(){
    double skill=skills[SKILL_MAGIC_AIR][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_MAGIC_AIR][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_magic_fire(){
    double skill=skills[SKILL_MAGIC_FIRE][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_MAGIC_FIRE][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_magic_water(){
    double skill=skills[SKILL_MAGIC_WATER][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_MAGIC_WATER][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_magic_cold(){
    double skill=skills[SKILL_MAGIC_COLD][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_MAGIC_COLD][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_magic_earth(){
    double skill=skills[SKILL_MAGIC_EARTH][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_MAGIC_EARTH][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_magic_conjuration(){
    double skill=skills[SKILL_MAGIC_CONJURATION][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_MAGIC_CONJURATION][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_magic_enchantment(){
    double skill=skills[SKILL_MAGIC_ENCHANTMENT][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_MAGIC_ENCHANTMENT][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}

int Creature::return_skill_magic_summoning(){
    double skill=skills[SKILL_MAGIC_SUMMONING][SKILL_EXPERIENCE_LEVEL];

    //Apply the racial modifier.
    skill+=templates.template_races[race].skills[SKILL_MAGIC_SUMMONING][SKILL_EXPERIENCE_LEVEL];

    if(skill<1.0){
        skill=1.0;
    }

    return (int)skill;
}*/
