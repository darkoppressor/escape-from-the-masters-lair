/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef player_h
#define player_h

#include <string>
#include <vector>

#include <SDL.h>

#include "timer.h"
#include "inventory_data.h"
#include "creature.h"

class Player: public Creature{
    private:

    //We will use this to hold the keystates, which will be given to us by SDL.
    Uint8 *keystates;

    //Current movement state of the camera.
    short cam_state;

    public:

    //**********************************************//
    // Variables that should be saved in savegames. //
    //**********************************************//

    //The starting date for this game.
    std::string start_date;

    //The starting time for this game.
    std::string start_time;

    //The score accumulated during this game.
    unsigned int score_from_game;

    int turn;

    //The number of deaths in this game.
    int deaths;

    //**************************************************//
    // Variables that should not be saved in savegames. //
    //**************************************************//

    //The score for this game.
    unsigned int score;

    //Did the player win the game?
    bool won;

    //The number of runestones the player left the dungeon with.
    int runestones;

    //If true, a game is currently in progress.
    //If false, a game is not currently in progress.
    bool game_in_progress;

    //String used for retrieving the player's name.
    std::string get_name;

    //String used for retrieving the player's race.
    std::string get_race;

    //If true, the player has finished selecting focused skills.
    //If false, the player has not finished selecting focused skills.
    bool done_focusing_skills;

    //If true, the player has finished selecting starting items.
    //If false, the player has not finished selecting starting items.
    bool done_buying_start_items;

    //Keeps track of the starting items the player has selected.
    std::vector<int> starting_items;

    //The money available for purchasing starting items.
    int starting_items_gold;

    //A list of starting items available for purchase.
    std::vector<std::string> available_starting_items;

    //Keeps track of what item categories are to be rendered on the inventory window.
    std::vector<short> inventory_categories_to_render;

    //Used to display the death message in the death window.
    std::string death_message;

    //The currently displayed window.
    short current_window;

    //The ending date for this game.
    std::string end_date;

    //The ending time for this game.
    std::string end_time;

    bool chat_mode;
    std::vector<std::string> text_log;
    Timer timer_cursor;
    bool cursor;
    int cursor_opacity;
    int text_log_display_position;

    bool option_fullscreen;
    bool option_dev;
    bool option_fps;
    bool option_healthbars;

    //The coordinates and dimensions of the camera.
    double camera_x,camera_y;
    double camera_w,camera_h;

    Player();

    //Setup the player's starting inventory.
    void set_inventory();

    //Set the base stats of the player.
    void set_base_stats();

    //Get input.
    void handle_input();

    //Move the player.
    void move();

    void update_fov();

    //Handle camera movement.
    void set_camera();

    void update_window_caption(int frame_rate,double ms_per_frame);

    //Handle the windows' input.
    void handle_input_no_game();
    void handle_input_get_name();
    void handle_input_get_race();
    void handle_input_get_focused_skills();
    void handle_input_get_starting_items();
    void handle_input_start_message();

    void handle_input_inventory();
    void handle_input_stats();
    void handle_input_levelup();
    void handle_input_interactive_inventory_relevant();
    void handle_input_interactive_inventory_all();
    void handle_input_item_info();
    void handle_input_death();
    void handle_input_death_1();
    void handle_input_confirm_leave_dungeon();
    void handle_input_leave_dungeon();

    //Render the windows.
    void render_no_game();
    void render_get_name();
    void render_get_race();
    void render_get_focused_skills();
    void render_get_starting_items();
    void render_start_message();

    void render_inventory(bool all_categories);
    void render_stats();
    void render_levelup();
    void render_item_info();
    void render_death();
    void render_leave_dungeon();

    //Returns true if the passed category
    bool render_inventory_category(short category);

    //Render the player.
    void render(std::vector< std::vector<bool> >* tile_rendered);

    //Saves an entry to the game log for this game.
    void save_game_log_entry(short cause_of_death,std::string killer="",std::string killer_item="");
};

#endif
