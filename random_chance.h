/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef random_chance_h
#define random_chance_h

#include "creature.h"

//All of these return true if the action requested succeeded,
//or false if it failed.

//Combat//

bool rc_attack_critical_strike(Creature* attacker,Creature* target);

bool rc_attack_dodge(Creature* attacker,Creature* target);

bool rc_attack_hit(Creature* attacker,Creature* target);

//Level interaction//

bool rc_open_door();

bool rc_close_door();

bool rc_discover_secret_door();

//Turn stuff//

bool rc_regain_mana();

//Level generation//

bool rc_levelgen_secret_door();

bool rc_levelgen_open_door();

#endif
