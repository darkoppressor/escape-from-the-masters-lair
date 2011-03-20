/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef update_h
#define update_h

#include <SDL.h>

void input();

//Process one full game "turn."
void turn();

void events();

void animation();

void camera(int frame_rate, double ms_per_frame);

void render(int frame_rate, double ms_per_frame);

#endif
