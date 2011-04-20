/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef random_chance_h
#define random_chance_h

#include "creature.h"

//All of these return true if the action requested succeeded,
//or false if it failed.

//Level interaction//

bool rc_open_door();

bool rc_close_door();

bool rc_discover_secret_door();

bool rc_fountain_dry_up();

//Turn stuff//

///bool rc_regain_mana();

///bool rc_regain_mana_bonus(short attribute);

bool rc_regain_health();

bool rc_gain_thirst();

bool rc_thirst_faint();

bool rc_thirst_lose_health();

//Skills//

bool rc_exercise_skill_armor();

bool rc_exercise_skill_speed();

//Level generation//

bool rc_levelgen_secret_door();

bool rc_levelgen_open_door();

#endif
