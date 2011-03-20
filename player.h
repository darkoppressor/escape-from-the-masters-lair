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

    Player();

    //Setup the player's starting inventory.
    void set_inventory();

    //Set the base stats of the player.
    void set_base_stats();

    //Load initial data for the player.
    void load_data();

    //Get input.
    void handle_input();

    //Move the player.
    void move();

    void update_fov();

    //Handle camera movement.
    void set_camera();

    void update_window_caption(int frame_rate,double ms_per_frame);

    //Render the inventory.
    void render_inventory();

    //
    void render_stats();

    //Render the player.
    void render(std::vector< std::vector<bool> >* tile_rendered);

    //
    bool display_inventory;
    bool display_stats;

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

    //The coordinates and dimensions of the camera.
    double camera_x,camera_y;
    double camera_w,camera_h;
};

#endif
