/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef pixels_h
#define pixels_h

#include <SDL.h>

Uint32 surface_get_pixel(SDL_Surface *surface,int x,int y);

void surface_put_pixel(SDL_Surface *surface,int x,int y,Uint32 pixel);

#endif
