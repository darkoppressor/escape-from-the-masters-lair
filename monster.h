/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef monster_h
#define monster_h

#include "creature.h"

struct coordinates{
    short x;
    short y;
};

struct ai_action{
    short action;
    coordinates coords;
};

class Monster: public Creature{
    private:

    //Virtual AI "keystates".
    bool ai_keystates[256];

    public:

    //Setup the monster's starting inventory.
    void set_inventory();

    //Set the base stats of the monster.
    void set_base_stats(short pass_level);

    //Handle AI.
    void handle_ai();

    //Handle input.
    void handle_input();

    //Move the monster.
    void move();

    //Render the monster.
    void render(std::vector< std::vector<bool> >* tile_rendered);

    //Check states and determine the action the ai wishes to carry out.
    ai_action ai_determine_action();

    //
    coordinates ai_nearest_hostile();

    //
    coordinates ai_opposite_direction(short target_x,short target_y);

    //AI States:

    bool ai_state_thirsty();
    bool ai_state_low_health();
    bool ai_state_hostile_nearby(short hostile_x,short hostile_y);
};

#endif
