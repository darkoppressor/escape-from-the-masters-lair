/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef image_data_h
#define image_data_h

#include <SDL.h>
#include <vector>

#define GLEW_STATIC
#include <glew.h>

struct image_data{
    GLuint texture;
    double w;
    double h;
};

#endif
