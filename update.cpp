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

//Move things that need to be moved.
void movement(){
    bool player_moved=false;

    while(!player_moved){
        //Tick down towards the player's next move.
        player.next_move--;

        if(player.next_move<=0){
            player.move();
            player.next_move=player.movement_speed;
            player_moved=true;
        }

        //Tick down towards all monsters' next moves.
        for(int i=0;i<vector_levels[current_level].monsters.size();i++){
            vector_levels[current_level].monsters[i].next_move--;

            if(vector_levels[current_level].monsters[i].next_move<=0){
                vector_levels[current_level].monsters[i].handle_ai();
                vector_levels[current_level].monsters[i].handle_input();
                vector_levels[current_level].monsters[i].move();
                vector_levels[current_level].monsters[i].next_move=vector_levels[current_level].monsters[i].movement_speed;
            }
        }
    }
}

//Handle events, such as collisions.
void events(){
    player.update_fov();
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
        font.show(15,15,msg,COLOR_WHITE);
    }
    else{
        //Only show the game info when the inventory is closed.
        if(!player.display_inventory){
            ///render_rectangle(0,0,450,275,0.75,COLOR_BLACK);
            ///render_rectangle(10,10,430,255,0.75,COLOR_GRAY);
            ss.clear();ss.str("");ss<<"Dungeon Level: ";ss<<current_level+1;ss<<"\xA";msg=ss.str();
            ss.clear();ss.str("");ss<<"Turn: ";ss<<player.turn;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Experience Level: ";ss<<player.experience_level;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Experience (Next): ";ss<<player.experience;ss<<" (";ss<<player.experience_max;ss<<")";ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Health: ";ss<<player.health;ss<<"/";ss<<player.health_max;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Mana: ";ss<<player.mana;ss<<"/";ss<<player.mana_max;ss<<"\xA";msg+=ss.str();
            ss.clear();ss.str("");ss<<"Money: ";ss<<player.inventory[0].stack;ss<<"\xA";msg+=ss.str();
            font.show(15,15,msg,COLOR_WHITE);
        }
    }

    //Swap the buffers, updating the screen.
    SDL_GL_SwapBuffers();
}
