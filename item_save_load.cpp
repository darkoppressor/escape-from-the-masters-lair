/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "item.h"

using namespace std;

string Item::return_save_data(){
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

    save<<owner<<"\n";

    save<<light_on<<"\n";

    save<<(short)facing<<"\n";

    save<<(short)inventory_letter<<"\n";

    save<<equipped<<"\n";

    save<<race<<"\n";

    save<<dye<<"\n";

    save<<stack<<"\n";

    save<<stackable<<"\n";

    save<<spawnable<<"\n";

    send_name=writing;
    while(send_name.rfind(" ")!=string::npos){
        send_name.replace(send_name.rfind(" "),1,"SPACE");
    }
    save<<send_name<<"\n";

    send_name=plural_name;
    while(send_name.rfind(" ")!=string::npos){
        send_name.replace(send_name.rfind(" "),1,"SPACE");
    }
    save<<send_name<<"\n";

    save<<monetary_value<<"\n";

    save<<category<<"\n";

    save<<material<<"\n";

    save<<damage_min_melee<<"\n";
    save<<damage_max_melee<<"\n";

    save<<damage_min_ranged<<"\n";
    save<<damage_max_ranged<<"\n";

    save<<damage_min_thrown<<"\n";
    save<<damage_max_thrown<<"\n";

    save<<fov_radius<<"\n";

    save<<beam<<"\n";

    save<<fov_angle<<"\n";

    save<<effects.size()<<"\n";
    for(int i=0;i<effects.size();i++){
        save<<effects[i]<<"\n";
    }

    save<<governing_skill_weapon<<"\n";

    save<<weapon_category<<"\n";

    save<<armor_category<<"\n";

    save<<defense<<"\n";

    save<<is_corpse<<"\n";

    save<<thirst_quenched<<"\n";

    save<<fuel<<"\n";

    save<<fuel_max<<"\n";

    return save.str();
}

void Item::load_data(stringstream* load){
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

    *load>>owner;

    *load>>light_on;

    short get_facing;
    *load>>get_facing;
    facing=(fov_direction_type)get_facing;

    short get_inventory_letter;
    *load>>get_inventory_letter;
    inventory_letter=get_inventory_letter;

    *load>>equipped;

    *load>>race;

    *load>>dye;

    *load>>stack;

    *load>>stackable;

    *load>>spawnable;

    get_name="";
    *load>>get_name;
    while(get_name.rfind("SPACE")!=string::npos){
        get_name.replace(get_name.rfind("SPACE"),5," ");
    }
    writing=get_name;

    get_name="";
    *load>>get_name;
    while(get_name.rfind("SPACE")!=string::npos){
        get_name.replace(get_name.rfind("SPACE"),5," ");
    }
    plural_name=get_name;

    *load>>monetary_value;

    *load>>category;

    *load>>material;

    *load>>damage_min_melee;
    *load>>damage_max_melee;

    *load>>damage_min_ranged;
    *load>>damage_max_ranged;

    *load>>damage_min_thrown;
    *load>>damage_max_thrown;

    *load>>fov_radius;

    *load>>beam;

    *load>>fov_angle;

    int effects_size=0;
    effects.clear();

    *load>>effects_size;

    for(int i=0;i<effects_size;i++){
        short number=0;
        *load>>number;
        effects.push_back(number);
    }

    *load>>governing_skill_weapon;

    *load>>weapon_category;

    *load>>armor_category;

    *load>>defense;

    *load>>is_corpse;

    *load>>thirst_quenched;

    *load>>fuel;

    *load>>fuel_max;
}
