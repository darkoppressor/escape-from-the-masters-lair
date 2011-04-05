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
    name="";
    appearance="";
    color=COLOR_WHITE;
    weight=1.0;
    alive=true;
}

double Object::return_absolute_x(){
    return x*TILE_SIZE_X;
}

double Object::return_absolute_y(){
    return y*TILE_SIZE_Y;
}

bool Object::object_nearby(short object_x,short object_y,int range){
    //Check all objects within range tiles of the object.
    for(int int_x=x-range;int_x<x+range+1;int_x++){
        for(int int_y=y-range;int_y<y+range+1;int_y++){
            if(int_x==object_x && int_y==object_y){
                return true;
            }
        }
    }

    return false;
}

bool Object::has_covering(short covering){
    for(int i=0;i<coverings.size();i++){
        if(coverings[i].type==covering){
            return true;
        }
    }

    return false;
}

void Object::add_covering(short covering){
    //If the object doesn't already have this covering.
    if(!has_covering(covering)){
        bool create_covering=true;

        if(covering==COVERING_WATER){
            remove_covering(COVERING_BLOOD_LOTS);
            remove_covering(COVERING_BLOOD_LITTLE);
            remove_covering(COVERING_BLOOD_DRIED);

            if(remove_covering(COVERING_FIRE)){
                create_covering=false;
            }

            if(has_covering(COVERING_ICE)){
                create_covering=false;
            }
        }
        else if(covering==COVERING_ICE){
            remove_covering(COVERING_WATER);

            if(remove_covering(COVERING_FIRE)){
                create_covering=false;
            }
        }
        else if(covering==COVERING_FIRE){
            if(remove_covering(COVERING_WATER)){
                create_covering=false;
            }

            if(remove_covering(COVERING_ICE)){
                create_covering=false;
            }
        }

        //If nothing prevented the new covering from being added.
        if(create_covering){
            //Add the new covering.
            coverings.push_back(covering);
        }
    }
}

bool Object::remove_covering(short covering){
    bool covering_exists=false;

    //Look through all of the coverings.
    for(int i=0;i<coverings.size();i++){
        if(coverings[i].type==covering){
            coverings.erase(coverings.begin()+i);
            i--;
            covering_exists=true;
        }
    }

    return covering_exists;
}
