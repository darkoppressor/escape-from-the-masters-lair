/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "game.h"
#include "world.h"
#include "render.h"
#include "random_number_generator.h"
#include "random_chance.h"
#include "max_objects.h"
#include "dungeon.h"
#include "message_log.h"
#include "grammar.h"

#include <fstream>

using namespace std;
using namespace boost;

Game::Game(){
    turn=0;

    generated_level_x=0;
    generated_level_y=0;

    for(int i=0;i<LEVEL_VARIATION_LAVA+1;i++){
        level_variations[i]=false;
    }

    generated_temperature=TEMP_ROOM_TEMPERATURE;

    level_theme=LEVEL_THEME_RECTANGLES_AND_CIRCLES;

    for(int i=0;i<GUARANTEED_RUNES;i++){
        guaranteed_rune_levels[i]=-1;
    }
}

Game::~Game(){
    //Free the music.
    Mix_FreeMusic(music);
    music=NULL;
}

void Game::new_game(){
    fprintf(stdout,"Starting new game for %s...\n",player.name.c_str());

    time_t now;
    struct tm *tm_now;
    char buff[BUFSIZ];
    now=time(NULL);
    tm_now=localtime(&now);

    strftime(buff,sizeof buff,"%d",tm_now);

    string day=buff;
    string initial_day_digit="";
    initial_day_digit+=day[0];

    //Remove the leading zero, if any.
    if(atoi(initial_day_digit.c_str())==0){
        erase_head(day,1);
    }

    string number_ending="";

    if(atoi(day.c_str())==11 || atoi(day.c_str())==12 || atoi(day.c_str())==13){
        number_ending="th";
    }
    else if(atoi(day.c_str())%10==1){
        number_ending="st";
    }
    else if(atoi(day.c_str())%10==2){
        number_ending="nd";
    }
    else if(atoi(day.c_str())%10==3){
        number_ending="rd";
    }
    else{
        number_ending="th";
    }

    player.start_date="the ";
    player.start_date+=day;
    player.start_date+=number_ending;
    strftime(buff,sizeof buff," day of %B, %Y",tm_now);
    player.start_date+=buff;

    strftime(buff,sizeof buff,"%H:%M:%S",tm_now);

    player.start_time=buff;

    player.game_in_progress=true;
    player.current_window=WINDOW_NONE;

    prepare_identifiers();

    player.assign_identifier();

    player.apply_race(player.race);

    player.set_base_stats();

    player.set_inventory();

    //Determine the guaranteed rune levels.
    for(int i=0;i<GUARANTEED_RUNES;i++){
        while(guaranteed_rune_levels[i]==-1){
            int random_level=random_range(0,DUNGEON_DEPTH-1);

            bool level_claimed=false;

            for(int n=0;n<GUARANTEED_RUNES;n++){
                if(guaranteed_rune_levels[n]==random_level){
                    level_claimed=true;
                    break;
                }
            }

            if(!level_claimed){
                guaranteed_rune_levels[i]=random_level;
            }
        }
    }

    //Generate all but the deepest level.
    for(int i=0;i<DUNGEON_DEPTH-1;i++){
        generate_level();
    }

    //Generate the deepest level.
    generate_level(true);

    current_level=DUNGEON_DEPTH-1;
    max_level=current_level;
    change_level(NONE);

    //Update initial fov.
    player.update_fov();

    string welcome="Welcome, ";
    welcome+=player.name;
    welcome+="! You are ";
    welcome+=a_vs_an(&player);
    welcome+=" ";
    welcome+=player.race_name;
    welcome+=" ";
    welcome+=player.class_name;
    welcome+=".";

    update_text_log(welcome.c_str(),true);
}

void Game::old_game(){
    fprintf(stdout,"Loading game for %s...\n",player.name.c_str());

    player.game_in_progress=true;
    player.current_window=WINDOW_NONE;

    load_game();

    //Update initial fov.
    player.update_fov();
}

void Game::prepare_identifiers(){
    for(short i=0;i<OBJECT_ITEM+1;i++){
        identifiers.push_back(vector<uint32_t>());
    }

    for(uint32_t i=1;i<MAX_OBJECTS+1;i++){
        identifiers[OBJECT_CREATURE].push_back(i);
        identifiers[OBJECT_ITEM].push_back(i);
    }
}

uint32_t Game::assign_identifier(short object_type){
    //If the identifiers list for this object type is not empty.
    if(identifiers[object_type].size()>0){
        uint32_t new_identifier=identifiers[object_type][identifiers[object_type].size()-1];

        identifiers[object_type].erase(identifiers[object_type].end()-1);

        return new_identifier;
    }
    //If the identifiers list for this object type is empty.
    else{
        fprintf(stderr,"identifiers[%i] depleted!\n",object_type);

        return 0;
    }
}

void Game::return_identifier(short object_type,uint32_t returning_identifier){
    identifiers[object_type].push_back(returning_identifier);
}

void Game::change_level(short direction){
    //If the player has traveled up to the surface.
    if(direction==UP && current_level-1<0){
        update_text_log("You make your way out of the dungeon...",true);

        player.current_window=WINDOW_LEAVE_DUNGEON;

        player.save_game_log_entry(CAUSE_OF_DEATH_NONE);

        return;
    }

    //If any music is playing, fade it out and then stop it.
    while(!Mix_FadeOutMusic(1000) && Mix_PlayingMusic()){
        SDL_Delay(100);
    }
    Mix_HaltMusic();

    //Free the music.
    Mix_FreeMusic(music);
    music=NULL;

    //If the level is changing to a lower level.
    if(direction==DOWN){
        current_level++;

        update_text_log("You make your way down the stairs.",true);
    }
    //If the level is changing to a higher level.
    else if(direction==UP){
        current_level--;

        update_text_log("You make your way up the stairs.",true);
    }

    if(max_level>current_level){
        max_level=current_level;
    }

    //Load the background music. The music that is loaded will later be determined by which level is being loaded.
    //music=Mix_LoadMUS("data/music/1.ogg");

    //If the player has traveled upwards.
    if(direction==UP){
        //Locate the starting position for the player.
        for(short y=0;y<vector_levels[current_level].level_y;y++){
            for(short x=0;x<vector_levels[current_level].level_x;x++){
                if(vector_levels[current_level].tiles[x][y].type==TILE_TYPE_DOWN_STAIRS){
                    //Set the player's starting position.
                    player.x=vector_levels[current_level].tiles[x][y].x;
                    player.y=vector_levels[current_level].tiles[x][y].y;
                }
            }
        }
    }
    //If the player has traveled downwards.
    else if(direction==DOWN){
        //Locate the starting position for the player.
        for(short y=0;y<vector_levels[current_level].level_y;y++){
            for(short x=0;x<vector_levels[current_level].level_x;x++){
                if(vector_levels[current_level].tiles[x][y].type==TILE_TYPE_UP_STAIRS){
                    //Set the player's starting position.
                    player.x=vector_levels[current_level].tiles[x][y].x;
                    player.y=vector_levels[current_level].tiles[x][y].y;
                }
            }
        }
    }
    //If this is the first level.
    else if(direction==NONE){
        while(true){
            short x=random_range(0,vector_levels[current_level].level_x-1);
            short y=random_range(0,vector_levels[current_level].level_y-1);

            if(vector_levels[current_level].tiles[x][y].type==TILE_TYPE_FLOOR){
                //Set the player's starting position.
                player.x=vector_levels[current_level].tiles[x][y].x;
                player.y=vector_levels[current_level].tiles[x][y].y;

                break;
            }
        }
    }

    //Begin playing the background music for this level.
    //Mix_FadeInMusic(music,-1,500);
}
