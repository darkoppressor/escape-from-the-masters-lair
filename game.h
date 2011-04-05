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

    short level_variations[LEVEL_VARIATION_LAVA+1];

    short generated_temperature;

    short level_theme;

    std::vector< std::vector<Tile> > generated_tiles;

    std::vector< std::vector<short> > generated_fog;

    //Used in testing walkable tiles' reachability.
    std::vector< std::vector<bool> > tiles_check;

    std::vector<Item> generated_items;

    std::vector<Monster> generated_monsters;

    public:

    //Available identifiers for objects.
    //0 is reserved to mean "no identifier assigned."
    std::vector< std::vector<uint32_t> > identifiers;

    //The default constructor. It does not currently do anything.
    Game();

    //The destructor frees all of the level files that need freeing.
    ~Game();

    //Start a new game.
    void new_game();

    //Load a saved game.
    void old_game();

    //Prepare the identifier lists.
    void prepare_identifiers();

    //Returns an unused identifier to be assigned to an object.
    uint32_t assign_identifier(short object_type);

    //Returns an identifier to the list.
    void return_identifier(short object_type,uint32_t returning_identifier);

    //Returns true if the checked tile is a wall.
    ///short tile_check(short check_x,short check_y);

    //Recursively fills the whole generated dungeon with "reachable" flags for each walkable tile.
    void flood_fill_reachable(int x,int y);

    //Returns true if all walkable tiles in the generated level are reachable.
    //Returns false if one or more walkable tiles are unreachable.
    bool all_tiles_reachable();

    void level_theme_rooms();
    void level_theme_caves();
    void level_theme_big_room();
    void level_theme_maze();

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
