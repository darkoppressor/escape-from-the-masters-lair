/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef game_window_h
#define game_window_h

#include <SDL.h>
#include <SDL_mixer.h>

#define GLEW_STATIC
#include <glew.h>

class Game_Window{
    private:
    public:

    Game_Window();

    bool initialize_opengl();

    bool init_sdl();

    bool init();

    void screenshot();

    void toggle_fullscreen();

    short SCREEN_BPP;

    SDL_Surface* screen;

    SDL_Surface* icon;

    Uint32 icon_colorkey;
};

#endif
