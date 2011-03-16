#ifndef update_h
#define update_h

#include <SDL.h>

void input();

void movement();

void events();

void animation();

void camera(int frame_rate, double ms_per_frame);

void render(int frame_rate, double ms_per_frame);

#endif
