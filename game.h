/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef game_h
#define game_h

#include "tile.h"
#include "level_size.h"
#include "item.h"
#include "monster.h"

#include <vector>

#include <SDL.h>
#include <SDL_mixer.h>

class Game{
    private:
    //Background music will be loaded into this variable.
    Mix_Music *music;

    short generated_temperature;

    std::vector< std::vector<Tile> > generated_tiles;

    std::vector<Item> generated_items;
    std::vector<Monster> generated_monsters;

    public:
    //The default constructor. It does not currently do anything.
    Game();

    //The destructor frees all of the level files that need freeing.
    ~Game();

    //Returns true if the checked tile is a wall.
    ///short tile_check(short check_x,short check_y);

    //This function loads whatever level is needed from the corresponding level file.
    void generate_level();

    void draw_rectangle_room(short start_x,short start_y,short size_x,short size_y,short type,short material);

    void clear_rectangle(short center_x,short center_y,short size,short type_to_clear,short type_replacement,short material_replacement);

    //Returns true if rectangle has nothing but type_wanted.
    //Returns false otherwise.
    bool check_rectangle(short start_x,short start_y,short size_x,short size_y,short type_wanted);
    bool check_rectangle2(short start_x,short start_y,short size_x,short size_y,short type_wanted,short type_wanted2);

    bool check_circle(short center_x,short center_y,short max_radius,short type_wanted);

    void draw_circle(short center_x,short center_y,short max_radius,short type,short material);

    //This function is run whenever the level is changed.
    //If the level already exists when the player goes there, only this is run.
    //If the level must first be generated, generate_level() is run, and then change_level().
    void change_level(short direction);

    short generated_level_x,generated_level_y;
};

#endif
