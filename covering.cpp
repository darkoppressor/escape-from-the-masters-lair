/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "covering.h"
#include "enumerations.h"
#include "age_limits.h"

using namespace std;

Covering::Covering(short get_type){
    type=get_type;

    age=0;
}

bool Covering::process_turn(){
    if(type==COVERING_BLOOD_LITTLE){
        age++;

        if(age>=AGE_BLOOD_LITTLE_DRY){
            return false;
        }
    }
    else if(type==COVERING_BLOOD_LOTS){
        age++;

        if(age>=AGE_BLOOD_LOTS_DRY){
            type=COVERING_BLOOD_DRIED;
            age=0;
        }
    }
    else if(type==COVERING_ICE){
        age++;

        if(age>=AGE_ICE){
            type=COVERING_WATER;
            age=0;
        }
    }
    else if(type==COVERING_WATER){
        age++;

        if(age>=AGE_WATER){
            return false;
        }
    }
    else if(type==COVERING_FIRE){
        age++;

        if(age>=AGE_FIRE){
            return false;
        }
    }

    return true;
}
