/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "grammar.h"
#include "world.h"

using namespace std;

string a_vs_an(Item* item){
    string a_or_an="";

    //If the item is a corpse or skeleton.
    if(item->is_corpse || item->is_skeleton){
        a_or_an=templates.template_races[item->race].prefix_article;
    }
    //If the item is anything else.
    else{
        a_or_an=item->prefix_article;
    }

    return a_or_an;
}

string a_vs_an(Creature* creature){
    string a_or_an="";

    a_or_an=templates.template_races[creature->race].prefix_article;

    return a_or_an;
}

string a_vs_an(short race){
    string a_or_an="";

    a_or_an=templates.template_races[race].prefix_article;

    return a_or_an;
}
