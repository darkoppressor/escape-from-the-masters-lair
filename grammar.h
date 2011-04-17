/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef grammar_h
#define grammar_h

#include "item.h"
#include "creature.h"

#include <string>

std::string a_vs_an(Item* item);

std::string a_vs_an(Creature* creature);

std::string a_vs_an(short race);

#endif
