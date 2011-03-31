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

    //Save the player's data from Creature.
    save<<player.return_save_data();

    //Save the player's data from Player.
    save<<player.turn<<"\n";
    save<<player.start_date<<"\n";
    save<<player.start_time<<"\n";

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
                save<<vector_levels[i].tiles[x][y].covering<<"\n";
            }
        }

        //Fog array.
        for(int x=0;x<vector_levels[i].level_x;x++){
            for(int y=0;y<vector_levels[i].level_y;y++){
                save<<vector_levels[i].fog[x][y]<<"\n";
            }
        }

        save<<vector_levels[i].monsters.size()<<"\n";

        //Monsters.
        for(int n=0;n<vector_levels[i].monsters.size();n++){
            //Save the monster's data from Creature.
            save<<vector_levels[i].monsters[n].return_save_data();

            //Save the monster's data from Monster.
            ///Nothing right now.
            ///save<<vector_levels[i].monsters[n].<<"\n";
        }

        save<<vector_levels[i].items.size()<<"\n";

        //Items.
        for(int n=0;n<vector_levels[i].items.size();n++){
            //Save the item's data from Item.
            save<<vector_levels[i].items[n].return_save_data();
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
    //Setup the load file.
    string save_directory="saves/";
    string load_file="";
    load_file=save_directory;
    load_file+=boost::algorithm::to_lower_copy(player.name);

    //Open the load file for reading.
    ifstream load_from_file;
    load_from_file.open(load_file.c_str(),ios_base::in|ios_base::binary);

    if(load_from_file!=NULL){

        //Prepare the input with bzip2 decompression.
        boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
        in.push(boost::iostreams::bzip2_decompressor());
        in.push(load_from_file);

        //Setup a string stream to hold the load data.
        stringstream load("");
        load.clear();load.str("");

        //Copy the load data from the input file.
        boost::iostreams::copy(in,load);

        string line="";

        //*****************//
        // Load game data. //
        //*****************//

        int identifiers_sizes[OBJECT_ITEM+1];

        //Identifiers.
        for(short i=0;i<OBJECT_ITEM+1;i++){
            game.identifiers.push_back(vector<uint32_t>());

            getline(load,line);
            identifiers_sizes[i]=atoi(line.c_str());
        }

        for(short i=0;i<OBJECT_ITEM+1;i++){
            for(uint32_t n=0;n<identifiers_sizes[i];n++){
                int this_identifier=0;

                getline(load,line);
                this_identifier=atoi(line.c_str());

                game.identifiers[i].push_back(this_identifier);
            }
        }

        //Level information.
        getline(load,line);
        current_level=atoi(line.c_str());

        getline(load,line);
        max_level=atoi(line.c_str());

        getline(load,line);
        last_level=atoi(line.c_str());

        //*******************//
        // Load player data. //
        //*******************//

        //Load the player's data from Creature.
        player.load_data(&load);

        //Load the player's data from Player.
        getline(load,line);
        player.turn=atoi(line.c_str());

        getline(load,line);
        player.start_date=line;

        getline(load,line);
        player.start_time=line;

        //******************//
        // Load level data. //
        //******************//

        int number_of_levels=0;

        getline(load,line);
        number_of_levels=atoi(line.c_str());

        //Loop through all levels, saving their data.
        for(int i=0;i<number_of_levels;i++){
            int level_x=0;
            int level_y=0;

            getline(load,line);
            level_x=atoi(line.c_str());

            getline(load,line);
            level_y=atoi(line.c_str());

            vector_levels.push_back(Level(level_x,level_y));

            getline(load,line);
            vector_levels[i].temperature=atoi(line.c_str());

            //Tile array.
            for(int x=0;x<vector_levels[i].level_x;x++){
                for(int y=0;y<vector_levels[i].level_y;y++){
                    getline(load,line);
                    vector_levels[i].tiles[x][y].x=atoi(line.c_str());

                    getline(load,line);
                    vector_levels[i].tiles[x][y].y=atoi(line.c_str());

                    getline(load,line);
                    vector_levels[i].tiles[x][y].type=atoi(line.c_str());

                    getline(load,line);
                    vector_levels[i].tiles[x][y].material=atoi(line.c_str());

                    getline(load,line);
                    vector_levels[i].tiles[x][y].covering=atoi(line.c_str());
                }
            }

            //Fog array.
            for(int x=0;x<vector_levels[i].level_x;x++){
                for(int y=0;y<vector_levels[i].level_y;y++){
                    getline(load,line);
                    vector_levels[i].fog[x][y]=atoi(line.c_str());
                }
            }

            int number_of_monsters=0;
            getline(load,line);
            number_of_monsters=atoi(line.c_str());

            //Monsters.
            for(int n=0;n<number_of_monsters;n++){
                vector_levels[i].monsters.push_back(Monster());

                //Load the monster's data from Creature.
                vector_levels[i].monsters[n].load_data(&load);

                //Load the monster's data from Monster.
                ///Nothing right now.
                ///load>>vector_levels[i].monsters[n].;
            }

            int number_of_items=0;
            getline(load,line);
            number_of_items=atoi(line.c_str());

            //Items.
            for(int n=0;n<number_of_items;n++){
                vector_levels[i].items.push_back(Item());

                //Load the item's data from Item.
                vector_levels[i].items[n].load_data(&load);
            }
        }

        load_from_file.close();
        load_from_file.clear();

        //Delete save file.
        delete_file(load_file);

        fprintf(stdout,"%s's game was loaded successfully.\n",player.name.c_str());
    }
    else{
        fprintf(stderr,"Error loading %s's game.\n",player.name.c_str());
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
        load>>player.option_healthbars;

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
        save<<player.option_healthbars<<"\n";

        save.close();
        save.clear();
    }
    else{
        return false;
    }

    return true;
}
