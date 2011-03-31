/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "monster.h"

using namespace std;

bool Monster::ai_state_thirsty(){
    //If the creature is thirsty.
    if(thirst>=THIRST_THIRSTY){
        return true;
    }

    return false;
}

bool Monster::ai_state_encumbered(){
    //If the creature is slightly burdened.
    if(return_inventory_weight()>=return_carry_capacity()+1 && return_inventory_weight()<=return_carry_capacity()*1.5){
        return true;
    }
    //If the creature is somewhat burdened.
    else if(return_inventory_weight()>=return_carry_capacity()*1.5+1 && return_inventory_weight()<=return_carry_capacity()*2.0){
        return true;
    }
    //If the creature is quite burdened.
    else if(return_inventory_weight()>=return_carry_capacity()*2.0+1 && return_inventory_weight()<=return_carry_capacity()*2.5){
        return true;
    }
    //If the creature is heavily burdened.
    else if(return_inventory_weight()>=return_carry_capacity()*2.5+1 && return_inventory_weight()<=return_carry_capacity()*3.0){
        return true;
    }
    //If the creature is overburdened.
    else if(return_inventory_weight()>=return_carry_capacity()*3.0+1){
        return true;
    }

    return false;
}

bool Monster::ai_state_overencumbered(){
    //If the creature is overburdened.
    if(return_inventory_weight()>=return_carry_capacity()*3.0+1){
        return true;
    }

    return false;
}

bool Monster::ai_state_low_health(){
    if(return_health()<return_health_max()*0.25){
        return true;
    }

    return false;
}

bool Monster::ai_state_hostile_nearby(short hostile_x,short hostile_y){
    if(hostile_x!=-1 && hostile_y!=-1){
        if(object_nearby(hostile_x,hostile_y,8)){
            return true;
        }
    }

    return false;
}
