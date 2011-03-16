/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef image_h
#define image_h

#include <vector>

#include "image_data.h"

#include <SDL.h>
#include <string>

#define GLEW_STATIC
#include <glew.h>

class Image{
    private:
    public:
    //
    void load_images_global();
    void unload_images_global();

    //The currently bound texture.
    GLuint current_texture;

    //Global images.
    image_data inventory;
    image_data icon_item_lights;
};

#endif
