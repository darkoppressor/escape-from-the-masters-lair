/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "object.h"
#include "tile_size.h"
#include "enumerations.h"

using namespace std;

Object::Object(){
    identifier=0;
    x=0;
    y=0;
    name=" ";
    appearance="";
    color=COLOR_WHITE;
    weight=1;
}

double Object::return_absolute_x(){
    return x*TILE_SIZE;
}

double Object::return_absolute_y(){
    return y*TILE_SIZE;
}
