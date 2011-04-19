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

/**int Creature::return_mana(){
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
}*/

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

    //Apply the racial modifier.
    number+=templates.template_races[race].natural_armor;

    if(number<0.0){
        number=0.0;
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
    number+=weight/24.0;

    return number;
}

short Creature::return_encumbrance_state(){
    //If the creature is unburdened.
    if(return_inventory_weight()<=return_carry_capacity()){
        return ENCUMBRANCE_UNBURDENED;
    }
    //If the creature is slightly burdened.
    else if(return_inventory_weight()>=return_carry_capacity()+1 && return_inventory_weight()<=return_carry_capacity()*1.5){
        return ENCUMBRANCE_SLIGHTLY_BURDENED;
    }
    //If the creature is somewhat burdened.
    else if(return_inventory_weight()>=return_carry_capacity()*1.5+1 && return_inventory_weight()<=return_carry_capacity()*2.0){
        return ENCUMBRANCE_SOMEWHAT_BURDENED;
    }
    //If the creature is quite burdened.
    else if(return_inventory_weight()>=return_carry_capacity()*2.0+1 && return_inventory_weight()<=return_carry_capacity()*2.5){
        return ENCUMBRANCE_QUITE_BURDENED;
    }
    //If the creature is heavily burdened.
    else if(return_inventory_weight()>=return_carry_capacity()*2.5+1 && return_inventory_weight()<=return_carry_capacity()*3.0){
        return ENCUMBRANCE_HEAVILY_BURDENED;
    }
    //If the creature is overburdened.
    else if(return_inventory_weight()>=return_carry_capacity()*3.0+1){
        return ENCUMBRANCE_OVERBURDENED;
    }
}

short Creature::return_thirst_state(){
    //If the creature is bloated.
    if(thirst<=-1){
        return THIRST_BLOATED;
    }
    //If the creature is satiated.
    else if(thirst>=0 && thirst<500){
        return THIRST_SATIATED;
    }
    //If the creature is not thirsty.
    else if(thirst>=500 && thirst<1500){
        return THIRST_NOT_THIRSTY;
    }
    //If the creature is thirsty.
    else if(thirst>=1500 && thirst<1800){
        return THIRST_THIRSTY;
    }
    //If the creature is weak.
    else if(thirst>=1800 && thirst<1900){
        return THIRST_WEAK;
    }
    //If the creature is fainting.
    else if(thirst>=1900 && thirst<2000+(return_attribute_hardiness()*15)){
        return THIRST_FAINTING;
    }
    //If the creature is dead.
    else if(thirst>=2000+(return_attribute_hardiness()*15)){
        return THIRST_DEATH;
    }
}

string Creature::return_encumbrance_string(){
    short encumbrance=return_encumbrance_state();

    if(encumbrance==ENCUMBRANCE_UNBURDENED){
        return "unburdened";
    }
    else if(encumbrance==ENCUMBRANCE_SLIGHTLY_BURDENED){
        return "slightly burdened";
    }
    else if(encumbrance==ENCUMBRANCE_SOMEWHAT_BURDENED){
        return "somewhat burdened";
    }
    else if(encumbrance==ENCUMBRANCE_QUITE_BURDENED){
        return "quite burdened";
    }
    else if(encumbrance==ENCUMBRANCE_HEAVILY_BURDENED){
        return "heavily burdened";
    }
    else if(encumbrance==ENCUMBRANCE_OVERBURDENED){
        return "overburdened";
    }
}

string Creature::return_thirst_string(){
    short thirst_state=return_thirst_state();

    //If the creature is bloated.
    if(thirst_state==THIRST_BLOATED){
        return "bloated";
    }
    //If the creature is satiated.
    else if(thirst_state==THIRST_SATIATED){
        return "satiated";
    }
    //If the creature is not thirsty.
    else if(thirst_state==THIRST_NOT_THIRSTY){
        return "not thirsty";
    }
    //If the creature is thirsty.
    else if(thirst_state==THIRST_THIRSTY){
        return "thirsty";
    }
    //If the creature is weak.
    else if(thirst_state==THIRST_WEAK){
        return "weak from thirst";
    }
    //If the creature is fainting.
    else if(thirst_state==THIRST_FAINTING){
        return "fainting from lack of water";
    }
    //If the creature is dead.
    else if(thirst_state==THIRST_DEATH){
        return "dying of thirst";
    }
}
