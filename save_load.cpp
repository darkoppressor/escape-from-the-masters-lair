/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "save_load.h"
#include "world.h"
#include "quit.h"

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

using namespace std;

void create_top_level_directories(){
    boost::filesystem::create_directory("data");
    boost::filesystem::create_directory("data/images");
    boost::filesystem::create_directory("data/music");
    boost::filesystem::create_directory("data/sounds");
    boost::filesystem::create_directory("data/text");

    boost::filesystem::create_directory("saves");
    boost::filesystem::create_directory("screenshots");
}

void create_directory(string directory){
    boost::filesystem::create_directory(directory.c_str());
}

void remove_directory(string directory){
    boost::filesystem::remove_all(directory.c_str());
}

void delete_file(string file){
    boost::filesystem::remove(file.c_str());
}

void save_game(){
    fprintf(stdout,"Saving %s's game...\n",player.name.c_str());

    //Setup a string stream to hold the save data.
    stringstream save("");
    save.clear();save.str("");

    //*****************//
    // Save game data. //
    //*****************//

    //Identifiers.
    for(short i=0;i<OBJECT_ITEM+1;i++){
        save<<game.identifiers[i].size()<<"\n";
    }

    for(short i=0;i<OBJECT_ITEM+1;i++){
        for(uint32_t n=0;n<game.identifiers[i].size();n++){
            save<<game.identifiers[i][n]<<"\n";
        }
    }

    //Level information.
    save<<current_level<<"\n";
    save<<max_level<<"\n";
    save<<last_level<<"\n";

    //*******************//
    // Save player data. //
    //*******************//

    save<<player.identifier<<"\n";

    save<<player.x<<"\n";
    save<<player.y<<"\n";

    save<<player.name<<"\n";
    save<<player.appearance<<"\n";
    save<<player.color<<"\n";
    save<<player.weight<<"\n";

    save<<player.gender<<"\n";

    save<<player.race<<"\n";
    save<<player.race_name<<"\n";

    save<<player.health<<"\n";
    save<<player.health_max<<"\n";

    save<<player.mana<<"\n";
    save<<player.mana_max<<"\n";

    save<<player.experience_level<<"\n";
    save<<player.experience<<"\n";
    save<<player.experience_max<<"\n";

    save<<player.thirst<<"\n";

    save<<player.base_damage_melee_min<<"\n";
    save<<player.base_damage_melee_max<<"\n";

    save<<player.base_damage_ranged_min<<"\n";
    save<<player.base_damage_ranged_max<<"\n";

    save<<player.base_damage_thrown_min<<"\n";
    save<<player.base_damage_thrown_max<<"\n";

    for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
        save<<player.attributes[i]<<"\n";
    }

    for(int i=0;i<SKILL_MAGIC_SUMMONING+1;i++){
        save<<player.skills[i][SKILL_EXPERIENCE_LEVEL]<<"\n";
        save<<player.skills[i][SKILL_EXPERIENCE]<<"\n";
        save<<player.skills[i][SKILL_EXPERIENCE_MAX]<<"\n";
    }

    for(int i=0;i<3;i++){
        save<<player.focused_skills[i]<<"\n";
    }

    for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
        save<<player.attribute_level_bonuses[i]<<"\n";
    }

    save<<player.carry_capacity<<"\n";

    save<<player.movement_speed<<"\n";
    save<<player.next_move<<"\n";

    //Inventory.
    /**save<<player.inventory.size()<<"\n";

    for(int i=0;i<;i++){
    }

    for(int i=EQUIP_HEAD;i<EQUIP_HOLD_LEFT+1;i++){
        save<<(short)player.equipment[i]<<"\n";
    }*/

    save<<player.facing<<"\n";

    save<<player.inventory_letters.size()<<"\n";

    for(int i=0;i<player.inventory_letters.size();i++){
        save<<(short)player.inventory_letters[i]<<"\n";
    }

    save<<player.turn<<"\n";

    //******************//
    // Save level data. //
    //******************//

    //Number of levels.
    save<<vector_levels.size()<<"\n";

    //Loop through all levels, saving their data.
    for(int i=0;i<vector_levels.size();i++){
        save<<vector_levels[i].level_x<<"\n";
        save<<vector_levels[i].level_y<<"\n";

        save<<vector_levels[i].temperature<<"\n";

        //Tile array.
        for(int x=0;x<vector_levels[i].level_x;x++){
            for(int y=0;y<vector_levels[i].level_y;y++){
                save<<vector_levels[i].tiles[x][y].x<<"\n";
                save<<vector_levels[i].tiles[x][y].y<<"\n";
                save<<vector_levels[i].tiles[x][y].type<<"\n";
                save<<vector_levels[i].tiles[x][y].material<<"\n";
            }
        }

        //Fog array.
        for(int x=0;x<vector_levels[i].level_x;x++){
            for(int y=0;y<vector_levels[i].level_y;y++){
                save<<vector_levels[i].fog[x][y]<<"\n";
            }
        }
    }

    //Setup the save file.
    string save_directory="saves/";
    create_directory(save_directory);
    string save_file="";
    save_file=save_directory;
    save_file+=boost::algorithm::to_lower_copy(player.name);

    //Open the save file for writing.
    ofstream save_to_file;
    save_to_file.open(save_file.c_str(),ios_base::out|ios_base::binary);

    //If the save file was opened successfully.
    if(save_to_file!=NULL){
        //Prepare the output with bzip2 compression.
        boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
        out.push(boost::iostreams::bzip2_compressor());
        out.push(save_to_file);

        //Copy the save data to the output file.
        boost::iostreams::copy(save,out);

        save_to_file.close();
        save_to_file.clear();
    }
    //If there was a problem opening the save file.
    else{
        fprintf(stderr,"Error saving %s's game.\n",player.name.c_str());
    }

    quit_game();
}

void load_game(){
    //Setup the load file for the player.

    string save_directory="saves/";

    ifstream load;
    string load_file="";

    load_file=save_directory;
    load_file+=boost::algorithm::to_lower_copy(player.name);
    load.open(load_file.c_str(),ifstream::in);

    if(load!=NULL){

        //*****************//
        // Load game data. //
        //*****************//

        int identifiers_sizes[OBJECT_ITEM+1];

        //Identifiers.
        for(short i=0;i<OBJECT_ITEM+1;i++){
            game.identifiers.push_back(vector<uint32_t>());

            load>>identifiers_sizes[i];
        }

        for(short i=0;i<OBJECT_ITEM+1;i++){
            for(uint32_t n=0;n<identifiers_sizes[i];n++){
                int this_identifier=0;

                load>>this_identifier;

                game.identifiers[i].push_back(this_identifier);
            }
        }

        //Level information.
        load>>current_level;
        load>>max_level;
        load>>last_level;

        //*******************//
        // Load player data. //
        //*******************//

        load>>player.identifier;

        load>>player.x;
        load>>player.y;

        load>>player.name;
        load>>player.appearance;
        load>>player.color;
        load>>player.weight;

        load>>player.gender;

        load>>player.race;
        load>>player.race_name;

        load>>player.health;
        load>>player.health_max;

        load>>player.mana;
        load>>player.mana_max;

        load>>player.experience_level;
        load>>player.experience;
        load>>player.experience_max;

        load>>player.thirst;

        load>>player.base_damage_melee_min;
        load>>player.base_damage_melee_max;

        load>>player.base_damage_ranged_min;
        load>>player.base_damage_ranged_max;

        load>>player.base_damage_thrown_min;
        load>>player.base_damage_thrown_max;

        for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
            load>>player.attributes[i];
        }

        for(int i=0;i<SKILL_MAGIC_SUMMONING+1;i++){
            load>>player.skills[i][SKILL_EXPERIENCE_LEVEL];
            load>>player.skills[i][SKILL_EXPERIENCE];
            load>>player.skills[i][SKILL_EXPERIENCE_MAX];
        }

        for(int i=0;i<3;i++){
            load>>player.focused_skills[i];
        }

        for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
            load>>player.attribute_level_bonuses[i];
        }

        load>>player.carry_capacity;

        load>>player.movement_speed;
        load>>player.next_move;

        ///Inventory.

        /**for(int i=EQUIP_HEAD;i<EQUIP_HOLD_LEFT+1;i++){
            load>>player.equipment[i];
        }*/

        short facing;
        load>>facing;
        player.facing=(fov_direction_type)facing;

        int inventory_letters_size=0;
        player.inventory_letters.clear();

        load>>inventory_letters_size;

        for(int i=0;i<inventory_letters_size;i++){
            int number=0;
            load>>number;
            player.inventory_letters.push_back(number);
        }

        load>>player.turn;

        //******************//
        // Load level data. //
        //******************//

        int number_of_levels=0;

        load>>number_of_levels;

        //Loop through all levels, saving their data.
        for(int i=0;i<number_of_levels;i++){
            int level_x=0;
            int level_y=0;

            load>>level_x;
            load>>level_y;

            vector_levels.push_back(Level(level_x,level_y));

            load>>vector_levels[i].temperature;

            //Tile array.
            for(int x=0;x<vector_levels[i].level_x;x++){
                for(int y=0;y<vector_levels[i].level_y;y++){
                    load>>vector_levels[i].tiles[x][y].x;
                    load>>vector_levels[i].tiles[x][y].y;
                    load>>vector_levels[i].tiles[x][y].type;
                    load>>vector_levels[i].tiles[x][y].material;
                }
            }

            //Fog array.
            for(int x=0;x<vector_levels[i].level_x;x++){
                for(int y=0;y<vector_levels[i].level_y;y++){
                    load>>vector_levels[i].fog[x][y];
                }
            }
        }

        load.close();
        load.clear();

        //Delete save file.
        delete_file(load_file);

        fprintf(stdout,"%s's game was loaded successfully.\n",player.name.c_str());
    }
    else{
        fprintf(stderr,"Error loading game.\n");
    }
}

bool load_commands(){
    ifstream load("data/text/commands.txt");

    if(load!=NULL){
        getline(load,string_command_what_direction);
        getline(load,string_command_drop_what);
        getline(load,string_door_not_there);
        getline(load,string_door_wont_move);
        getline(load,string_door_opens);
        getline(load,string_door_already_open);
        getline(load,string_door_closes);
        getline(load,string_door_already_closed);
        getline(load,string_up_stairs_not_here);
        getline(load,string_down_stairs_not_here);
        getline(load,string_no_item_this_tile);
        getline(load,string_inventory_full);
        getline(load,string_command_canceled);
        getline(load,string_discover_secret_door);

        load.close();
        load.clear();
    }
    else{
        if(!save_commands()){
            return false;
        }
    }

    return true;
}

bool save_commands(){
    ofstream save("data/text/commands.txt");

    if(save!=NULL){
        save<<string_command_what_direction<<"\n";
        save<<string_command_drop_what<<"\n";
        save<<string_door_not_there<<"\n";
        save<<string_door_wont_move<<"\n";
        save<<string_door_opens<<"\n";
        save<<string_door_already_open<<"\n";
        save<<string_door_closes<<"\n";
        save<<string_door_already_closed<<"\n";
        save<<string_up_stairs_not_here<<"\n";
        save<<string_down_stairs_not_here<<"\n";
        save<<string_no_item_this_tile<<"\n";
        save<<string_inventory_full<<"\n";
        save<<string_command_canceled<<"\n";
        save<<string_discover_secret_door<<"\n";

        save.close();
        save.clear();
    }
    else{
        return false;
    }

    return true;
}

bool options_load(){
    ifstream load("options.cfg");

    if(load!=NULL){
        load>>player.option_fullscreen;
        load>>player.option_dev;
        load>>player.option_fps;

        load.close();
        load.clear();
    }
    else{
        if(!options_save()){
            return false;
        }
    }

    return true;
}

bool options_save(){
    ofstream save("options.cfg");

    if(save!=NULL){
        save<<player.option_fullscreen<<"\n";
        save<<player.option_dev<<"\n";
        save<<player.option_fps<<"\n";

        save.close();
        save.clear();
    }
    else{
        return false;
    }

    return true;
}
