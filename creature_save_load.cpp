/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"

#include <cstdlib>

using namespace std;

string Creature::return_save_data(){
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

    save<<coverings.size()<<"\n";
    for(int n=0;n<coverings.size();n++){
        save<<coverings[n].type<<"\n";
        save<<coverings[n].age<<"\n";
    }

    save<<gender<<"\n";

    save<<race<<"\n";

    save<<race_name<<"\n";

    save<<class_name<<"\n";

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

    int coverings_size=0;

    getline(*load,line);
    coverings_size=atoi(line.c_str());

    for(int n=0;n<coverings_size;n++){
        int number=0;

        getline(*load,line);
        number=atoi(line.c_str());

        add_covering(number);

        getline(*load,line);
        number=atoi(line.c_str());

        coverings[n].age=number;
    }

    getline(*load,line);
    gender=atoi(line.c_str());

    getline(*load,line);
    race=atoi(line.c_str());

    getline(*load,line);
    race_name=line;

    getline(*load,line);
    class_name=line;

    getline(*load,line);
    health=atoi(line.c_str());

    getline(*load,line);
    health_max=atoi(line.c_str());

    getline(*load,line);
    mana=atoi(line.c_str());

    getline(*load,line);
    mana_max=atoi(line.c_str());

    getline(*load,line);
    experience_level=atoi(line.c_str());

    getline(*load,line);
    experience=atoi(line.c_str());

    getline(*load,line);
    experience_max=atoi(line.c_str());

    getline(*load,line);
    thirst=atoi(line.c_str());

    getline(*load,line);
    base_damage_melee_min=atoi(line.c_str());

    getline(*load,line);
    base_damage_melee_max=atoi(line.c_str());

    getline(*load,line);
    base_damage_ranged_min=atoi(line.c_str());

    getline(*load,line);
    base_damage_ranged_max=atoi(line.c_str());

    getline(*load,line);
    base_damage_thrown_min=atoi(line.c_str());

    getline(*load,line);
    base_damage_thrown_max=atoi(line.c_str());

    for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
        getline(*load,line);
        attributes[i]=atoi(line.c_str());
    }

    for(int i=0;i<SKILL_MAGIC_SUMMONING+1;i++){
        getline(*load,line);
        skills[i][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());

        getline(*load,line);
        skills[i][SKILL_EXPERIENCE]=atoi(line.c_str());

        getline(*load,line);
        skills[i][SKILL_EXPERIENCE_MAX]=atoi(line.c_str());
    }

    for(int i=0;i<3;i++){
        getline(*load,line);
        focused_skills[i]=atoi(line.c_str());
    }

    for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
        getline(*load,line);
        attribute_level_bonuses[i]=atoi(line.c_str());
    }

    getline(*load,line);
    carry_capacity=atoi(line.c_str());

    getline(*load,line);
    movement_speed=atoi(line.c_str());

    getline(*load,line);
    next_move=atoi(line.c_str());

    //Inventory.
    int number_of_items=0;
    getline(*load,line);
    number_of_items=atoi(line.c_str());

    //Items.
    for(int n=0;n<number_of_items;n++){
        inventory.push_back(Item());

        //Load the item's data from Item.
        inventory[n].load_data(load);
    }

    for(int i=EQUIP_HEAD;i<EQUIP_HOLD_LEFT+1;i++){
        getline(*load,line);
        equipment[i]=atoi(line.c_str());
    }

    short get_facing;
    getline(*load,line);
    get_facing=atoi(line.c_str());
    facing=(fov_direction_type)get_facing;

    int inventory_letters_size=0;
    inventory_letters.clear();

    getline(*load,line);
    inventory_letters_size=atoi(line.c_str());

    for(int i=0;i<inventory_letters_size;i++){
        short number=0;

        getline(*load,line);
        number=atoi(line.c_str());

        inventory_letters.push_back(number);
    }
}
