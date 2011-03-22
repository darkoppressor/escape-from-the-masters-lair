/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "quit.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include "world.h"
#include "save_load.h"

using namespace std;

void quit_game(){
    //If any music is playing, fade it out and then stop it.
    while(!Mix_FadeOutMusic(1000) && Mix_PlayingMusic()){
        SDL_Delay(100);
    }
    Mix_HaltMusic();

    //Disable unicode.
    SDL_EnableUNICODE(SDL_DISABLE);

    fov_settings_free(&player.fov_settings);

    //This function frees any dynamically allocated memory that was loaded in load_world().
    unload_world();

    //Close the audio subsystem.
    Mix_CloseAudio();

    //Shutdown SDL.
    SDL_Quit();

    if(!options_save()){
        fprintf(stderr,"Failed to save 'options.cfg.'\n");
    }

    //Print a success message to stdout.txt.
    printf("Exiting successfully.\n");

    //Tell the program to exit successfully.
    exit(EXIT_SUCCESS);
}
