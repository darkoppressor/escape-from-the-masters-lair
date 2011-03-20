/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "templates.h"
#include "enumerations.h"
#include "render.h"

using namespace std;

Templates::Templates(){
    for(int i=ITEM_WEAPON;i<ITEM_OTHER+1;i++){
        template_items.push_back(vector<Item>());
    }
}

bool Templates::load_templates(){
    load.open("data/templates",ifstream::in);

    if(load!=NULL){
        //As long as we haven't reached the end of the file.
        while(!load.eof()){
            string line="";

            //Grab the next line of the file.
            getline(load,line);

            //Clear initial whitespace from the line.

            //While the line still has text.
            while(line.length()>0){
                //If the character is a space or tab, delete it.
                if(line[0]==' ' || line[0]=='\x09'){
                    line.erase(0,1);
                }
                //If the character is anything else, stop checking the line.
                else{
                    break;
                }
            }

            //If the line is a comment.
            if(line.rfind("//")!=string::npos){
                //Ignore this line.
            }

            //If the line begins an item.
            else if(line.rfind("<item")!=string::npos){
                //Determine the item's category.

                //If the line begins a weapon item.
                if(line.rfind("weapon")!=string::npos){
                    load_template_item(ITEM_WEAPON);
                }
                //If the line begins an armor item.
                else if(line.rfind("armor")!=string::npos){
                    load_template_item(ITEM_ARMOR);
                }
                //If the line begins a food item.
                else if(line.rfind("food")!=string::npos){
                    load_template_item(ITEM_FOOD);
                }
                //If the line begins a drink item.
                else if(line.rfind("drink")!=string::npos){
                    load_template_item(ITEM_DRINK);
                }
                //If the line begins a scroll item.
                else if(line.rfind("scroll")!=string::npos){
                    load_template_item(ITEM_SCROLL);
                }
                //If the line begins a book item.
                else if(line.rfind("book")!=string::npos){
                    load_template_item(ITEM_BOOK);
                }
                //If the line begins a container item.
                else if(line.rfind("container")!=string::npos){
                    load_template_item(ITEM_CONTAINER);
                }
                //If the line begins an other item.
                else if(line.rfind("other")!=string::npos){
                    load_template_item(ITEM_OTHER);
                }
                else{
                    load.close();
                    load.clear();
                    fprintf(stderr,"Error while loading templates: Attempted to load an item with an unknown category.\n");
                    return false;
                }
            }

            //If the line begins a race.
            else if(line.rfind("<race>")!=string::npos){
                load_template_race();
            }

            //If the line begins a base stats block.
            else if(line.rfind("<base stats>")!=string::npos){
                load_base_stats();
            }
        }

        load.close();
        load.clear();
    }
    else{
        load.close();
        load.clear();
        fprintf(stderr,"Error loading templates file.\n");
        return false;
    }

    return true;
}

void Templates::load_base_stats(){
    //Create a temporary creature.

    Creature temp_creature;

    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string health="health:";
        string mana="mana:";
        string base_damage_melee="base damage melee:";
        string base_damage_ranged="base damage ranged:";
        string base_damage_thrown="base damage thrown:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.

        //While the line still has text.
        while(line.length()>0){
            //If the character is a space or tab, delete it.
            if(line[0]==' ' || line[0]=='\x09'){
                line.erase(0,1);
            }
            //If the character is anything else, stop checking the line.
            else{
                break;
            }
        }

        //If the line is a comment.
        if(line.rfind("//")!=string::npos){
            //Ignore this line.
        }

        //Load race data based on the line.

        //Health.
        else if(line.rfind(health.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,health.length());

            temp_creature.health_max=atoi(line.c_str());
        }
        //Mana.
        else if(line.rfind(mana.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,mana.length());

            temp_creature.mana_max=atoi(line.c_str());
        }
        //Base melee damage.
        else if(line.rfind(base_damage_melee.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,base_damage_melee.length());

            string min_damage="";
            string max_damage="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_damage+=line[n];
                    }

                    line.erase(i);

                    min_damage=line;
                    break;
                }
            }

            temp_creature.base_damage_melee_min=atoi(min_damage.c_str());
            temp_creature.base_damage_melee_max=atoi(max_damage.c_str());
        }
        //Base ranged damage.
        else if(line.rfind(base_damage_ranged.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,base_damage_ranged.length());

            string min_damage="";
            string max_damage="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_damage+=line[n];
                    }

                    line.erase(i);

                    min_damage=line;
                    break;
                }
            }

            temp_creature.base_damage_ranged_min=atoi(min_damage.c_str());
            temp_creature.base_damage_ranged_max=atoi(max_damage.c_str());
        }
        //Base thrown damage.
        else if(line.rfind(base_damage_thrown.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,base_damage_thrown.length());

            string min_damage="";
            string max_damage="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_damage+=line[n];
                    }

                    line.erase(i);

                    min_damage=line;
                    break;
                }
            }

            temp_creature.base_damage_thrown_min=atoi(min_damage.c_str());
            temp_creature.base_damage_thrown_max=atoi(max_damage.c_str());
        }

        //If the line ends the base stats.
        else if(line.rfind("</base stats>")!=string::npos){
            ///Ensure the base stats are legitimate.

            //Apply these base stats to the base_stats object.
            base_stats=temp_creature;

            return;
        }
    }
}

void Templates::load_template_race(){
    //Create a temporary race.

    Race temp_race;

    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string name="name:";
        string appearance="appearance:";
        string color="color:";
        string weight="weight:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.

        //While the line still has text.
        while(line.length()>0){
            //If the character is a space or tab, delete it.
            if(line[0]==' ' || line[0]=='\x09'){
                line.erase(0,1);
            }
            //If the character is anything else, stop checking the line.
            else{
                break;
            }
        }

        //If the line is a comment.
        if(line.rfind("//")!=string::npos){
            //Ignore this line.
        }

        //Load race data based on the line.

        //Name.
        else if(line.rfind(name.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,name.length());

            temp_race.name=line;
        }
        //Appearance.
        else if(line.rfind(appearance.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,appearance.length());

            temp_race.appearance=line;
        }
        //Color.
        else if(line.rfind(color.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,color.length());

            temp_race.color=string_to_color(line);
        }
        //Weight.
        else if(line.rfind(weight.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,weight.length());

            temp_race.weight=atoi(line.c_str());
        }

        //If the line ends the race.
        else if(line.rfind("</race>")!=string::npos){
            ///Ensure the race is legitimate.

            //Add this race to its templates vector.
            template_races.push_back(Race());
            template_races[template_races.size()-1]=temp_race;

            return;
        }
    }
}

void Templates::load_template_item(short category){
    //Create a temporary item based on category.

    Item temp_item;

    temp_item.spawnable=true;

    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The item data name strings used in the file.

        string spawnable="<DON'T SPAWN>";
        string name="name:";
        string plural_name="plural name:";
        string writing="writing:";
        string appearance="appearance:";
        string color="color:";
        string stackable="stackable:";
        string weight="weight:";
        string monetary_value="value:";
        string material="material:";
        string damage_melee="melee damage:";
        string damage_thrown="thrown damage:";
        string damage_ranged="ranged damage:";
        string fov_radius="light radius:";
        string beam="light beam:";
        string fov_angle="beam angle:";
        string use="use:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.

        //While the line still has text.
        while(line.length()>0){
            //If the character is a space or tab, delete it.
            if(line[0]==' ' || line[0]=='\x09'){
                line.erase(0,1);
            }
            //If the character is anything else, stop checking the line.
            else{
                break;
            }
        }

        //If the line is a comment.
        if(line.rfind("//")!=string::npos){
            //Ignore this line.
        }

        //Load item data based on the line.

        //Spawnable.
        else if(line.rfind(spawnable.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,spawnable.length());

            temp_item.spawnable=false;
        }
        //Plural name.
        else if(line.rfind(plural_name.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,plural_name.length());

            temp_item.plural_name=line;
        }
        //Writing.
        else if(line.rfind(writing.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,writing.length());

            temp_item.writing=line;
        }
        //Name.
        else if(line.rfind(name.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,name.length());

            temp_item.name=line;
        }
        //Appearance.
        else if(line.rfind(appearance.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,appearance.length());

            temp_item.appearance=line;
        }
        //Color.
        else if(line.rfind(color.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,color.length());

            temp_item.color=string_to_color(line);
        }
        //Stackable.
        else if(line.rfind(stackable.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,stackable.length());

            temp_item.stackable=(bool)atoi(line.c_str());
        }
        //Weight.
        else if(line.rfind(weight.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,weight.length());

            temp_item.weight=atoi(line.c_str());
        }
        //Monetary value.
        else if(line.rfind(monetary_value.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,monetary_value.length());

            temp_item.monetary_value=atoi(line.c_str());
        }
        //Material.
        else if(line.rfind(material.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,material.length());

            temp_item.material=string_to_material(line);
        }
        //Melee damage.
        else if(line.rfind(damage_melee.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,damage_melee.length());

            string min_damage="";
            string max_damage="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_damage+=line[n];
                    }

                    line.erase(i);

                    min_damage=line;
                    break;
                }
            }

            temp_item.damage_min_melee=atoi(min_damage.c_str());
            temp_item.damage_max_melee=atoi(max_damage.c_str());
        }
        //Thrown damage.
        else if(line.rfind(damage_thrown.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,damage_thrown.length());

            string min_damage="";
            string max_damage="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_damage+=line[n];
                    }

                    line.erase(i);

                    min_damage=line;
                    break;
                }
            }

            temp_item.damage_min_thrown=atoi(min_damage.c_str());
            temp_item.damage_max_thrown=atoi(max_damage.c_str());
        }
        //Ranged damage.
        else if(line.rfind(damage_ranged.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,damage_ranged.length());

            string min_damage="";
            string max_damage="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_damage+=line[n];
                    }

                    line.erase(i);

                    min_damage=line;
                    break;
                }
            }

            temp_item.damage_min_ranged=atoi(min_damage.c_str());
            temp_item.damage_max_ranged=atoi(max_damage.c_str());
        }
        //Light radius.
        else if(line.rfind(fov_radius.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,fov_radius.length());

            temp_item.fov_radius=atoi(line.c_str());
        }
        //Light beam.
        else if(line.rfind(beam.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,beam.length());

            temp_item.beam=(bool)atoi(line.c_str());
        }
        //Beam angle.
        else if(line.rfind(fov_angle.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,fov_angle.length());

            temp_item.fov_angle=atoi(line.c_str());
        }
        //Use.
        else if(line.rfind(use.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,use.length());

            temp_item.use=string_to_use(line);
        }

        //If the line begins weapon-specific data.
        else if(line.rfind("<weapon")!=string::npos){
            load_template_item_weapon(&temp_item);
        }

        //If the line begins armor-specific data.
        else if(line.rfind("<armor")!=string::npos){
            load_template_item_armor(&temp_item);
        }

        //If the line begins food-specific data.
        else if(line.rfind("<food")!=string::npos){
            load_template_item_food(&temp_item);
        }

        //If the line begins drink-specific data.
        else if(line.rfind("<drink")!=string::npos){
            load_template_item_drink(&temp_item);
        }

        //If the line begins scroll-specific data.
        else if(line.rfind("<scroll")!=string::npos){
            load_template_item_scroll(&temp_item);
        }

        //If the line begins book-specific data.
        else if(line.rfind("<book")!=string::npos){
            load_template_item_book(&temp_item);
        }

        //If the line begins container-specific data.
        else if(line.rfind("<container")!=string::npos){
            load_template_item_container(&temp_item);
        }

        //If the line begins other-specific data.
        else if(line.rfind("<other")!=string::npos){
            load_template_item_other(&temp_item);
        }

        //If the line ends the item.
        else if(line.rfind("</item>")!=string::npos){
            ///Ensure the item is legitimate.

            //Add this item to its templates vector.
            template_items[category].push_back(Item());
            temp_item.category=category;
            template_items[category][template_items[category].size()-1]=temp_item;

            return;
        }
    }
}

void Templates::load_template_item_weapon(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string governing_skill_weapon="governing weapon skill:";
        string weapon_category="weapon category:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.

        //While the line still has text.
        while(line.length()>0){
            //If the character is a space or tab, delete it.
            if(line[0]==' ' || line[0]=='\x09'){
                line.erase(0,1);
            }
            //If the character is anything else, stop checking the line.
            else{
                break;
            }
        }

        //If the line is a comment.
        if(line.rfind("//")!=string::npos){
            //Ignore this line.
        }

        //Load data based on the line.

        //Governing weapon skill.
        else if(line.rfind(governing_skill_weapon.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,governing_skill_weapon.length());

            temp_item->governing_skill_weapon=string_to_skill(line);
        }
        //Weapon category.
        else if(line.rfind(weapon_category.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,weapon_category.length());

            temp_item->weapon_category=string_to_weapon_category(line);
        }

        //If the line ends the weapon.
        else if(line.rfind("</weapon>")!=string::npos){
            //Done reading weapon data.
            return;
        }
    }
}

void Templates::load_template_item_armor(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string armor_category="armor category:";
        string defense="defense:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.

        //While the line still has text.
        while(line.length()>0){
            //If the character is a space or tab, delete it.
            if(line[0]==' ' || line[0]=='\x09'){
                line.erase(0,1);
            }
            //If the character is anything else, stop checking the line.
            else{
                break;
            }
        }

        //If the line is a comment.
        if(line.rfind("//")!=string::npos){
            //Ignore this line.
        }

        //Load data based on the line.

        //Armor category.
        else if(line.rfind(armor_category.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,armor_category.length());

            temp_item->armor_category=string_to_armor_category(line);
        }
        //Defense.
        else if(line.rfind(defense.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,defense.length());

            temp_item->defense=atoi(line.c_str());
        }

        //If the line ends the armor.
        else if(line.rfind("</armor>")!=string::npos){
            //Done reading armor data.
            return;
        }
    }
}

void Templates::load_template_item_food(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string is_corpse="is corpse:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.

        //While the line still has text.
        while(line.length()>0){
            //If the character is a space or tab, delete it.
            if(line[0]==' ' || line[0]=='\x09'){
                line.erase(0,1);
            }
            //If the character is anything else, stop checking the line.
            else{
                break;
            }
        }

        //If the line is a comment.
        if(line.rfind("//")!=string::npos){
            //Ignore this line.
        }

        //Load data based on the line.

        //Is corpse.
        else if(line.rfind(is_corpse.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,is_corpse.length());

            temp_item->is_corpse=(bool)atoi(line.c_str());
        }

        //If the line ends the food.
        else if(line.rfind("</food>")!=string::npos){
            //Done reading food data.
            return;
        }
    }
}

void Templates::load_template_item_drink(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string thirst_quenched="thirst quenched:";
        string drink_effect="effect:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.

        //While the line still has text.
        while(line.length()>0){
            //If the character is a space or tab, delete it.
            if(line[0]==' ' || line[0]=='\x09'){
                line.erase(0,1);
            }
            //If the character is anything else, stop checking the line.
            else{
                break;
            }
        }

        //If the line is a comment.
        if(line.rfind("//")!=string::npos){
            //Ignore this line.
        }

        //Load data based on the line.

        //Thirst quenched.
        else if(line.rfind(thirst_quenched.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,thirst_quenched.length());

            temp_item->thirst_quenched=atoi(line.c_str());
        }
        //Drink effect.
        else if(line.rfind(drink_effect.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,drink_effect.length());

            temp_item->drink_effect=string_to_drink_effect(line);
        }

        //If the line ends the drink.
        else if(line.rfind("</drink>")!=string::npos){
            //Done reading drink data.
            return;
        }
    }
}

void Templates::load_template_item_scroll(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        ///string is_corpse="is corpse:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.

        //While the line still has text.
        while(line.length()>0){
            //If the character is a space or tab, delete it.
            if(line[0]==' ' || line[0]=='\x09'){
                line.erase(0,1);
            }
            //If the character is anything else, stop checking the line.
            else{
                break;
            }
        }

        //If the line is a comment.
        if(line.rfind("//")!=string::npos){
            //Ignore this line.
        }

        //Load data based on the line.

        //Is corpse.
        /**else if(line.rfind(is_corpse.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,is_corpse.length());

            temp_item->is_corpse=(bool)atoi(line.c_str());
        }*/

        //If the line ends the scroll.
        else if(line.rfind("</scroll>")!=string::npos){
            //Done reading scroll data.
            return;
        }
    }
}

void Templates::load_template_item_book(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        ///string is_corpse="is corpse:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.

        //While the line still has text.
        while(line.length()>0){
            //If the character is a space or tab, delete it.
            if(line[0]==' ' || line[0]=='\x09'){
                line.erase(0,1);
            }
            //If the character is anything else, stop checking the line.
            else{
                break;
            }
        }

        //If the line is a comment.
        if(line.rfind("//")!=string::npos){
            //Ignore this line.
        }

        //Load data based on the line.

        //Is corpse.
        /**else if(line.rfind(is_corpse.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,is_corpse.length());

            temp_item->is_corpse=(bool)atoi(line.c_str());
        }*/

        //If the line ends the book.
        else if(line.rfind("</book>")!=string::npos){
            //Done reading book data.
            return;
        }
    }
}

void Templates::load_template_item_container(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        ///string is_corpse="is corpse:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.

        //While the line still has text.
        while(line.length()>0){
            //If the character is a space or tab, delete it.
            if(line[0]==' ' || line[0]=='\x09'){
                line.erase(0,1);
            }
            //If the character is anything else, stop checking the line.
            else{
                break;
            }
        }

        //If the line is a comment.
        if(line.rfind("//")!=string::npos){
            //Ignore this line.
        }

        //Load data based on the line.

        //Is corpse.
        /**else if(line.rfind(is_corpse.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,is_corpse.length());

            temp_item->is_corpse=(bool)atoi(line.c_str());
        }*/

        //If the line ends the container.
        else if(line.rfind("</container>")!=string::npos){
            //Done reading container data.
            return;
        }
    }
}

void Templates::load_template_item_other(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string fuel_max="max fuel:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.

        //While the line still has text.
        while(line.length()>0){
            //If the character is a space or tab, delete it.
            if(line[0]==' ' || line[0]=='\x09'){
                line.erase(0,1);
            }
            //If the character is anything else, stop checking the line.
            else{
                break;
            }
        }

        //If the line is a comment.
        if(line.rfind("//")!=string::npos){
            //Ignore this line.
        }

        //Load data based on the line.

        //Max fuel amount.
        else if(line.rfind(fuel_max.c_str())!=string::npos){
            //Clear the data name.
            line.erase(0,fuel_max.length());

            temp_item->fuel_max=atoi(line.c_str());
        }

        //If the line ends the other.
        else if(line.rfind("</other>")!=string::npos){
            //Done reading other data.
            return;
        }
    }
}
