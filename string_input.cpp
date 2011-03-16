/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "string_input.h"
#include "world.h"
#include "quit.h"
#include "save_load.h"
#include "version.h"
#include "message_log.h"

using namespace std;

string_input::string_input(){
    str1="";
}

string_input::~string_input(){
}

void string_input::handle_events(){
    if(event.type==SDL_KEYDOWN){
        if(str1.length()<67){/**If the string is less than the maximum size.*/
            if(event.key.keysym.unicode>=(Uint16)' ' && event.key.keysym.unicode<=(Uint16)'~'){
                str1+=(char)event.key.keysym.unicode;
            }
        }

        if(event.key.keysym.sym==SDLK_BACKSPACE && str1.length()!=0){/**If the backspace key is pressed and the string is not empty.*/
            str1.erase(str1.length()-1);/**Remove one character from the end of the string.*/
        }

        if((event.key.keysym.sym==SDLK_RETURN || event.key.keysym.sym==SDLK_KP_ENTER) && str1.length()>0){/**If the player hits enter.*/
            string str_command=str1;

            if(str_command=="info"){
                //Determine the date and time.
                time_t now;
                struct tm *tm_now;
                char buff[BUFSIZ];
                now=time(NULL);
                tm_now=localtime(&now);
                strftime(buff,sizeof buff,"%A, %B %d, %Y - %H:%M:%S",tm_now);

                update_text_log("------------------------------------Info---------------------------------------",true,MESSAGE_SYSTEM);

                msg="Name: ";
                msg+=player.name;
                update_text_log(msg.c_str(),true,MESSAGE_SYSTEM);

                msg=buff;
                update_text_log(msg.c_str(),true,MESSAGE_SYSTEM);
            }

            else if(str_command=="dev"){
                player.option_dev=!player.option_dev;
                options_save();
                if(player.option_dev){
                    update_text_log("Dev mode on.",true,MESSAGE_SYSTEM);
                }
                else{
                    update_text_log("Dev mode off.",true,MESSAGE_SYSTEM);
                }
            }

            else if(str_command=="save"){
                save_game();
            }

            else if(str_command=="fps"){
                player.option_fps=!player.option_fps;
                options_save();
                if(player.option_fps){
                    update_text_log("FPS will now be displayed.",true,MESSAGE_SYSTEM);
                }
                else{
                    update_text_log("FPS will no longer be displayed.",true,MESSAGE_SYSTEM);
                }
            }

            else if(str_command=="help" || str_command=="h"){
                update_text_log("-------------------------------------Help---------------------------------------",true,MESSAGE_SYSTEM);
                update_text_log("For a full list of commands, type '/commands'.",true,MESSAGE_SYSTEM);
            }

            else if(str_command=="commands"){
                update_text_log("-----------------------------------Commands-------------------------------------",true,MESSAGE_SYSTEM);
                update_text_log("/info",true,MESSAGE_SYSTEM);
                update_text_log("/help",true,MESSAGE_SYSTEM);
                update_text_log("/about",true,MESSAGE_SYSTEM);
                update_text_log("/exit",true,MESSAGE_SYSTEM);
                update_text_log("/fullscreen",true,MESSAGE_SYSTEM);
            }

            else if(str_command=="about" || str_command=="a"){
                string version="";
                ss.clear();ss.str("");ss<<"                               Version: ";ss<<AutoVersion::MAJOR;ss<<".";ss<<AutoVersion::MINOR;ss<<" ";ss<<AutoVersion::STATUS;version=ss.str();

                string date="";
                ss.clear();ss.str("");ss<<"                            Built on: ";ss<<AutoVersion::MONTH;ss<<"-";ss<<AutoVersion::DATE;ss<<"-";ss<<AutoVersion::YEAR;date=ss.str();

                update_text_log("--------------------------------------------------------------------------------",true,MESSAGE_SYSTEM);
                update_text_log(date.c_str(),true,MESSAGE_SYSTEM);
                update_text_log(version.c_str(),true,MESSAGE_SYSTEM);
                update_text_log("                            Created by: Kevin Wells",true,MESSAGE_SYSTEM);
                update_text_log("                                    Roguelike                                   ",true,MESSAGE_SYSTEM);
                update_text_log("--------------------------------------------------------------------------------",true,MESSAGE_SYSTEM);
            }

            else if(str_command=="quit" || str_command=="exit"){
                quit_game();
            }

            else if(str_command=="toggle fullscreen" || str_command=="togglefullscreen" || str_command=="toggle fs" || str_command=="togglefs" || str_command=="fullscreen" || str_command=="fs"){
                main_window.toggle_fullscreen();
                if(player.option_fullscreen){
                    update_text_log("Switched to fullscreen mode.",true,MESSAGE_SYSTEM);
                }
                else{
                    update_text_log("Switched to windowed mode.",true,MESSAGE_SYSTEM);
                }
            }

            else{
                update_text_log("Unrecognized command.",true,MESSAGE_SYSTEM);
            }

            str1.clear();

            player.chat_mode=false;

            //Make sure that enter is cleared before moving to the next screen.
            Uint8 *keystates=SDL_GetKeyState(NULL);/**Get keystates.*/
            keystates[SDLK_RETURN]=NULL;
            keystates[SDLK_KP_ENTER]=NULL;
        }

        else if((event.key.keysym.sym==SDLK_RETURN || event.key.keysym.sym==SDLK_KP_ENTER) && str1.length()==0){
            player.chat_mode=false;

            //Make sure that enter is cleared before moving to the next screen.
            Uint8 *keystates=SDL_GetKeyState(NULL);/**Get keystates.*/
            keystates[SDLK_RETURN]=NULL;
            keystates[SDLK_KP_ENTER]=NULL;
        }

        if(event.key.keysym.sym==SDLK_ESCAPE){
            player.chat_mode=false;
            str1.clear();

            //Make sure that escape is cleared before moving to the next screen.
            Uint8 *keystates=SDL_GetKeyState(NULL);/**Get keystates.*/
            keystates[SDLK_ESCAPE]=NULL;
        }
    }
}
