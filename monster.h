/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef monster_h
#define monster_h

#include "creature.h"

class Monster: public Creature{
    private:

    //Virtual AI "keystates".
    bool ai_keystates[256];

    public:

    //Setup the monster's starting inventory.
    void set_inventory();

    //Set the base stats of the monster.
    void set_base_stats(short pass_level);

    //Get AI.
    void handle_ai();

    //Handle input.
    void handle_input();

    //Move the monster.
    void move();

    //Render the monster.
    void render(std::vector< std::vector<bool> >* tile_rendered);
};

#endif
