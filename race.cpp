/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "race.h"

using namespace std;

Race::Race(){
    levelup_hp_min=0;
    levelup_hp_max=0;

    levelup_mana_min=0;
    levelup_mana_max=0;

    natural_armor=0;

    prefix_article="a";

    ai_trait_flees=false;
    ai_trait_uses_items=false;
    ai_trait_wanders=true;
}
