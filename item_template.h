/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef item_template_h
#define item_template_h

#include "item.h"

class Item_Template: public Item{
    private:
    public:

    //The size of the item.
    double size;

    //If true, this item will have its size randomly determined upon generation.
    //If false, the size variable will simply be used as-is.
    bool random_size;

    //The materials this item can be.
    //This is only used for items that do not have a color/material defined in their template entry.
    std::vector<short> allowed_materials;

    Item_Template();
};

#endif
