/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include <SDL.h>

#include "timer.h"

using namespace std;

Timer::Timer(){
    //Initializes the variables.
    start_ticks=0;
    paused_ticks=0;
    paused=false;
    started=false;
}

void Timer::start(){
    //Start the timer.
    started=true;

    //Unpause the timer.
    paused=false;

    //Get the current clock time.
    start_ticks=SDL_GetTicks();
}

void Timer::stop(){
    //Stop the timer.
    started=false;

    //Unpause the timer.
    paused=false;
}

Uint32 Timer::get_ticks(){
    //If the timer is running.
    if(started==true){
        //If the timer is paused.
        if(paused==true){
            //Return the number of ticks when the timer was paused.
            return paused_ticks;
        }
        else{
            //Return the current time minus the start time.
            return SDL_GetTicks()-start_ticks;
        }
    }
    //If the timer isn't running.
    return 0;
}

void Timer::pause(){
    //If the timer is running and isn't already paused.
    if(started==true && paused==false){
        //Pause the timer.
        paused=true;

        //Calculate the paused ticks.
        paused_ticks=SDL_GetTicks()-start_ticks;
    }
}

void Timer::unpause(){
    //If the timer is paused.
    if(paused==true){
        //Unpause the timer.
        paused=false;

        //Reset the starting ticks.
        start_ticks=SDL_GetTicks()-paused_ticks;

        //Reset the paused ticks.
        paused_ticks=0;
    }
}

bool Timer::is_started(){
    return started;
}

bool Timer::is_paused(){
    return paused;
}
