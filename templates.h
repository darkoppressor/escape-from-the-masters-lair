/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef templates_h
#define templates_h

#include "item.h"
#include "save_load.h"
#include "race.h"
#include "creature.h"

#include <vector>

class Templates{
    private:
    public:

    std::ifstream load;

    std::vector< std::vector<Item> > template_items;

    std::vector<Race> template_races;

    Creature base_stats;

    Templates();

    bool load_templates();

    void load_base_stats();

    void load_template_race();

    void load_template_item(short category);

    void load_template_item_weapon(Item* temp_item);

    void load_template_item_armor(Item* temp_item);

    void load_template_item_food(Item* temp_item);

    void load_template_item_drink(Item* temp_item);

    void load_template_item_scroll(Item* temp_item);

    void load_template_item_book(Item* temp_item);

    void load_template_item_container(Item* temp_item);

    void load_template_item_other(Item* temp_item);

    //Pass a string representing a color.
    //Returns the short for that color.
    short string_to_color(std::string line);

    //Pass a string representing a weapon category.
    //Returns the short for that weapon category.
    short string_to_weapon_category(std::string line);

    //Pass a string representing an armor category.
    //Returns the short for that armor category.
    short string_to_armor_category(std::string line);

    //Pass a string representing a skill.
    //Returns the short for that skill.
    short string_to_skill(std::string line);

    //Pass a string representing a material.
    //Returns the short for that material.
    short string_to_material(std::string line);

    //Pass a string representing an item use.
    //Returns the short for that item use.
    short string_to_use(std::string line);

    //Pass a string representing a drink effect.
    //Returns the short for that drink effect.
    short string_to_drink_effect(std::string line);
};

#endif
