/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "string_input.h"
#include "world.h"
#include "quit.h"
#include "save_load.h"
#include "version.h"
#include "message_log.h"
#include "pixels.h"
#include "savepng.h"

using namespace std;
using namespace boost::algorithm;

string_input::string_input(){
    str1="";

    for(int i=0;i<100;i++){
        recalled_strings.push_back("\x1F");
    }

    current_recalled_string=recalled_strings.size();
}

void string_input::handle_events(){
    if(event.type==SDL_KEYDOWN){
        //If the string is less than the maximum size.
        if(str1.length()<110){
            if(event.key.keysym.unicode>=(Uint16)' ' && event.key.keysym.unicode<=(Uint16)'~'){
                str1+=(char)event.key.keysym.unicode;
            }
        }

        //If the backspace key is pressed and the string is not empty.
        if(event.key.keysym.sym==SDLK_BACKSPACE && str1.length()>0){
            //Remove one character from the end of the string.
            str1.erase(str1.length()-1);
        }

        else if(event.key.keysym.sym==SDLK_UP){
            if(current_recalled_string>0 && recalled_strings[current_recalled_string-1]!="\x1F"){
                current_recalled_string--;
            }

            str1=recalled_strings[current_recalled_string];
        }
        else if(event.key.keysym.sym==SDLK_DOWN){
            if(current_recalled_string<recalled_strings.size()-1){
                current_recalled_string++;

                str1=recalled_strings[current_recalled_string];
            }
            else{
                current_recalled_string=recalled_strings.size();

                str1.clear();
            }
        }

        //If the player hits enter.
        else if((event.key.keysym.sym==SDLK_RETURN || event.key.keysym.sym==SDLK_KP_ENTER) && str1.length()>0){
            string str_command=str1;

            //Forget the oldest command string.
            recalled_strings.erase(recalled_strings.begin());

            //Remember this string.
            recalled_strings.push_back(str_command);

            trim(str_command);

            if(istarts_with(str_command,"dev")){
                player.option_dev=!player.option_dev;
                options_save();

                if(player.option_dev){
                    update_text_log("Dev mode on.",true,MESSAGE_SYSTEM);
                }
                else{
                    update_text_log("Dev mode off.",true,MESSAGE_SYSTEM);
                }
            }

            else if(istarts_with(str_command,"export")){
                for(int i=0;i<vector_levels.size();i++){
                    //Determine the date and time.
                    /**time_t now;
                    struct tm *tm_now;
                    char buff[BUFSIZ];
                    now=time(NULL);
                    tm_now=localtime(&now);

                    //Store the date and time in buff.
                    strftime(buff,sizeof buff,"%Y-%m-%d_%H.%M.%S",tm_now);*/

                    //Used to store the filename of the picture.
                    string picture_name;

                    //Set the filename.
                    picture_name="dungeon_map_";
                    picture_name+=string_stuff.num_to_string(i);
                    picture_name+=".png";

                    SDL_Surface* surface_temp=NULL;

                    int map_width=vector_levels[i].level_x;
                    int map_height=vector_levels[i].level_y;

                    uint32_t rmask,gmask,bmask,amask;

                    if(SDL_BYTEORDER==SDL_BIG_ENDIAN){
                        rmask = 0xff000000;
                        gmask = 0x00ff0000;
                        bmask = 0x0000ff00;
                        amask = 0x000000ff;
                    }
                    else{
                        rmask = 0x000000ff;
                        gmask = 0x0000ff00;
                        bmask = 0x00ff0000;
                        amask = 0xff000000;
                    }

                    //Setup a surface to hold the tiles as individual pixels.
                    surface_temp=SDL_CreateRGBSurface(SDL_SWSURFACE,map_width,map_height,32,rmask,gmask,bmask,amask);

                    //If the surface must be locked.
                    if(SDL_MUSTLOCK(surface_temp)){
                        //Lock the surface.
                        SDL_LockSurface(surface_temp);
                    }

                    //Put one pixel in the temporary surface to represent each tile.
                    for(int x=0;x<map_width;x++){
                        for(int y=0;y<map_height;y++){
                            color_data color=tile_to_color(&vector_levels[i].tiles[x][y]);
                            uint32_t pixel=SDL_MapRGBA(surface_temp->format,color.red*255,color.green*255,color.blue*255,255);
                            surface_put_pixel(surface_temp,x,y,pixel);
                        }
                    }

                    //If the surface had to be locked.
                    if(SDL_MUSTLOCK(surface_temp)){
                        //Unlock the surface.
                        SDL_UnlockSurface(surface_temp);
                    }

                    IMG_SavePNG(picture_name.c_str(),surface_temp,-1);

                    SDL_FreeSurface(surface_temp);
                }
            }

            else if(player.option_dev && istarts_with(str_command,"add")){
                ierase_first(str_command,"add");
                trim(str_command);

                string message="";

                string item_name="";
                int stack_size=1;
                short item_category=-1;
                int item_index=-1;

                for(int i=0;i<str_command.length();i++){
                    if(isdigit((int)str_command[i]) || i==str_command.length()-1){
                        if(isdigit((int)str_command[i])){
                            for(int n=0;n<i;n++){
                                item_name+=str_command[n];
                            }
                        }
                        else if(i==str_command.length()-1){
                            for(int n=0;n<=i;n++){
                                item_name+=str_command[n];
                            }
                        }

                        trim(item_name);

                        ierase_first(str_command,item_name);

                        trim(str_command);

                        break;
                    }
                }

                if(str_command.size()>0){
                    stack_size=atoi(str_command.c_str());
                }

                //Attempt to add the item to the player's inventory.
                if(player.give_item(item_name,stack_size)){
                    message="Added the ";
                    message+=item_name;
                    message+=" x";
                    ss.clear();ss.str("");ss<<stack_size;message+=ss.str();
                    message+=" to your inventory.";
                }
                //If the item could not be added, the player's inventory must be full.
                else{
                    message="Failed to add ";
                    message+=item_name;
                    message+=" x";
                    ss.clear();ss.str("");ss<<stack_size;message+=ss.str();
                    message+=" to your inventory.";
                }

                update_text_log(message.c_str(),true,MESSAGE_SYSTEM);
            }

            else if(player.option_dev && istarts_with(str_command,"set")){
                ierase_first(str_command,"set");
                trim(str_command);

                if(istarts_with(str_command,"att")){
                    ierase_first(str_command,"att");
                    trim(str_command);

                    string message="";

                    short stat=-1;
                    int stat_number=1;
                    string stat_being_set="";

                    if(istarts_with(str_command,"strength")){
                        stat_being_set="strength";
                        ierase_first(str_command,stat_being_set);
                        stat=ATTRIBUTE_STRENGTH;
                    }
                    else if(istarts_with(str_command,"agility")){
                        stat_being_set="agility";
                        ierase_first(str_command,stat_being_set);
                        stat=ATTRIBUTE_AGILITY;
                    }
                    else if(istarts_with(str_command,"hardiness")){
                        stat_being_set="hardiness";
                        ierase_first(str_command,stat_being_set);
                        stat=ATTRIBUTE_HARDINESS;
                    }
                    else if(istarts_with(str_command,"comprehension")){
                        stat_being_set="comprehension";
                        ierase_first(str_command,stat_being_set);
                        stat=ATTRIBUTE_COMPREHENSION;
                    }
                    else if(istarts_with(str_command,"acumen")){
                        stat_being_set="acumen";
                        ierase_first(str_command,stat_being_set);
                        stat=ATTRIBUTE_ACUMEN;
                    }
                    else if(istarts_with(str_command,"luck")){
                        stat_being_set="luck";
                        ierase_first(str_command,stat_being_set);
                        stat=ATTRIBUTE_LUCK;
                    }

                    //If the stat is valid.
                    if(stat!=-1){
                        trim(str_command);
                        stat_number=atoi(str_command.c_str());

                        string temp="";
                        ss.clear();ss.str("");ss<<stat_number;temp=ss.str();

                        message="Setting ";
                        message+=stat_being_set;
                        message+=" to ";
                        message+=temp;
                        message+=".";

                        update_text_log(message.c_str(),true,MESSAGE_SYSTEM);

                        player.attributes[stat]=stat_number;
                    }
                    //If the stat is invalid.
                    else{
                        update_text_log("That is an invalid stat.",true,MESSAGE_SYSTEM);
                    }
                }
                else{
                    update_text_log("That isn't something that can be set.",true,MESSAGE_SYSTEM);
                }
            }

            else if(istarts_with(str_command,"hb")){
                player.option_healthbars=!player.option_healthbars;
                options_save();
                if(player.option_healthbars){
                    update_text_log("Health bar display on.",true,MESSAGE_SYSTEM);
                }
                else{
                    update_text_log("Health bar display off.",true,MESSAGE_SYSTEM);
                }
            }

            else if(istarts_with(str_command,"hl")){
                player.option_highlight_self=!player.option_highlight_self;
                options_save();
                if(player.option_highlight_self){
                    update_text_log("Highlight self on.",true,MESSAGE_SYSTEM);
                }
                else{
                    update_text_log("Highlight self off.",true,MESSAGE_SYSTEM);
                }
            }

            else if(istarts_with(str_command,"fps")){
                player.option_fps=!player.option_fps;
                options_save();
                if(player.option_fps){
                    update_text_log("FPS will now be displayed.",true,MESSAGE_SYSTEM);
                }
                else{
                    update_text_log("FPS will no longer be displayed.",true,MESSAGE_SYSTEM);
                }
            }

            else if(istarts_with(str_command,"fs")){
                main_window.toggle_fullscreen();
                if(player.option_fullscreen){
                    update_text_log("Switched to fullscreen mode.",true,MESSAGE_SYSTEM);
                }
                else{
                    update_text_log("Switched to windowed mode.",true,MESSAGE_SYSTEM);
                }
            }

            else if(istarts_with(str_command,"save")){
                save_game();
            }

            /**else if(istarts_with(str_command,"help") || istarts_with(str_command,"?")){
                update_text_log("For a list of extended commands, type 'commands'.",true,MESSAGE_SYSTEM);
            }

            else if(istarts_with(str_command,"commands")){
                update_text_log("help",true,MESSAGE_SYSTEM);
            }*/

            else if(istarts_with(str_command,"about")){
                string version="";
                ss.clear();ss.str("");ss<<"                              Version: ";ss<<AutoVersion::MAJOR;ss<<".";ss<<AutoVersion::MINOR;ss<<" ";ss<<AutoVersion::STATUS;version=ss.str();

                string date="";
                ss.clear();ss.str("");ss<<"                             Built on: ";ss<<AutoVersion::MONTH;ss<<"-";ss<<AutoVersion::DATE;ss<<"-";ss<<AutoVersion::YEAR;date=ss.str();

                update_text_log("--------------------------------------------------------------------------------",true,MESSAGE_SYSTEM);
                update_text_log(date.c_str(),true,MESSAGE_SYSTEM);
                update_text_log(version.c_str(),true,MESSAGE_SYSTEM);
                update_text_log("                            Created by: Kevin Wells",true,MESSAGE_SYSTEM);
                update_text_log("                         Escape from the Master's Lair",true,MESSAGE_SYSTEM);
                update_text_log("--------------------------------------------------------------------------------",true,MESSAGE_SYSTEM);
            }

            else if(istarts_with(str_command,"quit")){
                save_game();
            }

            else if(istarts_with(str_command,"4 8 15 16 23 42")){
                ///
            }

            else{
                update_text_log("Unrecognized command.",true,MESSAGE_SYSTEM);
            }

            str1.clear();

            player.chat_mode=false;

            //Make sure that enter is cleared before moving to the next screen.
            Uint8 *keystates=SDL_GetKeyState(NULL);
            keystates[SDLK_RETURN]=NULL;
            keystates[SDLK_KP_ENTER]=NULL;
        }

        else if((event.key.keysym.sym==SDLK_RETURN || event.key.keysym.sym==SDLK_KP_ENTER) && str1.length()==0){
            player.chat_mode=false;

            //Make sure that enter is cleared before moving to the next screen.
            Uint8 *keystates=SDL_GetKeyState(NULL);
            keystates[SDLK_RETURN]=NULL;
            keystates[SDLK_KP_ENTER]=NULL;
        }

        else if(event.key.keysym.sym==SDLK_ESCAPE){
            str1.clear();

            player.chat_mode=false;

            //Make sure that escape is cleared before moving to the next screen.
            Uint8 *keystates=SDL_GetKeyState(NULL);
            keystates[SDLK_ESCAPE]=NULL;
        }
    }
}

color_data string_input::tile_to_color(Tile* tile){
    color_data color;
    color=color_shorts_to_doubles(0,0,0);

    /**
    TILE_TYPE_UP_STAIRS,
    ,
    ,
    ,
    ,
    TILE_TYPE_TRAP*/

    if(tile->type==TILE_TYPE_WALL){
        color=color_shorts_to_doubles(255,255,255);
    }
    else if(tile->type==TILE_TYPE_FLOOR){
        color=color_shorts_to_doubles(0,0,0);
    }
    else if(tile->type==TILE_TYPE_SOLID){
        color=color_shorts_to_doubles(89,89,89);
    }
    else if(tile->type==TILE_TYPE_LIQUID || tile->type==TILE_TYPE_FOUNTAIN){
        if(tile->material==MATERIAL_WATER){
            if(tile->has_covering(COVERING_ICE)){
                color=color_shorts_to_doubles(128,0,128);
            }
            else{
                color=color_shorts_to_doubles(0,0,255);
            }
        }
        else if(tile->material==MATERIAL_LAVA){
            color=color_shorts_to_doubles(255,0,0);
        }
    }
    else if(tile->type==TILE_TYPE_DOWN_STAIRS){
        color=color_shorts_to_doubles(0,255,0);
    }
    else if(tile->type==TILE_TYPE_DOOR_CLOSED || tile->type==TILE_TYPE_DOOR_OPEN || tile->type==TILE_TYPE_SECRET_DOOR){
        color=color_shorts_to_doubles(89,44,0);
    }

    return color;
}
