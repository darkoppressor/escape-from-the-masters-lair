/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "game_window.h"
#include "world.h"
#include "quit.h"
#include "pixels.h"
#include "save_load.h"
#include "render.h"

#include <time.h>
#include <string>

using namespace std;

Game_Window::Game_Window(){
    SCREEN_BPP=32;

    screen=NULL;

    icon=NULL;

    icon_colorkey=0;
}

bool Game_Window::initialize_opengl(){
    //Set OpenGL attributes:
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    //Set up the screen:
    if(player.option_fullscreen){
        screen=SDL_SetVideoMode(player.option_screen_width,player.option_screen_height,SCREEN_BPP,SDL_OPENGL|SDL_HWSURFACE|SDL_FULLSCREEN);
    }
    else{
        screen=SDL_SetVideoMode(player.option_screen_width,player.option_screen_height,SCREEN_BPP,SDL_OPENGL|SDL_HWSURFACE);
    }

    //Initialize OpenGL.
    //I am still a beginner at using OpenGL, so I'll put forth my best explanation for how things work, exactly.

    //Enable 2D textures.
    glEnable(GL_TEXTURE_2D);

    //Enable blending.
    glEnable(GL_BLEND);

    //
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    //Set clear color. When the screen is cleared, it will be filled with this color.
    glClearColor(0,0,0,1);

    //Set the OpenGL viewport to be the same size as our screen.
    glViewport(0,0,player.option_screen_width,player.option_screen_height);

    //Set projection:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,player.option_screen_width,player.option_screen_height,0,-1,1);

    //Initialize modelview matrix:
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Initialize GLEW.
    GLenum glew_err=glewInit();
    if(glew_err!=GLEW_OK){
        fprintf(stderr,"GLEW Error: %s\n",glewGetErrorString(glew_err));
        return false;
    }

    //If OpenGL version is not at least 1.1.
    if(!GLEW_VERSION_1_1){
        fprintf(stderr,"OpenGL version is not at least 1.1.\n");
        return false;
    }
    /**else{
        //If OpenGL version is not at least 3.0, check for the needed extension.
        //If that is also unsupported, the game cannot run.
        if(!GLEW_VERSION_3_0){
            if(!GLEW_ARB_framebuffer_object){
                if(!GLEW_EXT_framebuffer_object){
                    fprintf(stderr,"GLEW Error: Extension GL_EXT_framebuffer_object is not supported by your system.\n");
                    return false;
                }
            }
        }
    }*/

    return true;
}

bool Game_Window::init_sdl(){
    //Initialize all of the standard SDL stuff, and return false if it did not initialize properly.
    if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
        fprintf(stderr,"Unable to init SDL: %s\n",SDL_GetError());
        return false;
    }

    ///player.option_screen_width=SDL_GetVideoInfo()->current_w;
    ///player.option_screen_height=SDL_GetVideoInfo()->current_h;

    return true;
}

bool Game_Window::init(){
    //Attempt to center the window on the screen.
    SDL_putenv("SDL_VIDEO_CENTERED=center");

    //Set the window icon.
    icon=SDL_LoadBMP("data/images/icon.bmp");
    icon_colorkey=SDL_MapRGB(icon->format,0,0,0);
    SDL_SetColorKey(icon,SDL_SRCCOLORKEY,icon_colorkey);
    SDL_WM_SetIcon(icon,NULL);

    if(!initialize_opengl()){
        return false;
    }

    //If the screen could not be set up.
    if(!screen){
        fprintf(stderr,"Unable to set video mode: %s\n",SDL_GetError());
        return false;
    }

    //If there were any errors.
    if(glGetError()!=GL_NO_ERROR){
        fprintf(stderr,"OpenGL initialization failed.\n");
        return false;
    }

    //Start up the audio system.
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)==-1){
        fprintf(stderr,"SDL_Mixer initialization failed.\n");
        return false;
    }

    Mix_AllocateChannels(2000);

    //Set the window caption.
    SDL_WM_SetCaption("Escape from the Master's Lair","Escape from the Master's Lair");

    //Show the mouse cursor.
    SDL_ShowCursor(1);

    //Everything initialized fine, so return true.
    return true;
}

void Game_Window::toggle_fullscreen(){
    player.option_fullscreen=!player.option_fullscreen;
    options_save();

    //To toggle fullscreen with OpenGL, we must reinitialize all of the OpenGL stuff and reload the textures.

    initialize_opengl();

    unload_world();
    load_world();
}

void Game_Window::screenshot(){
    //First, we setup the filename for the screenshot.

    //Determine the date and time.
    time_t now;
    struct tm *tm_now;
    char buff[BUFSIZ];
    now=time(NULL);
    tm_now=localtime(&now);

    //Store the date and time in buff.
    strftime(buff,sizeof buff,"%Y-%A_%B_%d-%H%M.%S",tm_now);

    //Copy buff's data into the date string for use with screenshot_name below.
    string date=buff;

    //Used to store the filename of the screenshot.
    string screenshot_name;

    //Set the filename.
    screenshot_name="screenshots/";
    screenshot_name+=date;
    screenshot_name+=".bmp";

    //Create temporary surfaces.
    SDL_Surface *surface=NULL;
    SDL_Surface *flipped_surface=NULL;

    //This will hold the pixel data from the frame buffer.
    unsigned char pixel_data[4*player.option_screen_width*player.option_screen_height];

    //Read the pixels from the frame buffer and store them in pixel_data.
    glReadPixels(0,0,player.option_screen_width,player.option_screen_height,GL_BGRA,GL_UNSIGNED_BYTE,pixel_data);

    //Create an SDL surface from this pixel data.
    surface=SDL_CreateRGBSurfaceFrom(pixel_data,player.option_screen_width,player.option_screen_height,screen->format->BitsPerPixel,player.option_screen_width*screen->format->BytesPerPixel,screen->format->Rmask,screen->format->Gmask,screen->format->Bmask,screen->format->Amask);

    //Create a SDL surface for holding the flipped image.
    flipped_surface=SDL_CreateRGBSurface(SDL_SWSURFACE,surface->w,surface->h,surface->format->BitsPerPixel,surface->format->Rmask,surface->format->Gmask,surface->format->Bmask,surface->format->Amask);

    //If the surface must be locked.
    if(SDL_MUSTLOCK(surface)){
        //Lock the surface.
        SDL_LockSurface(surface);
    }

    //Read the pixel data from surface and store it (flipped vertically) in flipped_surface.
    for(int x=0,rx=surface->w-1;x<surface->w;x++,rx--){
        for(int y=0,ry=surface->h-1;y<surface->h;y++,ry--){
            Uint32 pixel=surface_get_pixel(surface,x,y);
            surface_put_pixel(flipped_surface,x,ry,pixel);
        }
    }

    //If the surface had to be locked.
    if(SDL_MUSTLOCK(surface)){
        //Unlock the surface.
        SDL_UnlockSurface(surface);
    }

    //Save the flipped surface to the screenshot file.
    SDL_SaveBMP(flipped_surface,screenshot_name.c_str());

    //Free the surfaces.
    SDL_FreeSurface(flipped_surface);
    SDL_FreeSurface(surface);
}
