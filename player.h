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

    SDLMod modstate;

    //Current movement state of the camera.
    short cam_state;

    public:

    //If true, a game is currently in progress.
    //If false, a game is not currently in progress.
    bool game_in_progress;

    //String used for retrieving the player's name.
    std::string get_name;

    //String used for retrieving the player's race.
    std::string get_race;

    //Keeps track of what item categories are to be rendered on the inventory window.
    std::vector<short> inventory_categories_to_render;

    //Used to display the death message in the death window.
    std::string death_message;

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
    void handle_input_inventory();
    void handle_input_stats();
    void handle_input_levelup();
    void handle_input_no_game();
    void handle_input_get_name();
    void handle_input_get_race();
    void handle_input_get_focused_skills();
    void handle_input_interactive_inventory_relevant();
    void handle_input_interactive_inventory_all();
    void handle_input_item_info();
    void handle_input_death();

    //Render the windows.
    void render_inventory(bool all_categories);
    void render_stats();
    void render_levelup();
    void render_no_game();
    void render_get_name();
    void render_get_race();
    void render_get_focused_skills();
    void render_item_info();
    void render_death();

    //Returns true if the passed category
    bool render_inventory_category(short category);

    //Render the player.
    void render(std::vector< std::vector<bool> >* tile_rendered);

    //Saves an entry to the game log for this game.
    void save_game_log_entry(short cause_of_death,std::string killer,std::string killer_item);

    //The currently displayed window.
    short current_window;

    //The starting date for this game.
    std::string start_date;

    //The starting time for this game.
    std::string start_time;

    //The ending date for this game.
    std::string end_date;

    //The ending time for this game.
    std::string end_time;

    //The final score of a game.
    //Only used when the game has ended.
    int score;

    int turn;

    bool chat_mode;
    unsigned short text_limit;
    std::string text_log[6];
    Timer timer_cursor;
    bool cursor;
    int cursor_opacity;

    bool option_fullscreen;
    bool option_dev;
    bool option_fps;
    bool option_healthbars;

    //The coordinates and dimensions of the camera.
    double camera_x,camera_y;
    double camera_w,camera_h;
};

#endif
