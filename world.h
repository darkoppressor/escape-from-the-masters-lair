/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef world_h
#define world_h

#include <string>
#include <sstream>
#include <vector>

#include "level.h"
#include "font.h"
#include "player.h"
#include "game.h"
#include "monster.h"
#include "game_window.h"
#include "random_number_generator.h"
#include "string_input.h"
#include "strings.h"
#include "level_size.h"
#include "fov.h"
#include "tooltip.h"
#include "image.h"
#include "tile_size.h"
#include "dungeon_location.h"
#include "templates.h"

extern Game_Window main_window;

extern Tooltip tooltip;

extern Random_Number_Generator rng;

extern SDL_Event event;

extern BitmapFont font;
extern BitmapFont font_small;

extern std::stringstream ss;
extern std::string msg;

extern string_input string_input_chat;

extern Templates templates;

extern Game game;

extern Player player;

extern std::vector<Level> vector_levels;

extern Image image;

///extern Mix_Chunk *sound_shot;

void load_world();

void unload_world();

#endif
