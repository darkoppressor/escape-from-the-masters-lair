/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "string_input.h"
#include "world.h"
#include "quit.h"
#include "save_load.h"
#include "version.h"
#include "message_log.h"

using namespace std;
using namespace boost::algorithm;

string_input::string_input(){
    str1="";
    last_string="";
}

void string_input::handle_events(){
    if(event.type==SDL_KEYDOWN){
        if(str1.length()<110){/**If the string is less than the maximum size.*/
            if(event.key.keysym.unicode>=(Uint16)' ' && event.key.keysym.unicode<=(Uint16)'~'){
                str1+=(char)event.key.keysym.unicode;
            }
        }

        if(event.key.keysym.sym==SDLK_BACKSPACE && str1.length()>0){/**If the backspace key is pressed and the string is not empty.*/
            str1.erase(str1.length()-1);/**Remove one character from the end of the string.*/
        }

        else if(event.key.keysym.sym==SDLK_UP){
            str1=last_string;
        }
        else if(event.key.keysym.sym==SDLK_DOWN){
            str1.clear();
        }

        else if((event.key.keysym.sym==SDLK_RETURN || event.key.keysym.sym==SDLK_KP_ENTER) && str1.length()>0){/**If the player hits enter.*/
            string str_command=str1;

            //Remember this string.
            last_string=str_command;

            trim(str_command);

            if(istarts_with(str_command,"info")){
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

            else if(istarts_with(str_command,"dev")){
                player.option_dev=!player.option_dev;
                options_save();
                if(player.option_dev){
                    update_text_log("Dev mode on.",true,MESSAGE_SYSTEM);
                }
                else{
                    update_text_log("Dev mode off.",true,MESSAGE_SYSTEM);
                }
            }

            else if(istarts_with(str_command,"health bars")){
                player.option_healthbars=!player.option_healthbars;
                options_save();
                if(player.option_healthbars){
                    update_text_log("Health bar display on.",true,MESSAGE_SYSTEM);
                }
                else{
                    update_text_log("Health bar display off.",true,MESSAGE_SYSTEM);
                }
            }

            else if(player.option_dev && istarts_with(str_command,"add")){
                ierase_first(str_command,"add");
                trim(str_command);

                string message="";

                short item_category=-1;
                int item_index=-1;
                int stack_size=1;

                if(istarts_with(str_command,"weapon")){
                    ierase_first(str_command,"weapon");
                    item_category=ITEM_WEAPON;
                }
                else if(istarts_with(str_command,"armor")){
                    ierase_first(str_command,"armor");
                    item_category=ITEM_ARMOR;
                }
                else if(istarts_with(str_command,"food")){
                    ierase_first(str_command,"food");
                    item_category=ITEM_FOOD;
                }
                else if(istarts_with(str_command,"drink")){
                    ierase_first(str_command,"drink");
                    item_category=ITEM_DRINK;
                }
                else if(istarts_with(str_command,"scroll")){
                    ierase_first(str_command,"scroll");
                    item_category=ITEM_SCROLL;
                }
                else if(istarts_with(str_command,"book")){
                    ierase_first(str_command,"book");
                    item_category=ITEM_BOOK;
                }
                else if(istarts_with(str_command,"container")){
                    ierase_first(str_command,"container");
                    item_category=ITEM_CONTAINER;
                }
                else if(istarts_with(str_command,"other")){
                    ierase_first(str_command,"other");
                    item_category=ITEM_OTHER;
                }

                trim(str_command);
                item_index=atoi(str_command.c_str());

                string temp="";
                ss.clear();ss.str("");ss<<item_index;temp=ss.str();
                ierase_first(str_command,temp);
                trim(str_command);

                if(str_command.size()>0){
                    stack_size=atoi(str_command.c_str());
                }

                //If the specified item can be added.
                if(item_category!=-1 && item_index!=-1 && item_category<templates.template_items.size() && item_index<templates.template_items[item_category].size()){
                    //If the inventory is not full, or the item is money, add the item.
                    if(player.inventory.size()<INVENTORY_MAX_SIZE || templates.template_items[item_category][item_index].inventory_letter=='$'){
                        //Generate the item.
                        Item temp_item;

                        //Apply the selected template to the item.
                        temp_item=templates.template_items[item_category][item_index];

                        //Run the item's setup function.
                        temp_item.setup();

                        //Apply the stack size.
                        if(temp_item.stackable){
                            temp_item.stack=stack_size;
                        }

                        message="Adding ";
                        message+=temp_item.return_full_name();
                        message+=" to your inventory.";

                        //Check to see if there is an identical item already in the inventory.
                        inventory_match match_check=player.check_for_inventory_match(&temp_item);

                        //If there is already an identical item in the inventory, and the item is stackable.
                        if(match_check.match_found && temp_item.stackable){
                            player.inventory[match_check.inventory_slot].stack+=temp_item.stack;
                        }
                        //If there is no identical item in the inventory, or the item is not stackable.
                        else{
                            //Determine an inventory letter for the item.

                            //Assign the item an available inventory letter.
                            temp_item.inventory_letter=player.assign_inventory_letter();

                            //Add the item to the inventory items vector.
                            player.inventory.push_back(temp_item);

                            //Assign an identifier to the item.
                            player.inventory[player.inventory.size()-1].assign_identifier();

                            //Assign an owner identifier to the item.
                            player.inventory[player.inventory.size()-1].owner=player.identifier;
                        }
                    }
                    //If the inventory is full and the item is not money.
                    else{
                        message="Your inventory is full.";
                    }
                }
                //If the specified item cannot be added.
                else{
                    message="The specified item does not exist.";
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

            else if(istarts_with(str_command,"save")){
                save_game();
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

            else if(istarts_with(str_command,"help")){
                update_text_log("-------------------------------------Help---------------------------------------",true,MESSAGE_SYSTEM);
                update_text_log("For a full list of commands, type '/commands'.",true,MESSAGE_SYSTEM);
            }

            else if(istarts_with(str_command,"commands")){
                update_text_log("-----------------------------------Commands-------------------------------------",true,MESSAGE_SYSTEM);
                update_text_log("/info",true,MESSAGE_SYSTEM);
                update_text_log("/help",true,MESSAGE_SYSTEM);
                update_text_log("/about",true,MESSAGE_SYSTEM);
                update_text_log("/exit",true,MESSAGE_SYSTEM);
                update_text_log("/fullscreen",true,MESSAGE_SYSTEM);
            }

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

            else if(istarts_with(str_command,"quit") || istarts_with(str_command,"exit")){
                quit_game();
            }

            else if(istarts_with(str_command,"fullscreen") || istarts_with(str_command,"fs")){
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

        else if(event.key.keysym.sym==SDLK_ESCAPE){
            player.chat_mode=false;
            str1.clear();

            //Make sure that escape is cleared before moving to the next screen.
            Uint8 *keystates=SDL_GetKeyState(NULL);/**Get keystates.*/
            keystates[SDLK_ESCAPE]=NULL;
        }
    }
}
