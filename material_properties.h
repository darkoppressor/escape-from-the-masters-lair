/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef material_properties_h
#define material_properties_h

#include "enumerations.h"

#include <string>

extern double specific_gravities[MATERIAL_LAVA+1];

extern double densities[MATERIAL_LAVA+1];

extern double toughness[MATERIAL_LAVA+1];

extern double values[MATERIAL_LAVA+1];

void prepare_specific_gravities();

void prepare_densities();

void prepare_toughness();

void prepare_values();

std::string material_to_string(short material);

std::string color_to_string(short color);

#endif
