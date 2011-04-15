/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "update.h"
#include "world.h"
#include "render.h"
#include "material_properties.h"

#include <SDL.h>

#include <ctime>

using namespace std;

//Gather input.
void input(){
    tooltip.on=false;

    if(player.game_in_progress){
        //Gather input from the player.
        player.handle_input();
    }
    else{
        player.handle_input_no_game();
    }
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

    //Process the tiles' turn stuff.
    for(int x=0;x<vector_levels[current_level].level_x;x++){
        for(int y=0;y<vector_levels[current_level].level_y;y++){
            vector_levels[current_level].tiles[x][y].process_turn();
        }
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
    for(int i=0;i<vector_levels[current_level].items.size();i++){
        //If the item has been destroyed.
        if(!vector_levels[current_level].items[i].alive){
            vector_levels[current_level].items.erase(vector_levels[current_level].items.begin()+i);
            i--;
        }
    }

    ///I may need to add some kind of check eventually, so movement cannot resume until no items need to move.
    //Move the items.
    for(int i=0;i<vector_levels[current_level].items.size();i++){
        vector_levels[current_level].items[i].move();
    }

    //Look through all of the monsters.
    for(int i=0;i<vector_levels[current_level].monsters.size();i++){
        //If the monster is dead.
        if(!vector_levels[current_level].monsters[i].alive){
            //Drop the monsters's inventory.

            for(int n=0;n<vector_levels[current_level].monsters[i].inventory.size();n++){
                vector_levels[current_level].monsters[i].drop_item(n);
            }

            //Drop the monster's corpse.

            //Create a temporary corpse item.
            Item temp_item=templates.template_items[ITEM_FOOD][0];

            double temp_item_size=0.0;

            //Set the item's info to the corpse's race.
            temp_item.race=vector_levels[current_level].monsters[i].race;

            //Transfer the monster's coverings to the corpse.
            temp_item.coverings=vector_levels[current_level].monsters[i].coverings;

            string temp_name=templates.template_races[temp_item.race].name;
            temp_name+=" ";
            temp_item.name=temp_name+temp_item.name;

            temp_item.color=templates.template_races[temp_item.race].color;

            temp_item.weight=templates.template_races[temp_item.race].weight;

            temp_item_size=(temp_item.weight*2)/specific_gravities[temp_item.material];

            temp_item.monetary_value=(temp_item.weight*values[temp_item.material])/2.0;
            temp_item.monetary_value+=(temp_item.effects.size()*values[temp_item.material])/4.0;
            if(temp_item.category==ITEM_WEAPON){
                temp_item.monetary_value+=(6*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_ARMOR){
                temp_item.monetary_value+=(5*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_FOOD){
                temp_item.monetary_value+=(1*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_DRINK){
                temp_item.monetary_value+=(3*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_SCROLL){
                temp_item.monetary_value+=(4*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_BOOK){
                temp_item.monetary_value+=(4*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_CONTAINER){
                temp_item.monetary_value+=(2*values[temp_item.material])/4.0;
            }
            else if(temp_item.category==ITEM_OTHER){
                temp_item.monetary_value+=(1*values[temp_item.material])/4.0;
            }

            temp_item.damage_min_melee=1;
            temp_item.damage_max_melee=(temp_item_size*temp_item.weight)/6.0;

            if(temp_item.damage_min_melee<1){
                temp_item.damage_min_melee=1;
            }
            if(temp_item.damage_max_melee<1){
                temp_item.damage_max_melee=1;
            }

            temp_item.damage_min_thrown=1;
            temp_item.damage_max_thrown=temp_item.damage_max_melee/2.0;

            if(temp_item.damage_min_thrown<1){
                temp_item.damage_min_thrown=1;
            }
            if(temp_item.damage_max_thrown<1){
                temp_item.damage_max_thrown=1;
            }

            //Set the item's position to the creature's current position.
            temp_item.x=vector_levels[current_level].monsters[i].x;
            temp_item.y=vector_levels[current_level].monsters[i].y;

            //Add the item to the dungeon items vector.
            vector_levels[current_level].items.push_back(temp_item);

            //Assign an identifier to the new item.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].assign_identifier();

            //Add blood around this creature's current tile.
            if(random_range(0,99)<75){
                ///Maybe this should be based on the creature's size.
                if(random_range(0,99)<30){
                    vector_levels[current_level].draw_circle_covering(vector_levels[current_level].monsters[i].x,vector_levels[current_level].monsters[i].y,random_range(4,5),COVERING_BLOOD_LOTS,60);
                }
                else{
                    vector_levels[current_level].draw_circle_covering(vector_levels[current_level].monsters[i].x,vector_levels[current_level].monsters[i].y,random_range(3,4),COVERING_BLOOD_LITTLE,60);
                }
            }

            //Remove the monster from the game.

            //Return the monster's identifier.
            vector_levels[current_level].monsters[i].return_identifier();

            //Erase the monster.
            vector_levels[current_level].monsters.erase(vector_levels[current_level].monsters.begin()+i);

            //Decrement i.
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
    if(player.game_in_progress){
        //Handle camera movements for the players.
        player.set_camera();
    }

    player.update_window_caption(frame_rate,ms_per_frame);
}

//Render everything to the screen.
void render(int frame_rate, double ms_per_frame){
    //Clear the screen, filling it with the clear color.
    glClear(GL_COLOR_BUFFER_BIT);

    if(player.game_in_progress){
        if(player.current_window==WINDOW_NONE || player.current_window==WINDOW_DEATH_1 || player.current_window==WINDOW_CONFIRM_LEAVE_DUNGEON){
            //Used to keep track of what spaces have something rendered on them this frame.
            vector< vector<bool> > tile_rendered;
            tile_rendered.resize(vector_levels[current_level].level_x,vector<bool>(vector_levels[current_level].level_y));
            for(int x=0;x<vector_levels[current_level].level_x;x++){
                for(int y=0;y<vector_levels[current_level].level_y;y++){
                    tile_rendered[x][y]=false;
                }
            }

            //Set the camera's current tile position.
            int camera_current_x=(int)((int)player.camera_x/TILE_SIZE_X);
            int camera_current_y=(int)((int)player.camera_y/TILE_SIZE_Y);

            //Check all tiles in a rectangle around the camera.
            int check_x_start=camera_current_x;
            int check_x_end=camera_current_x+(int)((int)player.camera_w/TILE_SIZE_X)+2;
            int check_y_start=camera_current_y;
            int check_y_end=camera_current_y+(int)((int)player.camera_h/TILE_SIZE_Y)+2;

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

            //Render chat stuff:

            render_rectangle(0,main_window.SCREEN_HEIGHT-122,main_window.SCREEN_WIDTH,122,1.0,COLOR_GRAY);
            render_rectangle(2,main_window.SCREEN_HEIGHT-120,796,118,1.0,COLOR_BLACK);

            if(player.chat_mode){
                render_rectangle(2,main_window.SCREEN_HEIGHT-font_small.spacing_y-6,796,font.spacing_y-4,1.0,COLOR_GRAY);
                render_rectangle(4,main_window.SCREEN_HEIGHT-font_small.spacing_y-4,792,font.spacing_y-8,1.0,COLOR_BLACK);
            }

            double opacity=1.0;

            for(short i=player.text_log_display_position,n=5;i>player.text_log_display_position-6;i--,n--){
                //If the current text log line is not empty.
                if(player.text_log[i]!=""){
                    //Set opacity.
                    if(n==5){
                        opacity=1.0;
                    }
                    else if(n==4){
                        opacity=.84;
                    }
                    else if(n==3){
                        opacity=.68;
                    }
                    else if(n==2){
                        opacity=.52;
                    }
                    else if(n==1){
                        opacity=.36;
                    }
                    else if(n==0){
                        opacity=.20;
                    }

                    //If the message is a system message, print it to screen in yellow system message text.
                    if(player.text_log[i].rfind("\x1F",0,1)!=string::npos && player.text_log[i].rfind("\x1F",0,1)==0){
                        string temp_string=player.text_log[i].substr(1);
                        font_small.show(4,main_window.SCREEN_HEIGHT-36-n*font_small.spacing_y,temp_string,COLOR_SYSTEM,opacity);
                    }
                    //Otherwise, print it to screen normally.
                    else{
                        font_small.show(4,main_window.SCREEN_HEIGHT-36-n*font_small.spacing_y,player.text_log[i],COLOR_WHITE,opacity);
                    }
                }
            }

            if(player.chat_mode){
                short chat_input_height=main_window.SCREEN_HEIGHT-font_small.spacing_y-2;
                font_small.show(4,chat_input_height,"> "+string_input_chat.str1,COLOR_WHITE,1.0);
                font_small.show(17+font_small.spacing_x*string_input_chat.str1.length(),chat_input_height,"\x7F",COLOR_WHITE,player.cursor_opacity*0.1);
            }

            //Display the framerate and milliseconds per frame.
            /**if(player.option_fps){
                ss.clear();ss.str("");ss<<"FPS: ";ss<<frame_rate;ss<<"\xA";msg=ss.str();
                ss.clear();ss.str("");ss<<"MS Per Frame: ";ss<<ms_per_frame;msg+=ss.str();
                font_small.show(624,0,msg,COLOR_RED);
            }*/

            if(player.option_dev && player.chat_mode){
                render_rectangle(0,0,275,225,0.75,COLOR_BLACK);
                render_rectangle(5,5,265,215,0.75,COLOR_GRAY);
                ss.clear();ss.str("");ss<<"Max Dungeon Level: ";ss<<max_level+1;ss<<"\xA";msg=ss.str();
                ss.clear();ss.str("");ss<<"Level Temperature: ";ss<<vector_levels[current_level].temperature;ss<<"\xA";msg+=ss.str();
                ss.clear();ss.str("");ss<<"Player Position (in tiles): ";ss<<player.x;ss<<"/";ss<<player.y;ss<<"\xA";msg+=ss.str();
                ss.clear();ss.str("");ss<<"Camera Position (in pixels): ";ss<<player.camera_x;ss<<"/";ss<<player.camera_y;ss<<"\xA";msg+=ss.str();
                ss.clear();ss.str("");ss<<"Inventory Size: ";ss<<player.inventory.size();ss<<"\xA";msg+=ss.str();
                ss.clear();ss.str("");ss<<"Level Items Size: ";ss<<vector_levels[current_level].items.size();ss<<"\xA";msg+=ss.str();
                ss.clear();ss.str("");ss<<"Level Monsters Size: ";ss<<vector_levels[current_level].monsters.size();ss<<"\xA";msg+=ss.str();
                ss.clear();ss.str("");ss<<"Level Size: ";ss<<vector_levels[current_level].level_x;ss<<"/";ss<<vector_levels[current_level].level_y;ss<<"\xA";msg+=ss.str();
                ss.clear();ss.str("");ss<<"CPU Architecture: ";ss<<8*sizeof(void*);ss<<"-bit";ss<<"\xA";msg+=ss.str();
                ss.clear();ss.str("");ss<<"Thirst: ";ss<<player.thirst;ss<<"\xA";msg+=ss.str();
                ss.clear();ss.str("");ss<<"Weight: ";ss<<player.weight;ss<<"\xA";msg+=ss.str();
                ss.clear();ss.str("");ss<<"Level Temp: ";ss<<vector_levels[current_level].temperature;ss<<"\xA";msg+=ss.str();
                font_small.show(5,5,msg,COLOR_WHITE);
            }

            bool thirsty=false;
            bool burdened=false;

            //If the player is anything aside from unencumbered.
            if(!(player.return_inventory_weight()<=player.return_carry_capacity())){
                burdened=true;
            }

            //If the player is anything other than not thirsty.
            if(!(player.thirst>=THIRST_NOT_THIRSTY && player.thirst<THIRST_THIRSTY)){
                thirsty=true;
            }

            short burden_color=COLOR_WHITE;
            //If the player is unencumbered.
            if(player.return_inventory_weight()<=player.return_carry_capacity()){
                burden_color=COLOR_GREEN;
            }
            //If the player is burdened.
            else if(player.return_inventory_weight()>=player.return_carry_capacity()+1 && player.return_inventory_weight()<=player.return_carry_capacity()*1.5){
                burden_color=COLOR_YELLOW;
            }
            //If the player is stressed.
            else if(player.return_inventory_weight()>=player.return_carry_capacity()*1.5+1 && player.return_inventory_weight()<=player.return_carry_capacity()*2.0){
                burden_color=COLOR_ORANGE;
            }
            //If the player is strained.
            else if(player.return_inventory_weight()>=player.return_carry_capacity()*2.0+1 && player.return_inventory_weight()<=player.return_carry_capacity()*2.5){
                burden_color=COLOR_PUMPKIN;
            }
            //If the player is overtaxed.
            else if(player.return_inventory_weight()>=player.return_carry_capacity()*2.5+1 && player.return_inventory_weight()<=player.return_carry_capacity()*3.0){
                burden_color=COLOR_RED;
            }
            //If the player is overloaded.
            else if(player.return_inventory_weight()>=player.return_carry_capacity()*3.0+1){
                burden_color=COLOR_DARK_RED;
            }

            short thirst_color=COLOR_WHITE;
            //If the player is bloated.
            if(player.thirst<=THIRST_BLOATED){
                thirst_color=COLOR_PURPLE;
            }
            //If the player is satiated.
            else if(player.thirst>=THIRST_SATIATED && player.thirst<THIRST_NOT_THIRSTY){
                thirst_color=COLOR_BLUE;
            }
            //If the player is not thirsty.
            else if(player.thirst>=THIRST_NOT_THIRSTY && player.thirst<THIRST_THIRSTY){
                thirst_color=COLOR_GREEN;
            }
            //If the player is thirsty.
            else if(player.thirst>=THIRST_THIRSTY && player.thirst<THIRST_WEAK){
                thirst_color=COLOR_YELLOW;
            }
            //If the player is weak.
            else if(player.thirst>=THIRST_WEAK && player.thirst<THIRST_FAINTING){
                thirst_color=COLOR_ORANGE;
            }
            //If the player is fainting.
            else if(player.thirst>=THIRST_FAINTING && player.thirst<THIRST_DEATH){
                thirst_color=COLOR_RED;
            }
            //If the player is dead.
            else if(player.thirst>=THIRST_DEATH){
                thirst_color=COLOR_DARK_RED;
            }

            short health_color=COLOR_WHITE;
            if(player.return_health()>=player.return_health_max()*0.75){
                health_color=COLOR_GREEN;
            }
            else if(player.return_health()>=player.return_health_max()*0.50 && player.return_health()<player.return_health_max()*0.75){
                health_color=COLOR_YELLOW;
            }
            else if(player.return_health()>=player.return_health_max()*0.25 && player.return_health()<player.return_health_max()*0.50){
                health_color=COLOR_ORANGE;
            }
            else{
                health_color=COLOR_RED;
            }

            short mana_color=COLOR_WHITE;
            if(player.return_mana()>=player.return_mana_max()*0.75){
                mana_color=COLOR_GREEN;
            }
            else if(player.return_mana()>=player.return_mana_max()*0.50 && player.return_mana()<player.return_mana_max()*0.75){
                mana_color=COLOR_YELLOW;
            }
            else if(player.return_mana()>=player.return_mana_max()*0.25 && player.return_mana()<player.return_mana_max()*0.50){
                mana_color=COLOR_ORANGE;
            }
            else{
                mana_color=COLOR_RED;
            }

            int size_last_msg=0;

            render_rectangle(0,main_window.SCREEN_HEIGHT-174,800,54,1.0,COLOR_GRAY);
            render_rectangle(2,main_window.SCREEN_HEIGHT-172,796,50,1.0,COLOR_BLACK);

            ss.clear();ss.str("");ss<<player.name;ss<<" the ";ss<<player.race_name;ss<<" ";ss<<player.class_name;msg=ss.str();
            font_small.show(5,main_window.SCREEN_HEIGHT-170,msg,COLOR_WHITE);
            size_last_msg=msg.length();

            //If the player is anything other than not thirsty.
            if(thirsty){
                if(!burdened){
                    ss.clear();ss.str("");ss<<" is ";ss<<player.return_thirst_state();ss<<".";msg=ss.str();
                }
                else{
                    ss.clear();ss.str("");ss<<" is ";ss<<player.return_thirst_state();msg=ss.str();
                }
                font_small.show(5+font_small.spacing_x*size_last_msg,main_window.SCREEN_HEIGHT-170,msg,thirst_color);
                size_last_msg+=msg.length();
            }

            //If the player is anything aside from unencumbered.
            if(burdened){
                if(!thirsty){
                    ss.clear();ss.str("");ss<<" is ";ss<<player.return_encumbrance_state();ss<<".";msg=ss.str();
                }
                else{
                    ss.clear();ss.str("");ss<<" and ";ss<<player.return_encumbrance_state();ss<<".";msg=ss.str();
                }
                font_small.show(5+font_small.spacing_x*size_last_msg,main_window.SCREEN_HEIGHT-170,msg,burden_color);
            }

            ss.clear();ss.str("");ss<<"Experience Level:";ss<<player.experience_level;msg=ss.str();
            ss.clear();ss.str("");ss<<" (";ss<<player.experience;ss<<"/";ss<<player.experience_max;ss<<")";msg+=ss.str();
            ss.clear();ss.str("");ss<<"  Dungeon Level:";ss<<current_level+1;msg+=ss.str();
            ss.clear();ss.str("");ss<<"  Turn:";ss<<player.turn;msg+=ss.str();
            font_small.show(5,main_window.SCREEN_HEIGHT-170+font_small.spacing_y,msg,COLOR_WHITE);

            ss.clear();ss.str("");ss<<"Health:";ss<<player.return_health();ss<<"/";ss<<player.return_health_max();msg=ss.str();
            font_small.show(5,main_window.SCREEN_HEIGHT-170+font_small.spacing_y*2,msg,health_color);
            size_last_msg=msg.length();

            ss.clear();ss.str("");ss<<"  Mana:";ss<<player.return_mana();ss<<"/";ss<<player.return_mana_max();msg=ss.str();
            font_small.show(5+font_small.spacing_x*size_last_msg,main_window.SCREEN_HEIGHT-170+font_small.spacing_y*2,msg,mana_color);
            size_last_msg+=msg.length();

            ss.clear();ss.str("");ss<<"  Armor:";ss<<player.return_armor();msg=ss.str();
            ss.clear();ss.str("");ss<<"  Carrying:";ss<<(int)player.return_inventory_weight();ss<<"/";ss<<(int)player.return_carry_capacity();msg+=ss.str();
            font_small.show(5+font_small.spacing_x*size_last_msg,main_window.SCREEN_HEIGHT-170+font_small.spacing_y*2,msg,COLOR_WHITE);
        }
        else if(player.current_window==WINDOW_INVENTORY){
            player.render_inventory(true);
        }
        else if(player.current_window==WINDOW_STATS){
            player.render_stats();
        }
        else if(player.current_window==WINDOW_LEVELUP){
            player.render_levelup();
        }
        else if(player.current_window==WINDOW_DEATH){
            player.render_death();
        }
        else if(player.current_window==WINDOW_LEAVE_DUNGEON){
            player.render_leave_dungeon();
        }
        else if(player.current_window==WINDOW_INTERACTIVE_INVENTORY_RELEVANT){
            player.render_inventory(false);
        }
        else if(player.current_window==WINDOW_INTERACTIVE_INVENTORY_ALL){
            player.render_inventory(true);
        }
        else if(player.current_window==WINDOW_ITEM_INFO){
            player.render_item_info();
        }
    }
    else{
        player.render_no_game();
    }

    if(tooltip.on){
        tooltip.render();
    }

    render_window();

    //Swap the buffers, updating the screen.
    SDL_GL_SwapBuffers();
}
