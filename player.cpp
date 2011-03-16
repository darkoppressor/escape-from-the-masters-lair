/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "player.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "quit.h"
#include "save_load.h"
#include "message_log.h"

using namespace std;

Player::Player(){
    // From Object: //

    name="\x1F";

    // From Creature: //

    is_player=true;

    light_on=true;
    source_data.permanent=true;

    // From Player: //

    keystates=NULL;

    //The camera starts out sticky.
    cam_state=CAM_STICKY;

    display_inventory=false;

    turn=0;

    //Chat stuff:

    chat_mode=false;

    text_limit=6;

    for(short i=0;i<text_limit;i++){
        text_log[i]="";
    }

    timer_cursor.start();
    cursor=false;
    cursor_opacity=10;

    //Options:
    option_fullscreen=false;
    option_dev=false;
    option_fps=true;

    //Set the camera's initial location and its dimensions.
    camera_x=0.0;
    camera_y=0.0;
    camera_w=800.0;
    camera_h=420.0;

    /**for(int i=97;i<=122;i++){
        inventory.push_back(Item());
        inventory[inventory.size()-1].name=(char)i;
    }
    for(int i=65;i<=90;i++){
        inventory.push_back(Item());
        inventory[inventory.size()-1].name=(char)i;
    }*/
}

void Player::set_base_stats(){
    // From creature: //

    health_max=templates.base_stats.health_max;
    health=health_max;

    mana_max=templates.base_stats.mana_max;
    mana=mana_max;

    base_damage_melee_min=templates.base_stats.base_damage_melee_min;
    base_damage_melee_max=templates.base_stats.base_damage_melee_max;

    base_damage_ranged_min=templates.base_stats.base_damage_ranged_min;
    base_damage_ranged_max=templates.base_stats.base_damage_ranged_max;

    base_damage_thrown_min=templates.base_stats.base_damage_thrown_min;
    base_damage_thrown_max=templates.base_stats.base_damage_thrown_max;
}

void Player::load_data(){
    if(name=="\x1F"){
        unsigned int temp_i=random_range(0,99999);
        ss.clear();ss.str("");ss<<"Player-";ss<<temp_i;msg=ss.str();
        name=msg;
    }
}

void Player::handle_input(){
    int mouse_x,mouse_y;

    //Get the SDL keystates and store them in the keystates variable for evaluation.
    keystates=SDL_GetKeyState(NULL);

    modstate=SDL_GetModState();

    SDL_GetMouseState(&mouse_x,&mouse_y);

    while(SDL_PollEvent(&event)){
        SDL_EnableUNICODE(SDL_ENABLE);

        if(chat_mode){
            string_input_chat.handle_events();
        }
        switch(event.type){
            case SDL_QUIT:
                quit_game();
                break;

            case SDL_KEYDOWN:
                if(!chat_mode){

                    //********************//
                    // Standard Commands: //
                    //********************//

                    //Toggle light item.
                    if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.sym==SDLK_l){
                        check_command(COMMAND_TOGGLE_LIGHT);
                    }

                    //Go down stairs.
                    else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.sym==SDLK_PERIOD && (modstate&KMOD_SHIFT)!=0){
                        check_command(COMMAND_GO_DOWN_STAIRS);
                    }

                    //Go up stairs.
                    else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.sym==SDLK_COMMA && (modstate&KMOD_SHIFT)!=0){
                        check_command(COMMAND_GO_UP_STAIRS);
                    }

                    //Pick up item.
                    else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.sym==SDLK_COMMA){
                        check_command(COMMAND_PICK_UP);
                    }

                    //Wait a turn.
                    else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.sym==SDLK_PERIOD){
                        check_command(COMMAND_WAIT);
                    }

                    //Search.
                    else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.sym==SDLK_s){
                        check_command(COMMAND_SEARCH);
                    }

                    //***********************//
                    // Directional Commands: //
                    //***********************//

                    //Cancel directional or inventory input.
                    else if(event.key.keysym.sym==SDLK_ESCAPE || event.key.keysym.sym==SDLK_SPACE){
                        if(input_directional!=DIRECTIONAL_COMMAND_NONE){
                            update_text_log(string_command_canceled.c_str(),is_player);

                            input_directional=DIRECTIONAL_COMMAND_NONE;
                        }
                        if(input_inventory!=INVENTORY_COMMAND_NONE){
                            update_text_log(string_command_canceled.c_str(),is_player);

                            input_inventory=INVENTORY_COMMAND_NONE;
                        }
                        display_inventory=false;
                    }

                    //Open door.
                    else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.sym==SDLK_o){
                        update_text_log(string_command_what_direction.c_str(),is_player);

                        input_directional=DIRECTIONAL_COMMAND_OPEN_DOOR;
                    }

                    //Close door.
                    else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.sym==SDLK_c){
                        update_text_log(string_command_what_direction.c_str(),is_player);

                        input_directional=DIRECTIONAL_COMMAND_CLOSE_DOOR;
                    }

                    //Move left.
                    else if(event.key.keysym.sym==SDLK_LEFT || event.key.keysym.sym==SDLK_KP4){
                        move_state=LEFT;

                        //If no directional command is waiting on a direction.
                        if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE){
                            input_directional=DIRECTIONAL_COMMAND_MOVE_LEFT;
                        }
                    }

                    //Move up.
                    else if(event.key.keysym.sym==SDLK_UP || event.key.keysym.sym==SDLK_KP8){
                        move_state=UP;

                        //If no directional command is waiting on a direction.
                        if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE){
                            input_directional=DIRECTIONAL_COMMAND_MOVE_UP;
                        }
                    }

                    //Move right.
                    else if(event.key.keysym.sym==SDLK_RIGHT || event.key.keysym.sym==SDLK_KP6){
                        move_state=RIGHT;

                        //If no directional command is waiting on a direction.
                        if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE){
                            input_directional=DIRECTIONAL_COMMAND_MOVE_RIGHT;
                        }
                    }

                    //Move down.
                    else if(event.key.keysym.sym==SDLK_DOWN || event.key.keysym.sym==SDLK_KP2){
                        move_state=DOWN;

                        //If no directional command is waiting on a direction.
                        if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE){
                            input_directional=DIRECTIONAL_COMMAND_MOVE_DOWN;
                        }
                    }

                    //Move left up.
                    else if(event.key.keysym.sym==SDLK_KP7){
                        move_state=LEFT_UP;

                        //If no directional command is waiting on a direction.
                        if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE){
                            input_directional=DIRECTIONAL_COMMAND_MOVE_LEFT_UP;
                        }
                    }

                    //Move right up.
                    else if(event.key.keysym.sym==SDLK_KP9){
                        move_state=RIGHT_UP;

                        //If no directional command is waiting on a direction.
                        if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE){
                            input_directional=DIRECTIONAL_COMMAND_MOVE_RIGHT_UP;
                        }
                    }

                    //Move right down.
                    else if(event.key.keysym.sym==SDLK_KP3){
                        move_state=RIGHT_DOWN;

                        //If no directional command is waiting on a direction.
                        if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE){
                            input_directional=DIRECTIONAL_COMMAND_MOVE_RIGHT_DOWN;
                        }
                    }

                    //Move left down.
                    else if(event.key.keysym.sym==SDLK_KP1){
                        move_state=LEFT_DOWN;

                        //If no directional command is waiting on a direction.
                        if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE){
                            input_directional=DIRECTIONAL_COMMAND_MOVE_LEFT_DOWN;
                        }
                    }

                    //*********************//
                    // Inventory Commands: //
                    //*********************//

                    //Open inventory.
                    else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.sym==SDLK_i){
                        display_inventory=!display_inventory;
                    }

                    //Drop item.
                    else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.sym==SDLK_d){
                        update_text_log(string_command_drop_what.c_str(),is_player);

                        input_inventory=INVENTORY_COMMAND_DROP_ITEM;
                    }

                    //Equip item.
                    else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.sym==SDLK_e){
                        update_text_log("What do you want to equip?",is_player);

                        input_inventory=INVENTORY_COMMAND_EQUIP_ITEM;
                    }

                    //Unequip item.
                    else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.sym==SDLK_u){
                        update_text_log("What do you want to unequip?",is_player);

                        input_inventory=INVENTORY_COMMAND_UNEQUIP_ITEM;
                    }

                    //Throw item.
                    else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.sym==SDLK_t){
                        update_text_log("What do you want to throw?",is_player);

                        input_inventory=INVENTORY_COMMAND_THROW_ITEM;
                    }

                    //Inventory letter.
                    else if(input_inventory!=INVENTORY_COMMAND_NONE && ((event.key.keysym.unicode>=(Uint16)'A' && event.key.keysym.unicode<=(Uint16)'Z') || (event.key.keysym.unicode>=(Uint16)'a' && event.key.keysym.unicode<=(Uint16)'z') || (event.key.keysym.unicode==(Uint16)'$'))){
                        inventory_input_state=(char)event.key.keysym.unicode;
                    }
                }
                break;
        }
    }

    //Toggle fullscreen.
    //This is located here in the code because it needs to come before the chat toggle,
    //which also uses SDLK_RETURN.
    if((keystates[SDLK_LALT] || keystates[SDLK_RALT]) && keystates[SDLK_RETURN]){
        main_window.toggle_fullscreen();

        if(keystates[SDLK_LALT]){
            keystates[SDLK_LALT]=NULL;
        }
        if(keystates[SDLK_RALT]){
            keystates[SDLK_RALT]=NULL;
        }
        keystates[SDLK_RETURN]=NULL;
    }

    if(!chat_mode){
        //If a directional command has been given.
        if(input_directional!=DIRECTIONAL_COMMAND_NONE && move_state!=NONE){
            check_command_directional(move_state);
        }

        //If a inventory command has been given.
        if(input_inventory!=INVENTORY_COMMAND_NONE && inventory_input_state!=0){
            check_command_inventory(inventory_input_state);
        }

        //**************//
        // Other Input: //
        //**************//

        //******************//
        // Camera controls: //
        //******************//

        //If numpad 0 is pressed, toggle the camera's stickiness and play the appropriate sound.
        if(keystates[SDLK_KP0]){
            if(cam_state==CAM_STICKY){
                cam_state=0;
            }
            else{
                cam_state=CAM_STICKY;
            }

            //Once the toggle camera stickiness key has been hit, the player must release it for it to function again.
            keystates[SDLK_KP0]=NULL;
        }

        //If the camera is unsticky, check for camera inputs.
        if(cam_state!=CAM_STICKY){
            //Handle camera directional keys being pressed.
            if(keystates[SDLK_DELETE]){
                cam_state=LEFT;
            }
            if(keystates[SDLK_HOME]){
                cam_state=UP;
            }
            if(keystates[SDLK_PAGEDOWN]){
                cam_state=RIGHT;
            }
            if(keystates[SDLK_END]){
                cam_state=DOWN;
            }

            //Handle multiple camera directional keys being pressed at once.
            if(keystates[SDLK_DELETE] && keystates[SDLK_HOME]){
                cam_state=LEFT_UP;
            }
            if(keystates[SDLK_PAGEDOWN] && keystates[SDLK_HOME]){
                cam_state=RIGHT_UP;
            }
            if(keystates[SDLK_PAGEDOWN] && keystates[SDLK_END]){
                cam_state=RIGHT_DOWN;
            }
            if(keystates[SDLK_DELETE] && keystates[SDLK_END]){
                cam_state=LEFT_DOWN;
            }

            //If no camera directional keys are pressed, stop the camera.
            if(!keystates[SDLK_DELETE] && !keystates[SDLK_HOME] && !keystates[SDLK_PAGEDOWN] && !keystates[SDLK_END]){
                cam_state=0;
            }
        }

        //If the player hits the command key.
        if(keystates[SDLK_RETURN] || keystates[SDLK_KP_ENTER]){
            chat_mode=true;

            //
            keystates[SDLK_RETURN]=NULL;
            keystates[SDLK_KP_ENTER]=NULL;
        }

        ///******************///
        /// Debug/Test Code. ///
        ///******************///

        if(option_dev){
            //Die
            /**if(keystates[SDLK_d]){
                die();

                keystates[SDLK_d]=NULL;
            }*/

            //Command: DESTROY WALL (TEST)
            /**if(keystates[SDLK_t]){
                execute_command(-1);

                keystates[SDLK_t]=NULL;
            }*/

            //Command: WARP TO DOWN STAIR (TEST)
            /**if(keystates[SDLK_w]){
                execute_command(-2);

                keystates[SDLK_w]=NULL;
            }*/

            //Command: Toggle all light items in current dungeon level. (TEST)
            /**if(keystates[SDLK_f]){
                for(short i=0;i<vector_levels[current_level].vector_item_lights.size();i++){
                    //If the light item is on.
                    vector_levels[current_level].vector_item_lights[i].on=!vector_levels[current_level].vector_item_lights[i].on;
                }

                keystates[SDLK_f]=NULL;
            }*/

            if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON(SDL_BUTTON_LEFT)){
                int mouse_current_x=(int)((int)(mouse_x+camera_x)/TILE_SIZE);
                int mouse_current_y=(int)((int)(mouse_y+camera_y)/TILE_SIZE);
                x=mouse_current_x;
                y=mouse_current_y;
            }
        }

        ///*************************///
        /// End of Debug/Test Code. ///
        ///*************************///
    }

    //If the player hits the screenshot key, take a screenshot.
    if(keystates[SDLK_F5]){
        main_window.screenshot();

        //Once the screenshot key has been hit, the player must release it for it to function again.
        keystates[SDLK_F5]=NULL;
    }

    //Quit the game.
    if((keystates[SDLK_LALT] || keystates[SDLK_RALT]) && keystates[SDLK_F4]){
        quit_game();

        if(keystates[SDLK_LALT]){
            keystates[SDLK_LALT]=NULL;
        }
        if(keystates[SDLK_RALT]){
            keystates[SDLK_RALT]=NULL;
        }
        keystates[SDLK_F4]=NULL;
    }
}

void Player::move(){
    if(alive){
        if(input_directional!=DIRECTIONAL_COMMAND_NONE && move_state!=NONE){
            execute_command_directional(move_state);
        }

        else if(input_inventory!=INVENTORY_COMMAND_NONE && inventory_input_state!=0){
            execute_command_inventory(inventory_input_state);
        }

        else if(command_standard!=COMMAND_NONE){
            execute_command(command_standard);
        }

        //Movement has been handled, now clear the move state.
        move_state=NONE;

        //Remember the inventory_input_state in case it is needed for a directional inventory command.
        directional_inventory_input_state=inventory_input_state;

        inventory_input_state=0;

        command_standard=COMMAND_NONE;

        process_turn();
    }
}

void Player::update_fov(){
    //Clear the level's fog.
    for(short int_y=0;int_y<vector_levels[current_level].level_y;int_y++){
        for(short int_x=0;int_x<vector_levels[current_level].level_x;int_x++){
            //If the light is permanent.
            if(vector_levels[current_level].fog[int_x][int_y]==FOG_REVEALED_PERMANENT){
                vector_levels[current_level].fog[int_x][int_y]=FOG_FOG;
            }
            //If the light is temporary.
            else if(vector_levels[current_level].fog[int_x][int_y]==FOG_REVEALED_TEMPORARY){
                vector_levels[current_level].fog[int_x][int_y]=FOG_BLACK;
            }
        }
    }

    //Process any light items on this dungeon level.
    for(int i=0;i<vector_levels[current_level].items.size();i++){
        //If the item has a light radius.
        if(vector_levels[current_level].items[i].fov_radius!=LIGHT_NONE){
            //If the light item is on.
            if(vector_levels[current_level].items[i].light_on){
                //If the light source is a beam.
                if(vector_levels[current_level].items[i].beam){
                    fov_beam(&fov_settings,&vector_levels[current_level],&vector_levels[current_level].items[i].source_data,vector_levels[current_level].items[i].x,vector_levels[current_level].items[i].y,vector_levels[current_level].items[i].fov_radius,vector_levels[current_level].items[i].facing,vector_levels[current_level].items[i].fov_angle);
                }
                //If the light source is a circle.
                else{
                    fov_circle(&fov_settings,&vector_levels[current_level],&vector_levels[current_level].items[i].source_data,vector_levels[current_level].items[i].x,vector_levels[current_level].items[i].y,vector_levels[current_level].items[i].fov_radius);
                }

                //Light the item's own space.
                if(vector_levels[current_level].fog[vector_levels[current_level].items[i].x][vector_levels[current_level].items[i].y]==FOG_FOG || vector_levels[current_level].fog[vector_levels[current_level].items[i].x][vector_levels[current_level].items[i].y]==FOG_REVEALED_PERMANENT){
                    vector_levels[current_level].fog[vector_levels[current_level].items[i].x][vector_levels[current_level].items[i].y]=FOG_REVEALED_PERMANENT;
                }
                else{
                    vector_levels[current_level].fog[vector_levels[current_level].items[i].x][vector_levels[current_level].items[i].y]=FOG_REVEALED_TEMPORARY;
                }
            }
        }
    }

    //Process the player's light.

    //If the player's light source is on.
    if(light_on){
        //If the light source is a beam.
        if(beam){
            fov_beam(&fov_settings,&vector_levels[current_level],&source_data,player.x,player.y,fov_radius,facing,fov_angle);
        }
        //If the light source is a circle.
        else{
            fov_circle(&fov_settings,&vector_levels[current_level],&source_data,player.x,player.y,fov_radius);
        }

        //Light the player's own space.
        fov_circle(&fov_settings,&vector_levels[current_level],&source_data,player.x,player.y,0);
    }
    //If the player's light source is off.
    else{
        fov_circle(&fov_settings,&vector_levels[current_level],&(light_data)LIGHT_SOURCE_OFF_DATA,player.x,player.y,LIGHT_SOURCE_OFF_RADIUS);

        //Light the player's own space.
        fov_circle(&fov_settings,&vector_levels[current_level],&(light_data)LIGHT_SOURCE_OFF_DATA,player.x,player.y,0);
    }
}

void Player::set_camera(){
    //If the camera is sticky, center the camera on the player.
    if(cam_state==CAM_STICKY){
        //Now center the camera on the player.
        camera_x=(return_absolute_x()+TILE_SIZE/2)-(camera_w/2);
        camera_y=(return_absolute_y()+TILE_SIZE/2)-(camera_h/2);
    }

    //Move the camera if it is unsticky:
    if(cam_state!=CAM_STICKY){
        //Now check for camera input and move the camera accordingly.
        if(cam_state==LEFT){
            camera_x-=TILE_SIZE;
        }
        if(cam_state==UP){
            camera_y-=TILE_SIZE;
        }
        if(cam_state==RIGHT){
            camera_x+=TILE_SIZE;
        }
        if(cam_state==DOWN){
            camera_y+=TILE_SIZE;
        }
        if(cam_state==LEFT_UP){
            camera_x-=TILE_SIZE;
            camera_y-=TILE_SIZE;
        }
        if(cam_state==RIGHT_UP){
            camera_x+=TILE_SIZE;
            camera_y-=TILE_SIZE;
        }
        if(cam_state==RIGHT_DOWN){
            camera_x+=TILE_SIZE;
            camera_y+=TILE_SIZE;
        }
        if(cam_state==LEFT_DOWN){
            camera_x-=TILE_SIZE;
            camera_y+=TILE_SIZE;
        }
    }

    //If the camera goes past the bounds of the level map, set it back in the bounds.
    if(camera_x<0){
        camera_x=0;
    }
    if(camera_x+camera_w>vector_levels[current_level].return_absolute_x()){
        camera_x=vector_levels[current_level].return_absolute_x()-camera_w;
    }
    if(camera_y<0){
        camera_y=0;
    }
    if(camera_y+camera_h>vector_levels[current_level].return_absolute_y()){
        camera_y=vector_levels[current_level].return_absolute_y()-camera_h;
    }
}

void Player::update_window_caption(int frame_rate,double ms_per_frame){
    string temp="";

    time_t now;
    struct tm *tm_now;
    char buff[BUFSIZ];
    now=time(NULL);
    tm_now=localtime(&now);
    strftime(buff,sizeof buff,"   %A, %B %d, %Y - %H:%M:%S",tm_now);

    //Set the window caption.
    if(option_dev){
        msg="Roguelike (DEV Mode) - ";
    }
    else{
        msg="Roguelike - ";
    }

    msg+=player.name;

    msg+=buff;

    if(option_fps){
        msg+="   FPS: ";
        ss.clear();ss.str("");ss<<frame_rate;temp=ss.str();
        msg+=temp;

        msg+=" / MS Per Frame: ";
        temp="";
        ss.clear();ss.str("");ss<<ms_per_frame;temp=ss.str();
        msg+=temp;
    }

    SDL_WM_SetCaption(msg.c_str(),msg.c_str());
}

void Player::render(vector< vector<bool> >* tile_rendered){
    if(alive){
        //If nothing has been rendered here yet.
        if(!tile_rendered->at(x)[y]){
            //Render the player if the player is in the camera bounds:
            if(return_absolute_x()>=player.camera_x-TILE_SIZE && return_absolute_x()<=player.camera_x+player.camera_w && return_absolute_y()>=player.camera_y-TILE_SIZE && return_absolute_y()<=player.camera_y+player.camera_h){
                font.show((int)(return_absolute_x()-player.camera_x),(int)(return_absolute_y()-player.camera_y),appearance,color);

                tile_rendered->at(x)[y]=true;
            }
        }
    }
}
