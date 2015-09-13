/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef string_input_h
#define string_input_h

#include "render.h"
#include "tile.h"

#include <string>
#include <vector>

#include <SDL.h>

#include <boost/algorithm/string.hpp>

class string_input{
    private:
    public:
    //Storage string.
    std::string str1;

    //The last command strings entered.
    std::vector<std::string> recalled_strings;

    //The currently recalled string.
    //Used for scrolling around in the recalled strings.
    int current_recalled_string;

    string_input();

    void handle_events();

    color_data tile_to_color(Tile* tile);
};

#endif
