/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "monster.h"
#include "random_number_generator.h"
#include "world.h"
#include "ai_keys.h"
#include "ai.h"

using namespace std;

coordinates Monster::ai_opposite_direction(short target_x,short target_y){
    coordinates flee_coords;

    if(x<=target_x){
        flee_coords.x=x-1;
    }
    else if(x>target_x){
        flee_coords.x=x+1;
    }
    else if(y<=target_y){
        flee_coords.y=y-1;
    }
    else if(y>target_y){
        flee_coords.y=y+1;
    }

    return flee_coords;
}

coordinates Monster::ai_nearest_hostile(){
    coordinates hostile_creature;
    hostile_creature.x=-1;
    hostile_creature.y=-1;

    ///For now, this just checks for the player.
    ///Eventually, this will need to check for hostile monsters as well.
    hostile_creature.x=player.x;
    hostile_creature.y=player.y;

    return hostile_creature;
}

ai_action Monster::ai_determine_action(){
    ai_action action;
    action.action=AI_ACTION_DO_NOTHING;
    action.coords.x=0;
    action.coords.y=0;

    coordinates hostile_creature=ai_nearest_hostile();

    Race* mr=&templates.template_races[race];

    //-Survival-//

    if(mr->ai_trait_flees && ai_state_low_health() && ai_state_hostile_nearby(hostile_creature.x,hostile_creature.y)){
        action.action=AI_ACTION_MOVE;
        coordinates flee_coords=ai_opposite_direction(hostile_creature.x,hostile_creature.y);
        action.coords.x=flee_coords.x;
        action.coords.y=flee_coords.y;
    }
    else if(mr->ai_trait_uses_items && ai_state_low_health() && !ai_state_hostile_nearby(hostile_creature.x,hostile_creature.y)){
        action.action=AI_ACTION_HEAL;
    }
    else if(ai_state_thirsty()){
        if(mr->ai_trait_uses_items/** && Has thirst quenching item*/){
            action.action=AI_ACTION_DRINK;
        }
        else{
            action.action=AI_ACTION_SEEK_FOUNTAIN;
            ///Set action coords to nearest fountain.
        }
    }
    else if(!ai_state_low_health() && ai_state_hostile_nearby(hostile_creature.x,hostile_creature.y)){
        action.action=AI_ACTION_MOVE;
        action.coords.x=hostile_creature.x;
        action.coords.y=hostile_creature.y;
    }
    else if(mr->ai_trait_wanders){
        action.action=AI_ACTION_MOVE;
        action.coords.x=random_range(0,vector_levels[current_level].level_x-1);
        action.coords.y=random_range(0,vector_levels[current_level].level_y-1);
    }

    //-Movement-//

    if(action.action==AI_ACTION_MOVE && ai_state_overencumbered()){
        action.action=AI_ACTION_DROP_WEIGHT;
    }

    return action;
}

void Monster::handle_ai(){
    if(alive){
        //Reset AI keystates.
        for(short i=0;i<256;i++){
            ai_keystates[i]=false;
        }

        ai_action action=ai_determine_action();

        if(action.action==AI_ACTION_MOVE){
            short target_x=action.coords.x;
            short target_y=action.coords.y;

            if(x!=target_x || y!=target_y){
                if(target_x<x && target_y<y){
                    ai_keystates[AIK_LEFT_UP]=true;
                }
                else if(target_x>x && target_y<y){
                    ai_keystates[AIK_RIGHT_UP]=true;
                }
                else if(target_x>x && target_y>y){
                    ai_keystates[AIK_RIGHT_DOWN]=true;
                }
                else if(target_x<x && target_y>y){
                    ai_keystates[AIK_LEFT_DOWN]=true;
                }
                else if(target_x<x){
                    ai_keystates[AIK_LEFT]=true;
                }
                else if(target_y<y){
                    ai_keystates[AIK_UP]=true;
                }
                else if(target_x>x){
                    ai_keystates[AIK_RIGHT]=true;
                }
                else if(target_y>y){
                    ai_keystates[AIK_DOWN]=true;
                }
            }
        }
        else if(action.action==AI_ACTION_HEAL){
            for(int i=0;i<inventory.size();i++){
                if(inventory[i].possesses_effect(ITEM_EFFECT_HEALING_SLIGHT) || inventory[i].possesses_effect(ITEM_EFFECT_HEALING) || inventory[i].possesses_effect(ITEM_EFFECT_HEALING_CONSIDERABLE)){
                    ai_keystates[AIK_QUAFF_ITEM]=true;
                    inventory_input_state=inventory[i].inventory_letter;
                    break;
                }
            }
        }
        else if(action.action==AI_ACTION_DRINK){
            for(int i=0;i<inventory.size();i++){
                if(inventory[i].thirst_quenched>10){
                    ai_keystates[AIK_QUAFF_ITEM]=true;
                    inventory_input_state=inventory[i].inventory_letter;
                    break;
                }
            }
        }
        else if(action.action==AI_ACTION_SEEK_FOUNTAIN){
            ///If target location is this monster's location, drink from the fountain here.
            ///If not, move towards the location.
        }
        else if(action.action==AI_ACTION_DROP_WEIGHT){
            ///Find the heaviest non-equipped item.
            ///Drop the item.
        }
    }
}
