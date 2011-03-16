#include "world.h"
#include "render.h"

using namespace std;

//Create the main window.
Game_Window main_window;

Tooltip tooltip;

Random_Number_Generator rng;

//We use this to poll for events when getting input.
SDL_Event event;

//The game's bitmap fonts.
BitmapFont font;
BitmapFont font_small;

//String stream used to easily render ints and stuff as strings.
stringstream ss("");

//ss passes its information into this string for output.
string msg="";

string_input string_input_chat;

Templates templates;

//All of the world variables are defined here. These include the level object (which stores much of the level's information, and has a function that handles loading of levels),
//and the vectors that hold players, enemies, etc.

//All of the level's information is stored here.
Game game;

Player player;

vector<Level> vector_levels;

//Here we define the surface and texture variables:
Image image;

//Here we define the sound files.
///Mix_Chunk *sound_shot=NULL;

//This function is called at the beginning of the game.
void load_world(){
    //Generate the fonts.
    font.build_font("data/images/bitmap_font.png",4096,16);
    font_small.build_font("data/images/bitmap_font_small.png",2048,8);

    //Load the surfaces and textures.
    image.load_images_global();

    //Load the sound effects.
    ///sound_shot=Mix_LoadWAV("data/sounds/shot.wav");
}

//This is called in quit_game(). It frees all of the memory allocated in load_world().
void unload_world(){
    image.current_texture=0;

    //Unload the fonts.
    font.unload_font();
    font_small.unload_font();

    //Delete the surfaces and textures:
    image.unload_images_global();

    //Free the sound effects.
    ///Mix_FreeChunk(sound_shot);
}
