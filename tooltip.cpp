/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "tooltip.h"
#include "world.h"
#include "render.h"

using namespace std;

Tooltip::Tooltip(){
    on=false;

    message="";

    //Starting coordinates.
    x=0;
    y=0;

    //Dimensions.
    w=0;
    h=0;
}

void Tooltip::set_dimensions(){
    short current_width=5;
    w=5;
    h=16;

    for(short i=0;i<message.length();i++){
        //
        if(message[i]!='\xA'){
            current_width+=11;
        }
        //If we hit a new line.
        else{
            h+=24;
            if(current_width>w){
                w=current_width;
            }
            current_width=5;
        }

        if(i==message.length()-1){
            if(current_width>w){
                w=current_width;
            }
        }
    }

    h+=8;
}

void Tooltip::render(){
    render_rectangle(x,y-4,w,h,0.5,COLOR_SKY_BLUE);

    font.show(x,y,message,COLOR_WHITE);
}
