/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "level.h"
#include "world.h"
#include "random_number_generator.h"
#include "light_data.h"

#include <fstream>

using namespace std;

Level::Level(short get_level_x,short get_level_y){
    level_x=get_level_x;
    level_y=get_level_y;

    temperature=TEMP_ROOM_TEMPERATURE;

    fog.resize(level_x,vector<short>(level_y));
    tiles.resize(level_x,vector<Tile>(level_y));
}

double Level::return_absolute_x(){
    return level_x*TILE_SIZE_X;
}

double Level::return_absolute_y(){
    return level_y*TILE_SIZE_Y;
}

bool Level::onMap(unsigned short x, unsigned short y){
	return (x<level_x && y<level_y);
}

bool Level::blockLOS(unsigned short x, unsigned short y){
	if(!onMap(x,y)) return true;

	return this->tiles[x][y].type==TILE_TYPE_WALL || this->tiles[x][y].type==TILE_TYPE_SOLID || this->tiles[x][y].type==TILE_TYPE_DOOR_CLOSED
	|| this->tiles[x][y].type==TILE_TYPE_SECRET_DOOR;
}

void Level::setSeen(unsigned short x, unsigned short y,light_data source_data){
	if(!onMap(x,y)) return;

    //Permanent light.
	if(source_data.permanent){
        this->fog[x][y]=FOG_REVEALED_PERMANENT;
	}
	//Temporary light.
	else{
	    if(this->fog[x][y]==FOG_FOG || this->fog[x][y]==FOG_REVEALED_PERMANENT){
	        this->fog[x][y]=FOG_REVEALED_PERMANENT;
	    }
	    else{
	        this->fog[x][y]=FOG_REVEALED_TEMPORARY;
	    }
	}
}
