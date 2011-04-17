/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef race_h
#define race_h

#include "creature.h"

class Race: public Creature{
    private:
    public:

    int levelup_hp_min;
    int levelup_hp_max;

    int levelup_mana_min;
    int levelup_mana_max;

    int natural_armor;

    //A list of inventory item names.
    //These are the items that a member of this race starts with.
    std::vector<std::string> inventory_items;

    //AI Traits:

    bool ai_trait_flees;
    bool ai_trait_uses_items;
    bool ai_trait_wanders;

    Race();
};

#endif
