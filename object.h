/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef object_h
#define object_h

#include "covering.h"

#include <string>
#include <vector>
#include <stdint.h>

//An object is anything that is going to be rendered in the world. This means the tiles, the items, and even the player and npcs.
class Object{
    private:
    public:

    //The object's unique identifier.
    uint32_t identifier;

    Object();

    //Returns the absolute (i.e., not in terms of tiles, but of pixels) x position of the object.
    double return_absolute_x();

    //Returns the absolute (i.e., not in terms of tiles, but of pixels) y position of the object.
    double return_absolute_y();

    //Returns true if the object with passed coordinates is within 1 tile.
    //Returns false if the object is farther away.
    bool object_nearby(short object_x,short object_y,int range);

    //Returns true if the passed covering is on the object.
    //Returns false if the passed covering is not on the object.
    bool has_covering(short covering);

    //Adds the passed covering.
    void add_covering(short covering);

    //Removes the passed covering.
    //Returns true if the covering existed and was thus removed.
    //Returns false if the covering did not exist and thus was not removed.
    bool remove_covering(short covering);

    //The current coordinates of the object, in tiles.
    short x,y;

    //The object's name.
    std::string name;

    //The object's appearance.
    std::string appearance;

    //The color of the item.
    short color;

    //The weight of the object.
    double weight;

    //The coverings on this object.
    std::vector<Covering> coverings;

    bool alive;
};

#endif
