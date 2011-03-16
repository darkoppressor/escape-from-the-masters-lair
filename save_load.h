/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef save_load_h
#define save_load_h

#include <string>
#include <fstream>
#include <boost/filesystem.hpp>

void create_top_level_directories();

void create_directory(std::string directory);

void remove_directory(std::string directory);

void delete_file(std::string file);

void save_game();

void load_game();

bool load_commands();

bool save_commands();

bool options_load();

bool options_save();

#endif
