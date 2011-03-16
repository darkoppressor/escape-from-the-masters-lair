/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "save_load.h"
#include "world.h"
#include "quit.h"

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
    //Setup the save file for the player.

    string save_directory="saves/";

    create_directory(save_directory);

    ofstream save;
    string save_file="";

    save_file=save_directory;
    save_file+=player.name;
    save.open(save_file.c_str());

    if(save!=NULL){

        //*******************//
        // Save player data. //
        //*******************//

        //Level information.
        save<<current_level<<"\n";
        save<<max_level<<"\n";
        save<<last_level<<"\n";

        //Location.
        save<<player.x<<"\n";
        save<<player.y<<"\n";

        //Fov data.
        save<<player.facing<<"\n";
        save<<player.light_on<<"\n";

        //******************//
        // Save level data. //
        //******************//

        //Number of levels.
        save<<vector_levels.size()<<"\n";

        //Loop through all levels, saving their data.
        for(int i=0;i<vector_levels.size();i++){
            //Level dimensions.
            save<<vector_levels[i].level_x<<"\n";
            save<<vector_levels[i].level_y<<"\n";

            //Temperature.
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

        save.close();
        save.clear();
    }
    else{
        fprintf(stderr,"Error saving game.\n");
    }

    quit_game();
}

void load_game(){
    //Setup the load file for the player.

    string save_directory="saves/";

    ifstream load;
    string load_file="";

    load_file=save_directory;
    load_file+=player.name;
    load.open(load_file.c_str(),ifstream::in);

    if(load!=NULL){

        //*******************//
        // Load player data. //
        //*******************//

        //Level information.
        load>>current_level;
        load>>max_level;
        load>>last_level;

        //Location.
        load>>player.x;
        load>>player.y;

        //Fov data.
        short facing=0;
        load>>facing;
        player.facing=(fov_direction_type)facing;
        load>>player.light_on;

        //******************//
        // Load level data. //
        //******************//

        //Number of levels.
        int number_of_levels=0;
        load>>number_of_levels;

        //Loop through all levels, creating them and then loading their data.
        for(int i=0;i<number_of_levels;i++){
            short level_x=0;
            short level_y=0;

            //Level dimensions.
            load>>level_x;
            load>>level_y;

            //Create the level.
            vector_levels.push_back(Level(level_x,level_y));

            //Temperature.
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
