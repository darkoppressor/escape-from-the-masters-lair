/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef level_h
#define level_h

#include "tile.h"
#include "level_size.h"
#include "item.h"
#include "light_data.h"
#include "monster.h"

#include <vector>

#include <SDL.h>
#include <SDL_mixer.h>

class Level{
    private:
    public:
    Level(short get_level_x,short get_level_y);

    //Returns the absolute (i.e., not in terms of tiles, but of pixels) x dimension of the level.
    double return_absolute_x();

    //Returns the absolute (i.e., not in terms of tiles, but of pixels) y dimension of the level.
    double return_absolute_y();

    //The dimensions of the level map, in tiles.
    short level_x,level_y;

    //The temperature of this level, in celsius.
    short temperature;

    std::vector< std::vector<Tile> > tiles;

    std::vector< std::vector<char> > fog;

    std::vector<Item> items;
    std::vector<Monster> monsters;

    void setSeen(unsigned short x, unsigned short y,light_data source_data);
	bool onMap(unsigned short x, unsigned short y);
	bool blockLOS(unsigned short x, unsigned short y);
};

#endif
