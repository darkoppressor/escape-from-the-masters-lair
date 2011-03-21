/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "update.h"
#include "world.h"
#include "render.h"

#include <SDL.h>

#include <ctime>

using namespace std;

//Gather input.
void input(){
    tooltip.on=false;

    //Gather input from the player.
    player.handle_input();
}

void turn(){
    //Process the player's turn stuff.
    player.process_turn();

    //Process the monsters' turn stuff.
    for(int i=0;i<vector_levels[current_level].monsters.size();i++){
        vector_levels[current_level].monsters[i].process_turn();
    }

    //Process the items' turn stuff.
    for(int i=0;i<vector_levels[current_level].items.size();i++){
        vector_levels[current_level].items[i].process_turn();
    }

    bool player_moved=false;

    //As long as the player has not moved.
    while(!player_moved){
        //Tick down towards the player's next move.
        player.next_move--;

        //If it is time for the player's next move.
        if(player.return_next_move()<=0){
            player.move();
            player.next_move=player.return_movement_speed();
            player_moved=true;
        }

        //Tick down towards all monsters' next moves.
        for(int i=0;i<vector_levels[current_level].monsters.size();i++){
            vector_levels[current_level].monsters[i].next_move--;

            if(vector_levels[current_level].monsters[i].return_next_move()<=0){
                vector_levels[current_level].monsters[i].handle_ai();
                vector_levels[current_level].monsters[i].handle_input();
                vector_levels[current_level].monsters[i].move();
                vector_levels[current_level].monsters[i].next_move=vector_levels[current_level].monsters[i].return_movement_speed();
            }
        }
    }
}

//Handle events, such as collisions.
void events(){
    ///I may need to add some kind of check eventually, so movement cannot resume until no items need to move.
    //Move the items.
    for(int i=0;i<vector_levels[current_level].items.size();i++){
        vector_levels[current_level].items[i].move();
    }

    //Look through all of the monsters.
    for(int i=0;i<vector_levels[current_level].monsters.size();i++){
        //If the monster is dead.
        if(!vector_levels[current_level].monsters[i].alive){
            ///Drop the monster's items and corpse.
            for(int n=0;n<vector_levels[current_level].monsters[i].inventory.size();n++){
                //If the item is either not money, or is money but with at least 1 in the stack.
                if(vector_levels[current_level].monsters[i].inventory[n].inventory_letter!='$' || (vector_levels[current_level].monsters[i].inventory[n].inventory_letter=='$' && vector_levels[current_level].monsters[i].inventory[n].stack>0)){
                    //Return the item's inventory letter.
                    vector_levels[current_level].monsters[i].return_inventory_letter(vector_levels[current_level].monsters[i].inventory[n].inventory_letter);

                    //If the creature is not the player.
                    if(!vector_levels[current_level].monsters[i].is_player){
                        //Reset the item's inventory letter.
                        //We only do this for monsters. The player leaves the letter attached to the item.
                        vector_levels[current_level].monsters[i].inventory[n].inventory_letter=0;
                    }

                    //Set the item's position to the creature's current position.
                    vector_levels[current_level].monsters[i].inventory[n].x=vector_levels[current_level].monsters[i].x;
                    vector_levels[current_level].monsters[i].inventory[n].y=vector_levels[current_level].monsters[i].y;

                    //Add the item to the dungeon items vector.
                    vector_levels[current_level].items.push_back(vector_levels[current_level].monsters[i].inventory[n]);

                    //Assign an identifier to the new item.
                    vector_levels[current_level].items[vector_levels[current_level].items.size()-1].assign_identifier();

                    //Return the inventory item's identifier.
                    vector_levels[current_level].monsters[i].inventory[n].return_identifier();
                }
            }

            //Drop the creature's corpse.

            //Create a temporary corpse item.
            Item temp_item=templates.template_items[ITEM_FOOD][0];

            //Set the item's info to the corpse's race.
            string temp_name=templates.template_races[vector_levels[current_level].monsters[i].race].name;
            temp_name+=" ";
            temp_item.name=temp_name+temp_item.name;

            temp_item.color=templates.template_races[vector_levels[current_level].monsters[i].race].color;

            temp_item.weight=templates.template_races[vector_levels[current_level].monsters[i].race].weight;

            temp_item.damage_max_melee=temp_item.weight/6;

            temp_item.damage_max_thrown=temp_item.damage_max_melee/2;

            //Set the item's position to the creature's current position.
            temp_item.x=vector_levels[current_level].monsters[i].x;
            temp_item.y=vector_levels[current_level].monsters[i].y;

            //Add the item to the dungeon items vector.
            vector_levels[current_level].items.push_back(temp_item);

            //Assign an identifier to the new item.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].assign_identifier();

            ///Erase the monster.
            vector_levels[current_level].monsters.erase(vector_levels[current_level].monsters.begin()+i);

            ///Decrement i.
            i--;
        }
    }
}

void animation(){
    //Update the chat cursor.
    if(player.timer_cursor.get_ticks()>=50){
        if(player.cursor_opacity==10){
            player.cursor=false;
        }
        else if(player.cursor_opacity==0){
            player.cursor=true;
        }

        if(player.cursor && player.cursor_opacity<10){
            player.cursor_opacity+=1;
        }
        else if(!player.cursor && player.cursor_opacity>0){
            player.cursor_opacity-=1;
        }

        player.timer_cursor.start();
    }
}

//Handle camera movement.
void camera(int frame_rate, double ms_per_frame){
    //Handle camera movements for the players.
    player.set_camera();

    player.update_window_caption(frame_rate,ms_per_frame);
}

//Render everything to the screen.
void render(int frame_rate, double ms_per_frame){
    //Clear the screen, filling it with the clear color set in Window.init().
    glClear(GL_COLOR_BUFFER_BIT);

    //Used to keep track of what spaces have something rendered on them this frame.
    vector< vector<bool> > tile_rendered;
    tile_rendered.resize(vector_levels[current_level].level_x,vector<bool>(vector_levels[current_level].level_y));
    for(int x=0;x<vector_levels[current_level].level_x;x++){
        for(int y=0;y<vector_levels[current_level].level_y;y++){
            tile_rendered[x][y]=false;
        }
    }

    //Set the camera's current tile position.
    int camera_current_x=(int)((int)player.camera_x/TILE_SIZE);
    int camera_current_y=(int)((int)player.camera_y/TILE_SIZE);

    //Check all tiles in a rectangle around the camera.
    int check_x_start=camera_current_x;
    int check_x_end=camera_current_x+(int)((int)player.camera_w/TILE_SIZE)+2;
    int check_y_start=camera_current_y;
    int check_y_end=camera_current_y+(int)((int)player.camera_h/TILE_SIZE)+2;

    //Render each player held in the player vector in sequence.
    player.render(&tile_rendered);

    for(int i=0;i<vector_levels[current_level].monsters.size();i++){
        vector_levels[current_level].monsters[i].render(&tile_rendered);
    }

    for(int i=0;i<vector_levels[current_level].items.size();i++){
        vector_levels[current_level].items[i].render(&tile_rendered);
    }

    //Render each tile held in the tile vector in sequence.
    for(int int_y=check_y_start;int_y<check_y_end;int_y++){
        for(int int_x=check_x_start;int_x<check_x_end;int_x++){
            //As long as the current tile is within the level's boundaries.
            if(int_x>=0 && int_x<=vector_levels[current_level].level_x-1 && int_y>=0 && int_y<=vector_levels[current_level].level_y-1){
                vector_levels[current_level].tiles[int_x][int_y].render(&tile_rendered);
            }
        }
    }

    /**if(window_inventory.on){
        window_inventory.render();
    }*/

    //Render chat stuff:

    if(player.chat_mode){
        render_rectangle(0,main_window.SCREEN_HEIGHT-174,800,174,1.0,COLOR_GRAY);
        render_rectangle(10,main_window.SCREEN_HEIGHT-164,780,156,1.0,COLOR_BLACK);
    }

    double opacity=1.0;

    for(short i=player.text_limit-1;i>=0;i--){
        //If the current text log line is not empty.
        if(player.text_log[i]!=""){
            //Set opacity.
            if(i==player.text_limit-1){
                opacity=1.0;
            }
            else if(i==player.text_limit-2){
                opacity=.84;
            }
            else if(i==player.text_limit-3){
                opacity=.68;
            }
            else if(i==player.text_limit-4){
                opacity=.52;
            }
            else if(i==player.text_limit-5){
                opacity=.36;
            }
            else if(i==player.text_limit-6){
                opacity=.20;
            }
            else{
                opacity=.04;
            }

            //If the message is a system message, print it to screen in yellow system message text.
            if(player.text_log[i].rfind("\x1F",0,1)!=string::npos && player.text_log[i].rfind("\x1F",0,1)==0){
                string temp_string=player.text_log[i].substr(1);
                font.show(10,main_window.SCREEN_HEIGHT-50-i*22,temp_string,COLOR_SYSTEM,opacity);
            }
            //Otherwise, print it to screen normally.
            else{
                font.show(10,main_window.SCREEN_HEIGHT-50-i*22,player.text_log[i],COLOR_WHITE,opacity);
            }
        }
    }

    if(player.chat_mode){
        short chat_input_height=main_window.SCREEN_HEIGHT-26;
        font.show(10,chat_input_height,"> "+string_input_chat.str1,COLOR_WHITE,1.0);
        font.show(32+11*string_input_chat.str1.length(),chat_input_height-1,"\x7F",COLOR_WHITE,player.cursor_opacity*0.1);
    }

    player.render_inventory();
    player.render_stats();

    if(tooltip.on){
        tooltip.render();
    }

    //Display the framerate and milliseconds per frame.
    /**if(player.option_fps){
        ss.clear();ss.str("");ss<<"FPS: ";ss<<frame_rate;ss<<"\xA";msg=ss.str();
        ss.clear();ss.str("");ss<<"MS Per Frame: ";ss<<ms_per_frame;msg+=ss.str();
        font.show(624,0,msg,COLOR_RED);
    }*/

    if(player.option_dev && player.chat_mode){
        render_rectangle(0,0,450,275,0.75,COLOR_BLACK);
        render_rectangle(10,10,430,255,0.75,COLOR_GRAY);
        ss.clear();ss.str("");ss<<"Level: ";ss<<current_level+1;ss<<"\xA";msg=ss.str();
        ss.clear();ss.str("");ss<<"Last Level: ";ss<<last_level+1;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Max Level: ";ss<<max_level+1;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Level Temp: ";ss<<vector_levels[current_level].temperature;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Player Position (in tiles): ";ss<<player.x;ss<<"/";ss<<player.y;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Camera Position (in pixels): ";ss<<player.camera_x;ss<<"/";ss<<player.camera_y;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Inventory Size: ";ss<<player.inventory.size();ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Level Items Size: ";ss<<vector_levels[current_level].items.size();ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Level Monsters Size: ";ss<<vector_levels[current_level].monsters.size();ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Level Size: ";ss<<vector_levels[current_level].level_x;ss<<"/";ss<<vector_levels[current_level].level_y;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"CPU Architecture: ";ss<<8*sizeof(void*);ss<<"-bit";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Thirst: ";ss<<player.thirst;ss<<"\xA";msg+=ss.str();
        font.show(15,15,msg,COLOR_WHITE);
    }
    else{
        //Only show the game info when the inventory is closed.
        if(!player.display_inventory && !player.display_stats){
            ///render_rectangle(0,0,450,275,0.75,COLOR_BLACK);
            ///render_rectangle(10,10,430,255,0.75,COLOR_GRAY);
            ss.clear();ss.str("");ss<<"Dungeon Level: ";ss<<current_level+1;ss<<"\xA";msg=ss.str();
            ss.clear();ss.str("");ss<<"Turn: ";ss<<player.turn;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Experience Level: ";ss<<player.experience_level;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Experience: ";ss<<player.experience;ss<<"/";ss<<player.experience_max;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Health: ";ss<<player.return_health();ss<<"/";ss<<player.return_health_max();ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Mana: ";ss<<player.return_mana();ss<<"/";ss<<player.return_mana_max();ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Money: ";ss<<player.inventory[0].stack;ss<<"\xA";msg+=ss.str();
            //If the player is anything other than not thirsty.
            if(!(player.thirst>=THIRST_NOT_THIRSTY && player.thirst<THIRST_THIRSTY)){
                ss.clear();ss.str("");ss<<"You are ";ss<<player.return_thirst_state();ss<<".";ss<<"\xA";msg+=ss.str();
            }
            //If the player is anything aside from unencumbered.
            if(!(player.return_inventory_weight()<=player.return_carry_capacity())){
                ss.clear();ss.str("");ss<<"You are ";ss<<player.return_encumbrance_state();ss<<".";ss<<"\xA";msg+=ss.str();
            }
            font.show(15,15,msg,COLOR_WHITE);
        }
    }

    //Swap the buffers, updating the screen.
    SDL_GL_SwapBuffers();
}
