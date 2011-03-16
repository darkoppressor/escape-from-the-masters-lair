#include "object.h"
#include "tile_size.h"
#include "enumerations.h"

using namespace std;

Object::Object(){
    x=0;
    y=0;
    name="";
    appearance="";
    color=COLOR_WHITE;
}

double Object::return_absolute_x(){
    return x*TILE_SIZE;
}

double Object::return_absolute_y(){
    return y*TILE_SIZE;
}
