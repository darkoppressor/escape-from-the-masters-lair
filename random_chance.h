/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef random_chance_h
#define random_chance_h

#include "creature.h"

//All of these return true if the action requested succeeded,
//or false if it failed.

//Combat//

bool rc_attack_critical_strike(short attacker_agility,short attacker_experience_level,Creature* target);

bool rc_attack_dodge(short attacker_experience_level,Creature* target);

bool rc_attack_hit(short attacker_fighting_skill,short attacker_agility,short attacker_experience_level,Creature* target);

//Level interaction//

bool rc_open_door();

bool rc_close_door();

bool rc_discover_secret_door();

//Turn stuff//

bool rc_regain_mana();

bool rc_gain_thirst();

//Level generation//

bool rc_levelgen_secret_door();

bool rc_levelgen_open_door();

#endif
