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

    Race();
};

#endif
