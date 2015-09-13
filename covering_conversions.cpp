/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "covering_conversions.h"
#include "enumerations.h"

using namespace std;

short coverings_to_color(Object* object){
    short color=COLOR_RAINBOW;

    if(object->has_covering(COVERING_BLOOD_LOTS) || object->has_covering(COVERING_BLOOD_LITTLE)){
        color=COLOR_BLOOD;
    }
    else if(object->has_covering(COVERING_BLOOD_DRIED)){
        color=COLOR_BLOOD_DRIED;
    }
    else if(object->has_covering(COVERING_ICE)){
        color=COLOR_ICE;
    }
    else if(object->has_covering(COVERING_WATER)){
        color=COLOR_WATER;
    }
    else if(object->has_covering(COVERING_FIRE)){
        color=COLOR_FLAME;
    }

    return color;
}

string covering_to_string(short covering){
    string line="";

    if(covering==COVERING_ICE){
        line="ice";
    }
    else if(covering==COVERING_WATER){
        line="water";
    }
    else if(covering==COVERING_FIRE){
        line="fire";
    }
    else if(covering==COVERING_BLOOD_LOTS){
        line="blood";
    }
    else if(covering==COVERING_BLOOD_LITTLE){
        line="blood";
    }
    else if(covering==COVERING_BLOOD_DRIED){
        line="dried blood";
    }

    return line;
}
