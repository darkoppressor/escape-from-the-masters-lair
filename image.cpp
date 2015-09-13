/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "image.h"
#include "world.h"
#include "render.h"

using namespace std;

//Load global images.
void Image::load_images_global(){
    //Set texture dimensions.

    ///inventory.w=362;
    ///inventory.h=398;

    //Load the global images.
    ///inventory.texture=load_texture("data/images/inventory.png");
}

//Unload global images.
void Image::unload_images_global(){
    //Free up the global images.
    ///glDeleteTextures(1,&inventory.texture);
}
