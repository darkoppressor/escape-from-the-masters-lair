/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "player.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "quit.h"
#include "save_load.h"
#include "message_log.h"
#include "starting_values.h"
#include "covering_conversions.h"

using namespace std;

Player::Player(){
    // From Object: //

    name="\x1F";

    // From Creature: //

    is_player=true;

    source_data.permanent=true;

    race=-1;

    // From Player: //

    keystates=NULL;

    //The camera starts out sticky.
    cam_state=CAM_STICKY;

    current_window=WINDOW_NONE;

    turn=0;

    game_in_progress=false;

    get_name="";

    get_race="";

    done_focusing_skills=false;

    done_buying_start_items=false;

    starting_items_gold=STARTING_ITEMS_GOLD;

    available_starting_items.push_back("simple wooden dagger");

    available_starting_items.push_back("simple wooden long sword");

    available_starting_items.push_back("simple wooden axe");

    available_starting_items.push_back("simple wooden mace");

    available_starting_items.push_back("simple wooden quarterstaff");

    available_starting_items.push_back("simple wooden spear");

    available_starting_items.push_back("simple wooden bow");

    available_starting_items.push_back("simple wooden crossbow");

    available_starting_items.push_back("simple sling");

    available_starting_items.push_back("simple wooden arrow");

    available_starting_items.push_back("simple wooden crossbow bolt");

    available_starting_items.push_back("simple stone");

    available_starting_items.push_back("simple bone shuriken");

    available_starting_items.push_back("simple leather helm");

    available_starting_items.push_back("pair of simple leather spaulders");

    available_starting_items.push_back("simple leather chestpiece");

    available_starting_items.push_back("simple leather cloak");

    available_starting_items.push_back("simple leather belt");

    available_starting_items.push_back("pair of simple leather trousers");

    available_starting_items.push_back("pair of simple leather boots");

    available_starting_items.push_back("pair of simple leather gloves");

    available_starting_items.push_back("pair of simple leather bracers");

    available_starting_items.push_back("simple leather shield");

    available_starting_items.push_back("bottle of water");

    available_starting_items.push_back("bottle of oil");

    available_starting_items.push_back("potion of slight healing");

    death_message="";

    deaths=0;

    start_date="";
    start_time="";
    end_date="";
    end_time="";

    score_from_game=0;

    score=0;

    won=false;

    runestones=0;

    //Chat stuff:

    chat_mode=false;

    for(short i=0;i<1000;i++){
        text_log.push_back("\x1F");
    }

    timer_cursor.start();
    cursor=false;
    cursor_opacity=10;
    text_log_display_position=text_log.size()-1;

    //Options:
    option_fullscreen=false;
    option_screen_width=800;
    option_screen_height=600;
    option_dev=false;
    option_fps=true;
    option_healthbars=false;

    //Set the camera's initial location and its dimensions.
    camera_x=0.0;
    camera_y=0.0;
    camera_w=option_screen_width;
    camera_h=option_screen_height-180;
}

void Player::set_inventory(){
    give_item("brass lantern");

    give_item("bottle of water");

    inventory[0].light_on=true;
    inventory[0].fuel=inventory[0].fuel_max;

    //Give the player their purchased starting items.
    for(int i=0;i<starting_items.size();i++){
        int stack=1;

        if(available_starting_items[starting_items[i]]=="simple wooden arrow"){
            stack=STARTING_THROWN_STACK_SIZE;
        }
        else if(available_starting_items[starting_items[i]]=="simple wooden crossbow bolt"){
            stack=STARTING_THROWN_STACK_SIZE;
        }
        else if(available_starting_items[starting_items[i]]=="simple stone"){
            stack=STARTING_THROWN_STACK_SIZE;
        }
        else if(available_starting_items[starting_items[i]]=="simple bone shuriken"){
            stack=STARTING_THROWN_STACK_SIZE;
        }

        give_item(available_starting_items[starting_items[i]],stack);

        //If the new item is armor.
        if(inventory[inventory.size()-1].category==ITEM_ARMOR){
            if(equipment_slot_empty(inventory.size()-1,-1)){
                equip_item(inventory.size()-1,-1);
            }
        }
        //If the new item is a weapon.
        else if(inventory[inventory.size()-1].category==ITEM_WEAPON){
            short slot=-1;

            //If the item is an ammo item.
            if(inventory[inventory.size()-1].launcher!=WEAPON_THROWN){
                if(equipment_slot_empty(inventory.size()-1,EQUIP_QUIVER)){
                    slot=EQUIP_QUIVER;
                }
            }
            //If the item is a melee weapon or a ranged weapon.
            else if((inventory[inventory.size()-1].weapon_category>=WEAPON_SHORT_BLADES && inventory[inventory.size()-1].weapon_category<=WEAPON_STAVES) || (inventory[inventory.size()-1].weapon_category>=WEAPON_BOWS && inventory[inventory.size()-1].weapon_category<=WEAPON_SLINGS)){
                if(equipment_slot_empty(inventory.size()-1,EQUIP_HOLD_RIGHT)){
                    slot=EQUIP_HOLD_RIGHT;
                }
                else if(equipment_slot_empty(inventory.size()-1,EQUIP_HOLD_LEFT)){
                    slot=EQUIP_HOLD_LEFT;
                }
            }

            if(slot!=-1){
                equip_item(inventory.size()-1,slot);
            }
        }
    }
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

    movement_speed=templates.base_stats.movement_speed;
    next_move=movement_speed;

    carry_capacity=templates.base_stats.carry_capacity;

    for(int i=SKILL_BLADED_WEAPONS;i<SKILL_MAGIC_SUMMONING+1;i++){
        skills[i][SKILL_EXPERIENCE_MAX]+=templates.template_races[race].skills[i][SKILL_EXPERIENCE_MAX];
    }
}

void Player::handle_input(){
    int mouse_x,mouse_y;

    //Get the SDL keystates and store them in the keystates variable for evaluation.
    keystates=SDL_GetKeyState(NULL);

    SDL_GetMouseState(&mouse_x,&mouse_y);

    //If an item's info has been requested.
    if(item_info!=-1){
        current_window=WINDOW_ITEM_INFO;
    }

    while(SDL_PollEvent(&event)){
        SDL_EnableUNICODE(SDL_ENABLE);

        if(chat_mode){
            string_input_chat.handle_events();
        }

        switch(event.type){
            case SDL_QUIT:
                quit_game();
                break;

            case SDL_MOUSEBUTTONDOWN:
                //If the cursor is over the chat box.
                if(mouse_x>=0 && mouse_x<=option_screen_width && mouse_y>=option_screen_height-122 && mouse_y<=option_screen_height){
                    if(event.button.button==SDL_BUTTON_WHEELUP){
                        if(text_log_display_position<text_log.size()-1){
                            text_log_display_position++;
                        }
                    }
                    else if(event.button.button==SDL_BUTTON_WHEELDOWN){
                        if(text_log_display_position>5 && text_log[text_log_display_position-1]!="\x1F"){
                            text_log_display_position--;
                        }
                    }
                }
                break;

            case SDL_KEYDOWN:
                if(!chat_mode){
                    if(current_window==WINDOW_INVENTORY){
                        handle_input_inventory();
                    }
                    else if(current_window==WINDOW_STATS){
                        handle_input_stats();
                    }
                    else if(current_window==WINDOW_LEVELUP){
                        handle_input_levelup();
                    }
                    else if(current_window==WINDOW_DEATH){
                        handle_input_death();
                    }
                    else if(current_window==WINDOW_DEATH_1){
                        handle_input_death_1();
                    }
                    else if(current_window==WINDOW_CONFIRM_LEAVE_DUNGEON){
                        handle_input_confirm_leave_dungeon();
                    }
                    else if(current_window==WINDOW_CONFIRM_QUAFF_FROM_FOUNTAIN){
                        handle_input_confirm_quaff_from_fountain();
                    }
                    else if(current_window==WINDOW_LEAVE_DUNGEON){
                        handle_input_leave_dungeon();
                    }
                    else if(current_window==WINDOW_INTERACTIVE_INVENTORY_RELEVANT){
                        handle_input_interactive_inventory_relevant();
                    }
                    else if(current_window==WINDOW_INTERACTIVE_INVENTORY_ALL){
                        handle_input_interactive_inventory_all();
                    }
                    else if(current_window==WINDOW_ITEM_INFO){
                        handle_input_item_info();
                    }
                    //If no windows are open.
                    else{

                        //********************//
                        // Standard Commands: //
                        //********************//

                        //Go down stairs.
                        if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'>'){
                            check_command(COMMAND_GO_DOWN_STAIRS);
                        }

                        //Go up stairs.
                        else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'<'){
                            check_command(COMMAND_GO_UP_STAIRS);
                        }

                        //Pick up item.
                        else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.unicode==(Uint16)','){
                            check_command(COMMAND_PICK_UP);
                        }

                        //Wait a turn.
                        else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'.'){
                            check_command(COMMAND_WAIT);
                        }

                        //Search.
                        else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'s'){
                            check_command(COMMAND_SEARCH);
                        }

                        //***********************//
                        // Directional Commands: //
                        //***********************//

                        //Cancel directional or inventory input.
                        else if(event.key.keysym.sym==SDLK_ESCAPE || event.key.keysym.sym==SDLK_SPACE){
                            if(input_directional!=DIRECTIONAL_COMMAND_NONE){
                                update_text_log("Nevermind.",is_player);

                                input_directional=DIRECTIONAL_COMMAND_NONE;
                            }
                            if(input_inventory!=INVENTORY_COMMAND_NONE){
                                update_text_log("Nevermind.",is_player);

                                input_inventory=INVENTORY_COMMAND_NONE;
                            }
                        }

                        //Open door.
                        else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'o'){
                            update_text_log("In what direction?",is_player);

                            input_directional=DIRECTIONAL_COMMAND_OPEN_DOOR;
                        }

                        //Close door.
                        else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'c'){
                            update_text_log("In what direction?",is_player);

                            input_directional=DIRECTIONAL_COMMAND_CLOSE_DOOR;
                        }

                        //Fire item.
                        else if(input_directional==DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'f'){
                            update_text_log("In what direction?",is_player);

                            input_directional=DIRECTIONAL_COMMAND_FIRE_ITEM;
                        }

                        //Move left.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && (event.key.keysym.sym==SDLK_LEFT || event.key.keysym.sym==SDLK_KP4 || event.key.keysym.unicode==(Uint16)'h')){
                            move_state=LEFT;

                            //If no directional command is waiting on a direction.
                            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                                input_directional=DIRECTIONAL_COMMAND_MOVE_LEFT;
                            }
                        }

                        //Move up.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && (event.key.keysym.sym==SDLK_UP || event.key.keysym.sym==SDLK_KP8 || event.key.keysym.unicode==(Uint16)'k')){
                            move_state=UP;

                            //If no directional command is waiting on a direction.
                            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                                input_directional=DIRECTIONAL_COMMAND_MOVE_UP;
                            }
                        }

                        //Move right.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && (event.key.keysym.sym==SDLK_RIGHT || event.key.keysym.sym==SDLK_KP6 || event.key.keysym.unicode==(Uint16)'l')){
                            move_state=RIGHT;

                            //If no directional command is waiting on a direction.
                            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                                input_directional=DIRECTIONAL_COMMAND_MOVE_RIGHT;
                            }
                        }

                        //Move down.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && (event.key.keysym.sym==SDLK_DOWN || event.key.keysym.sym==SDLK_KP2 || event.key.keysym.unicode==(Uint16)'j')){
                            move_state=DOWN;

                            //If no directional command is waiting on a direction.
                            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                                input_directional=DIRECTIONAL_COMMAND_MOVE_DOWN;
                            }
                        }

                        //Move left up.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && (event.key.keysym.sym==SDLK_KP7 || event.key.keysym.unicode==(Uint16)'y')){
                            move_state=LEFT_UP;

                            //If no directional command is waiting on a direction.
                            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                                input_directional=DIRECTIONAL_COMMAND_MOVE_LEFT_UP;
                            }
                        }

                        //Move right up.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && (event.key.keysym.sym==SDLK_KP9 || event.key.keysym.unicode==(Uint16)'u')){
                            move_state=RIGHT_UP;

                            //If no directional command is waiting on a direction.
                            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                                input_directional=DIRECTIONAL_COMMAND_MOVE_RIGHT_UP;
                            }
                        }

                        //Move right down.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && (event.key.keysym.sym==SDLK_KP3 || event.key.keysym.unicode==(Uint16)'n')){
                            move_state=RIGHT_DOWN;

                            //If no directional command is waiting on a direction.
                            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                                input_directional=DIRECTIONAL_COMMAND_MOVE_RIGHT_DOWN;
                            }
                        }

                        //Move left down.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && (event.key.keysym.sym==SDLK_KP1 || event.key.keysym.unicode==(Uint16)'b')){
                            move_state=LEFT_DOWN;

                            //If no directional command is waiting on a direction.
                            if(input_directional==DIRECTIONAL_COMMAND_NONE){
                                input_directional=DIRECTIONAL_COMMAND_MOVE_LEFT_DOWN;
                            }
                        }

                        //Unknown direction.
                        else if(input_directional!=DIRECTIONAL_COMMAND_NONE && input_inventory==INVENTORY_COMMAND_NONE){
                            update_text_log("That isn't a direction!",is_player);

                            input_directional=DIRECTIONAL_COMMAND_NONE;
                        }

                        //*********************//
                        // Inventory Commands: //
                        //*********************//

                        //Drop item.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'d'){
                            update_text_log("Drop what?",is_player);

                            input_inventory=INVENTORY_COMMAND_DROP_ITEM;
                        }

                        //Quaff item.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'q'){
                            if(vector_levels[current_level].tiles[x][y].type==TILE_TYPE_FOUNTAIN){
                                update_text_log("There is a fountain here. Drink from it? [yn]",is_player);

                                player.current_window=WINDOW_CONFIRM_QUAFF_FROM_FOUNTAIN;
                            }
                            else{
                                update_text_log("What do you want to drink? [?*]",is_player);

                                input_inventory=INVENTORY_COMMAND_QUAFF_ITEM;
                            }
                        }

                        //Equip item in right hand.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'1'){
                            update_text_log("What do you want to wield in your right hand? [?*]",is_player);

                            input_inventory=INVENTORY_COMMAND_EQUIP_RIGHT_HAND;
                        }

                        //Equip item in left hand.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'2'){
                            update_text_log("What do you want to wield in your left hand? [?*]",is_player);

                            input_inventory=INVENTORY_COMMAND_EQUIP_LEFT_HAND;
                        }

                        //Quiver item.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'Q'){
                            update_text_log("What do you want to quiver? [?*]",is_player);

                            input_inventory=INVENTORY_COMMAND_QUIVER_ITEM;
                        }

                        //Equip armor.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'w'){
                            update_text_log("What do you want to wear? [?*]",is_player);

                            input_inventory=INVENTORY_COMMAND_EQUIP_ARMOR;
                        }

                        //Unequip item.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'r'){
                            update_text_log("What do you want to remove? [?*]",is_player);

                            input_inventory=INVENTORY_COMMAND_UNEQUIP_ITEM;
                        }

                        //Throw item.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'t'){
                            update_text_log("What do you want to throw? [?*]",is_player);

                            input_inventory=INVENTORY_COMMAND_THROW_ITEM;
                        }

                        //Use item.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'a'){
                            update_text_log("What do you want to use? [?*]",is_player);

                            input_inventory=INVENTORY_COMMAND_USE_ITEM;
                        }

                        //Mix items, first item.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'m'){
                            update_text_log("What do you want to mix? [?*]",is_player);

                            input_inventory=INVENTORY_COMMAND_MIX_ITEMS_1;
                        }

                        //Read item.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'R'){
                            update_text_log("What do you want to read? [?*]",is_player);

                            input_inventory=INVENTORY_COMMAND_READ_ITEM;
                        }

                        //Item info.
                        else if(input_inventory==INVENTORY_COMMAND_NONE && input_directional==DIRECTIONAL_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'e'){
                            update_text_log("What do you want to examine? [?*]",is_player);

                            input_inventory=INVENTORY_COMMAND_ITEM_INFO;
                        }

                        //Inventory letter.
                        else if(input_inventory!=INVENTORY_COMMAND_NONE && ((event.key.keysym.unicode>=(Uint16)'A' && event.key.keysym.unicode<=(Uint16)'Z') || (event.key.keysym.unicode>=(Uint16)'a' && event.key.keysym.unicode<=(Uint16)'z'))){
                            inventory_input_state=(char)event.key.keysym.unicode;
                        }

                        //Show interactive inventory with all items.
                        else if(input_inventory!=INVENTORY_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'*'){
                            current_window=WINDOW_INTERACTIVE_INVENTORY_ALL;
                        }

                        //Show interactive inventory with only relevant items.
                        else if(input_inventory!=INVENTORY_COMMAND_NONE && event.key.keysym.unicode==(Uint16)'?'){
                            current_window=WINDOW_INTERACTIVE_INVENTORY_RELEVANT;

                            inventory_categories_to_render.clear();

                            switch(input_inventory){
                            default:
                                for(short i=ITEM_WEAPON;i<ITEM_OTHER+1;i++){
                                    inventory_categories_to_render.push_back(i);
                                }
                                break;
                            case INVENTORY_COMMAND_EQUIP_RIGHT_HAND:case INVENTORY_COMMAND_EQUIP_LEFT_HAND:case INVENTORY_COMMAND_QUIVER_ITEM:
                                inventory_categories_to_render.push_back(ITEM_WEAPON);
                                break;
                            case INVENTORY_COMMAND_EQUIP_ARMOR:
                                inventory_categories_to_render.push_back(ITEM_ARMOR);
                                break;
                            case INVENTORY_COMMAND_UNEQUIP_ITEM:
                                for(short i=ITEM_WEAPON;i<ITEM_ARMOR+1;i++){
                                    inventory_categories_to_render.push_back(i);
                                }
                                break;
                            case INVENTORY_COMMAND_THROW_ITEM:
                                inventory_categories_to_render.push_back(ITEM_WEAPON);
                                break;
                            case INVENTORY_COMMAND_QUAFF_ITEM:
                                inventory_categories_to_render.push_back(ITEM_DRINK);
                                break;
                            case INVENTORY_COMMAND_USE_ITEM:
                                inventory_categories_to_render.push_back(ITEM_OTHER);
                                break;
                            case INVENTORY_COMMAND_MIX_ITEMS_1:case INVENTORY_COMMAND_MIX_ITEMS_2:
                                inventory_categories_to_render.push_back(ITEM_DRINK);
                                inventory_categories_to_render.push_back(ITEM_OTHER);
                                break;
                            }
                        }

                        //**************//
                        // Other Input: //
                        //**************//

                        //Open inventory window.
                        else if(event.key.keysym.sym==SDLK_i){
                            current_window=WINDOW_INVENTORY;
                        }

                        //Open stats window.
                        else if(event.key.keysym.sym==SDLK_F1){
                            current_window=WINDOW_STATS;
                        }
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

    if(!chat_mode && current_window==WINDOW_NONE){
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

            keystates[SDLK_RETURN]=NULL;
            keystates[SDLK_KP_ENTER]=NULL;
        }

        ///******************///
        /// Debug/Test Code. ///
        ///******************///

        if(option_dev){
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

            if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON(SDL_BUTTON_LEFT)){
                int mouse_current_x=(int)((int)(mouse_x+camera_x)/TILE_SIZE_X);
                int mouse_current_y=(int)((int)(mouse_y+camera_y)/TILE_SIZE_Y);
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
        update_fov();

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

        //Remember the inventory_input_state in case it is needed for a two part inventory command.
        two_part_inventory_input_state=inventory_input_state;

        if(input_inventory!=INVENTORY_COMMAND_UNEQUIPPED_NOW_DROP_ITEM){
            inventory_input_state=0;
        }

        command_standard=COMMAND_NONE;

        process_move();

        check_this_tile();

        update_fov();
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

    //Process the player's light(s).

    //Give off the "no lights" light.
    fov_circle(&fov_settings,&vector_levels[current_level],&(light_data)LIGHT_SOURCE_OFF_DATA,player.x,player.y,LIGHT_SOURCE_OFF_RADIUS);
    //Light the player's own space.
    fov_circle(&fov_settings,&vector_levels[current_level],&(light_data)LIGHT_SOURCE_OFF_DATA,player.x,player.y,0);

    bool any_lights=false;

    //Look through all of the player's items.
    for(int i=0;i<inventory.size();i++){
        //If the item has a light radius.
        if(inventory[i].fov_radius!=LIGHT_NONE){
            //If the light item is on.
            if(inventory[i].light_on){
                any_lights=true;

                //If the light source is a beam.
                if(inventory[i].beam){
                    fov_beam(&fov_settings,&vector_levels[current_level],&source_data,player.x,player.y,inventory[i].fov_radius,facing,inventory[i].fov_angle);
                }
                //If the light source is a circle.
                else{
                    fov_circle(&fov_settings,&vector_levels[current_level],&source_data,player.x,player.y,inventory[i].fov_radius);
                }
            }
        }
    }

    //If the player had any light items on.
    if(any_lights){
        //Light the player's own space.
        fov_circle(&fov_settings,&vector_levels[current_level],&source_data,player.x,player.y,0);
    }
}

void Player::set_camera(){
    //If the camera is sticky, center the camera on the player.
    if(cam_state==CAM_STICKY){
        //Now center the camera on the player.
        camera_x=(return_absolute_x()+TILE_SIZE_X/2)-(camera_w/2);
        camera_y=(return_absolute_y()+TILE_SIZE_Y/2)-(camera_h/2);
    }

    //Move the camera if it is unsticky:
    if(cam_state!=CAM_STICKY){
        //Now check for camera input and move the camera accordingly.
        if(cam_state==LEFT){
            camera_x-=TILE_SIZE_X;
        }
        if(cam_state==UP){
            camera_y-=TILE_SIZE_Y;
        }
        if(cam_state==RIGHT){
            camera_x+=TILE_SIZE_X;
        }
        if(cam_state==DOWN){
            camera_y+=TILE_SIZE_Y;
        }
        if(cam_state==LEFT_UP){
            camera_x-=TILE_SIZE_X;
            camera_y-=TILE_SIZE_Y;
        }
        if(cam_state==RIGHT_UP){
            camera_x+=TILE_SIZE_X;
            camera_y-=TILE_SIZE_Y;
        }
        if(cam_state==RIGHT_DOWN){
            camera_x+=TILE_SIZE_X;
            camera_y+=TILE_SIZE_Y;
        }
        if(cam_state==LEFT_DOWN){
            camera_x-=TILE_SIZE_X;
            camera_y+=TILE_SIZE_Y;
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
        msg="Escape from the Master's Lair (DEV Mode)";
    }
    else{
        msg="Escape from the Master's Lair";
    }

    //If the player has a name.
    if(player.name!="\x1F"){
        msg+=" - ";
        msg+=player.name;
    }

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
            if(return_absolute_x()>=camera_x-TILE_SIZE_X && return_absolute_x()<=camera_x+camera_w && return_absolute_y()>=camera_y-TILE_SIZE_Y && return_absolute_y()<=camera_y+camera_h){
                short render_color=color;

                short temp_color=coverings_to_color(this);
                if(temp_color!=COLOR_RAINBOW){
                    render_color=temp_color;
                }

                font.show((int)(return_absolute_x()-camera_x),(int)(return_absolute_y()-camera_y),appearance,render_color);

                if(player.option_healthbars){
                    short health_bar_color=COLOR_GREEN;
                    if(return_health()>=return_health_max()*0.75){
                        health_bar_color=COLOR_GREEN;
                    }
                    else if(return_health()>=return_health_max()*0.50 && return_health()<return_health_max()*0.75){
                        health_bar_color=COLOR_YELLOW;
                    }
                    else if(return_health()>=return_health_max()*0.25 && return_health()<return_health_max()*0.50){
                        health_bar_color=COLOR_ORANGE;
                    }
                    else{
                        health_bar_color=COLOR_RED;
                    }
                    double health_bar_width=((double)((double)health/(double)health_max)*100)/6.25;
                    render_rectangle((int)(return_absolute_x()-camera_x),(int)(return_absolute_y()-camera_y),health_bar_width,5,0.75,health_bar_color);
                }

                tile_rendered->at(x)[y]=true;
            }
        }
    }
}
