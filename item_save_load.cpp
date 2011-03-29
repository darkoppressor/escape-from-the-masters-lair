/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "item.h"

#include <cstdlib>

using namespace std;

string Item::return_save_data(){
    //Setup a string stream to hold the save data.
    stringstream save("");
    save.clear();save.str("");

    save<<identifier<<"\n";

    save<<x<<"\n";
    save<<y<<"\n";

    save<<name<<"\n";

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

    save<<writing<<"\n";

    save<<plural_name<<"\n";

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

    save<<launcher<<"\n";

    save<<armor_category<<"\n";

    save<<defense<<"\n";

    save<<is_corpse<<"\n";

    save<<thirst_quenched<<"\n";

    save<<fuel<<"\n";

    save<<fuel_max<<"\n";

    return save.str();
}

void Item::load_data(stringstream* load){
    string line="";

    getline(*load,line);
    identifier=atoi(line.c_str());

    getline(*load,line);
    x=atoi(line.c_str());

    getline(*load,line);
    y=atoi(line.c_str());

    getline(*load,line);
    name=line;

    getline(*load,line);
    appearance=line;

    getline(*load,line);
    color=atoi(line.c_str());

    getline(*load,line);
    weight=atof(line.c_str());

    getline(*load,line);
    owner=atoi(line.c_str());

    getline(*load,line);
    light_on=(bool)atoi(line.c_str());

    short get_facing;

    getline(*load,line);
    get_facing=atoi(line.c_str());

    facing=(fov_direction_type)get_facing;

    getline(*load,line);
    inventory_letter=atoi(line.c_str());

    getline(*load,line);
    equipped=(bool)atoi(line.c_str());

    getline(*load,line);
    race=atoi(line.c_str());

    getline(*load,line);
    dye=atoi(line.c_str());

    getline(*load,line);
    stack=atoi(line.c_str());

    getline(*load,line);
    stackable=(bool)atoi(line.c_str());

    getline(*load,line);
    spawnable=(bool)atoi(line.c_str());

    getline(*load,line);
    writing=line;

    getline(*load,line);
    plural_name=line;

    getline(*load,line);
    monetary_value=atoi(line.c_str());

    getline(*load,line);
    category=atoi(line.c_str());

    getline(*load,line);
    material=atoi(line.c_str());

    getline(*load,line);
    damage_min_melee=atoi(line.c_str());

    getline(*load,line);
    damage_max_melee=atoi(line.c_str());

    getline(*load,line);
    damage_min_ranged=atoi(line.c_str());

    getline(*load,line);
    damage_max_ranged=atoi(line.c_str());

    getline(*load,line);
    damage_min_thrown=atoi(line.c_str());

    getline(*load,line);
    damage_max_thrown=atoi(line.c_str());

    getline(*load,line);
    fov_radius=atoi(line.c_str());

    getline(*load,line);
    beam=(bool)atoi(line.c_str());

    getline(*load,line);
    fov_angle=atof(line.c_str());

    int effects_size=0;
    effects.clear();

    getline(*load,line);
    effects_size=atoi(line.c_str());

    for(int i=0;i<effects_size;i++){
        short number=0;

        getline(*load,line);
        number=atoi(line.c_str());

        effects.push_back(number);
    }

    getline(*load,line);
    governing_skill_weapon=atoi(line.c_str());

    getline(*load,line);
    weapon_category=atoi(line.c_str());

    getline(*load,line);
    launcher=atoi(line.c_str());

    getline(*load,line);
    armor_category=atoi(line.c_str());

    getline(*load,line);
    defense=atoi(line.c_str());

    getline(*load,line);
    is_corpse=(bool)atoi(line.c_str());

    getline(*load,line);
    thirst_quenched=atoi(line.c_str());

    getline(*load,line);
    fuel=atoi(line.c_str());

    getline(*load,line);
    fuel_max=atoi(line.c_str());
}
