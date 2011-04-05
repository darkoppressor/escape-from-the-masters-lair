/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef tile_h
#define tile_h

#include "object.h"

#include <vector>

class Tile: public Object{
    private:
    public:

    //The actual physical type of the tile.
    //In other words, what the tile IS.
    short type;

    //The material that the tile is composed of.
    //In other words, what the tile is MADE OF.
    short material;

    Tile();

    //Render the tile to the screen.
    void render(std::vector< std::vector<bool> >* tile_rendered);

    //Handle anything that needs handling on each game turn.
    void process_turn();
};

#endif
