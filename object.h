/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef object_h
#define object_h

#include <string>

//An object is anything that is going to be rendered in the world. This means the tiles, the items, and even the player and npcs.
class Object{
    private:
    public:

    Object();

    //Returns the absolute (i.e., not in terms of tiles, but of pixels) x position of the object.
    double return_absolute_x();

    //Returns the absolute (i.e., not in terms of tiles, but of pixels) y position of the object.
    double return_absolute_y();

    //The current coordinates of the object, in tiles.
    short x,y;

    //The object's name.
    std::string name;

    //The object's appearance.
    std::string appearance;

    //The color of the item.
    short color;
};

#endif
