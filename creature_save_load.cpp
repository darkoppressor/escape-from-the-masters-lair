/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"

using namespace std;

string Creature::return_save_data(){
    //Setup a string stream to hold the save data.
    stringstream save("");
    save.clear();save.str("");

    save<<identifier<<"\n";

    save<<x<<"\n";
    save<<y<<"\n";

    string send_name=name;
    while(send_name.rfind(" ")!=string::npos){
        send_name.replace(send_name.rfind(" "),1,"SPACE");
    }
    save<<send_name<<"\n";

    save<<appearance<<"\n";
    save<<color<<"\n";
    save<<weight<<"\n";

    save<<gender<<"\n";

    save<<race<<"\n";

    send_name=race_name;
    while(send_name.rfind(" ")!=string::npos){
        send_name.replace(send_name.rfind(" "),1,"SPACE");
    }
    save<<send_name<<"\n";

    save<<health<<"\n";
    save<<health_max<<"\n";

    save<<mana<<"\n";
    save<<mana_max<<"\n";

    save<<experience_level<<"\n";
    save<<experience<<"\n";
    save<<experience_max<<"\n";

    save<<thirst<<"\n";

    save<<base_damage_melee_min<<"\n";
    save<<base_damage_melee_max<<"\n";

    save<<base_damage_ranged_min<<"\n";
    save<<base_damage_ranged_max<<"\n";

    save<<base_damage_thrown_min<<"\n";
    save<<base_damage_thrown_max<<"\n";

    for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
        save<<attributes[i]<<"\n";
    }

    for(int i=0;i<SKILL_MAGIC_SUMMONING+1;i++){
        save<<skills[i][SKILL_EXPERIENCE_LEVEL]<<"\n";
        save<<skills[i][SKILL_EXPERIENCE]<<"\n";
        save<<skills[i][SKILL_EXPERIENCE_MAX]<<"\n";
    }

    for(int i=0;i<3;i++){
        save<<focused_skills[i]<<"\n";
    }

    for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
        save<<attribute_level_bonuses[i]<<"\n";
    }

    save<<carry_capacity<<"\n";

    save<<movement_speed<<"\n";
    save<<next_move<<"\n";

    //Inventory.
    save<<inventory.size()<<"\n";

    //Items.
    for(int n=0;n<inventory.size();n++){
        //Save the item's data from Item.
        save<<inventory[n].return_save_data();
    }

    for(int i=EQUIP_HEAD;i<EQUIP_HOLD_LEFT+1;i++){
        save<<(short)equipment[i]<<"\n";
    }

    save<<(short)facing<<"\n";

    save<<inventory_letters.size()<<"\n";

    for(int i=0;i<inventory_letters.size();i++){
        save<<(short)inventory_letters[i]<<"\n";
    }

    return save.str();
}

void Creature::load_data(stringstream* load){
    *load>>identifier;

    *load>>x;
    *load>>y;

    string get_name="";
    *load>>get_name;
    while(get_name.rfind("SPACE")!=string::npos){
        get_name.replace(get_name.rfind("SPACE"),5," ");
    }
    name=get_name;

    *load>>appearance;
    *load>>color;
    *load>>weight;

    *load>>gender;

    *load>>race;

    get_name="";
    *load>>get_name;
    while(get_name.rfind("SPACE")!=string::npos){
        get_name.replace(get_name.rfind("SPACE"),5," ");
    }
    race_name=get_name;

    *load>>health;
    *load>>health_max;

    *load>>mana;
    *load>>mana_max;

    *load>>experience_level;
    *load>>experience;
    *load>>experience_max;

    *load>>thirst;

    *load>>base_damage_melee_min;
    *load>>base_damage_melee_max;

    *load>>base_damage_ranged_min;
    *load>>base_damage_ranged_max;

    *load>>base_damage_thrown_min;
    *load>>base_damage_thrown_max;

    for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
        *load>>attributes[i];
    }

    for(int i=0;i<SKILL_MAGIC_SUMMONING+1;i++){
        *load>>skills[i][SKILL_EXPERIENCE_LEVEL];
        *load>>skills[i][SKILL_EXPERIENCE];
        *load>>skills[i][SKILL_EXPERIENCE_MAX];
    }

    for(int i=0;i<3;i++){
        *load>>focused_skills[i];
    }

    for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
        *load>>attribute_level_bonuses[i];
    }

    *load>>carry_capacity;

    *load>>movement_speed;
    *load>>next_move;

    //Inventory.
    int number_of_items=0;
    *load>>number_of_items;

    //Items.
    for(int n=0;n<number_of_items;n++){
        inventory.push_back(Item());

        //Load the item's data from Item.
        inventory[n].load_data(load);
    }

    for(int i=EQUIP_HEAD;i<EQUIP_HOLD_LEFT+1;i++){
        short number=0;
        *load>>number;
        equipment[i]=number;
    }

    short get_facing;
    *load>>get_facing;
    facing=(fov_direction_type)get_facing;

    int inventory_letters_size=0;
    inventory_letters.clear();

    *load>>inventory_letters_size;

    for(int i=0;i<inventory_letters_size;i++){
        short number=0;
        *load>>number;
        inventory_letters.push_back(number);
    }
}
