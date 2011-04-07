/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"
#include "world.h"

using namespace std;

int Creature::return_health(){
    double number=health;

    //Apply the hardiness bonus.
    number+=number*((double)return_attribute_hardiness()/100.0);

    //Apply the racial modifier.
    number+=templates.template_races[race].health_max;

    return (int)number;
}

int Creature::return_health_max(){
    double number=health_max;

    //Apply the hardiness bonus.
    number+=number*((double)return_attribute_hardiness()/100.0);

    //Apply the racial modifier.
    number+=templates.template_races[race].health_max;

    return (int)number;
}

int Creature::return_mana(){
    double number=mana;

    //Apply the comprehension bonus.
    number+=number*((double)return_attribute_comprehension()/100.0);

    //Apply the acumen bonus.
    number+=number*(((double)return_attribute_acumen()/2.0)/100.0);

    //Apply the racial modifier.
    number+=templates.template_races[race].mana_max;

    return (int)number;
}

int Creature::return_mana_max(){
    double number=mana_max;

    //Apply the comprehension bonus.
    number+=number*((double)return_attribute_comprehension()/100.0);

    //Apply the acumen bonus.
    number+=number*(((double)return_attribute_acumen()/2.0)/100.0);

    //Apply the racial modifier.
    number+=templates.template_races[race].mana_max;

    return (int)number;
}

int Creature::return_armor(){
    double number=0.0;

    //Look through all of the armor slots.
    for(int i=EQUIP_HEAD;i<EQUIP_FINGER_LEFT+1;i++){
        //If this slot has an item equipped.
        if(equipment[i]!=0){
            //Determine the identifier for the item equipped in this slot.
            int item_identifier=index_of_item_in_slot(i);

            //Determine the base amount of damage absorbed by this item.
            double armor_absorption=inventory[item_identifier].defense;

            //Apply the armor skill.
            armor_absorption+=armor_absorption*(return_skill_armor()/10);

            //Apply the hardiness bonus.
            armor_absorption+=armor_absorption*(return_attribute_hardiness()/12);

            number+=armor_absorption;
        }
    }

    return (int)number;
}

int Creature::return_movement_speed(){
    double speed=movement_speed;

    //Subtract the speed skill bonus.
    speed-=return_skill_speed()/4.0;

    //Subtract the agility bonus.
    speed-=return_attribute_agility()/8.0;

    //Subtract the hardiness bonus.
    speed-=return_attribute_hardiness()/10.0;

    //If the creature is bloated.
    if(thirst<=THIRST_BLOATED){
        //Being bloated causes a penalty to speed.
        speed+=speed*0.25;
    }

    //Apply the racial modifier.
    speed+=templates.template_races[race].movement_speed;

    if(speed<1.0){
        speed=1.0;
    }

    return (int)speed;
}

int Creature::return_next_move(){
    double speed=next_move;

    //Subtract the speed skill bonus.
    speed-=return_skill_speed()/4.0;

    //Subtract the agility bonus.
    speed-=return_attribute_agility()/8.0;

    //Subtract the hardiness bonus.
    speed-=return_attribute_hardiness()/10.0;

    //If the creature is bloated.
    if(thirst<=THIRST_BLOATED){
        //Being bloated causes a penalty to speed.
        speed+=speed*0.25;
    }

    //Apply the racial modifier.
    speed+=templates.template_races[race].movement_speed;

    return (int)speed;
}

double Creature::return_inventory_weight(short item_category){
    double total_weight=0.0;

    for(int i=0;i<inventory.size();i++){
        //If all categories are being checked, or the item's category is the one being checked.
        if(item_category==-1 || item_category==inventory[i].category){
            double item_weight=inventory[i].weight;

            //Apply the Armor skill armor weight contribution bonus.
            //This is only applied when the category being checked is not armor.
            if(item_category!=ITEM_ARMOR && inventory[i].category==ITEM_ARMOR){
                item_weight-=return_skill_armor()*0.5;

                //The armor skill bonus cannot make an item weight less than 1.
                if(item_weight<1.0){
                    item_weight=1.0;
                }
            }

            total_weight+=item_weight*inventory[i].stack;
        }
    }

    return total_weight;
}

double Creature::return_carry_capacity(){
    double number=carry_capacity;

    //Add the strength bonus.
    number+=return_attribute_strength()*3;

    //Add the hardiness bonus.
    number+=return_attribute_hardiness();

    //Apply the weight bonus.
    number+=weight/12.0;

    return number;
}
