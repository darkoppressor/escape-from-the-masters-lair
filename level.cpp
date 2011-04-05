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

void Level::draw_circle_covering(short center_x,short center_y,short max_radius,short covering_type,int covering_chance){
    short radius=0;

    for(;radius<max_radius;radius++){
        short offset=1;
        short x=0;
        short y=radius-offset;

        short diameter=3-2*(radius-offset);

        while(y>=x){
            if(center_x+x>=0 && center_x+x<=level_x-1 && center_y+y>=0 && center_y+y<=level_y-1){
                if(tiles[center_x+x][center_y+y].type!=TILE_TYPE_LIQUID && tiles[center_x+x][center_y+y].type!=TILE_TYPE_FOUNTAIN){
                    if(random_range(0,99)<covering_chance){
                        tiles[center_x+x][center_y+y].add_covering(covering_type);
                    }
                    for(int i=0;i<items.size();i++){
                        if(items[i].x==center_x+x && items[i].y==center_y+y){
                            if(random_range(0,99)<covering_chance){
                                items[i].coverings.push_back(covering_type);
                            }
                        }
                    }
                }
            }
            if(center_x-x>=0 && center_x-x<=level_x-1 && center_y+y>=0 && center_y+y<=level_y-1){
                if(tiles[center_x-x][center_y+y].type!=TILE_TYPE_LIQUID && tiles[center_x-x][center_y+y].type!=TILE_TYPE_FOUNTAIN){
                    if(random_range(0,99)<covering_chance){
                        tiles[center_x-x][center_y+y].add_covering(covering_type);
                    }
                    for(int i=0;i<items.size();i++){
                        if(items[i].x==center_x-x && items[i].y==center_y+y){
                            if(random_range(0,99)<covering_chance){
                                items[i].coverings.push_back(covering_type);
                            }
                        }
                    }
                }
            }
            if(center_x+x>=0 && center_x+x<=level_x-1 && center_y-y>=0 && center_y-y<=level_y-1){
                if(tiles[center_x+x][center_y-y].type!=TILE_TYPE_LIQUID && tiles[center_x+x][center_y-y].type!=TILE_TYPE_FOUNTAIN){
                    if(random_range(0,99)<covering_chance){
                        tiles[center_x+x][center_y-y].add_covering(covering_type);
                    }
                    for(int i=0;i<items.size();i++){
                        if(items[i].x==center_x+x && items[i].y==center_y-y){
                            if(random_range(0,99)<covering_chance){
                                items[i].coverings.push_back(covering_type);
                            }
                        }
                    }
                }
            }
            if(center_x-x>=0 && center_x-x<=level_x-1 && center_y-y>=0 && center_y-y<=level_y-1){
                if(tiles[center_x-x][center_y-y].type!=TILE_TYPE_LIQUID && tiles[center_x-x][center_y-y].type!=TILE_TYPE_FOUNTAIN){
                    if(random_range(0,99)<covering_chance){
                        tiles[center_x-x][center_y-y].add_covering(covering_type);
                    }
                    for(int i=0;i<items.size();i++){
                        if(items[i].x==center_x-x && items[i].y==center_y-y){
                            if(random_range(0,99)<covering_chance){
                                items[i].coverings.push_back(covering_type);
                            }
                        }
                    }
                }
            }

            if(center_x+y>=0 && center_x+y<=level_x-1 && center_y+x>=0 && center_y+x<=level_y-1){
                if(tiles[center_x+y][center_y+x].type!=TILE_TYPE_LIQUID && tiles[center_x+y][center_y+x].type!=TILE_TYPE_FOUNTAIN){
                    if(random_range(0,99)<covering_chance){
                        tiles[center_x+y][center_y+x].add_covering(covering_type);
                    }
                    for(int i=0;i<items.size();i++){
                        if(items[i].x==center_x+y && items[i].y==center_y+x){
                            if(random_range(0,99)<covering_chance){
                                items[i].coverings.push_back(covering_type);
                            }
                        }
                    }
                }
            }
            if(center_x-y>=0 && center_x-y<=level_x-1 && center_y+x>=0 && center_y+x<=level_y-1){
                if(tiles[center_x-y][center_y+x].type!=TILE_TYPE_LIQUID && tiles[center_x-y][center_y+x].type!=TILE_TYPE_FOUNTAIN){
                    if(random_range(0,99)<covering_chance){
                        tiles[center_x-y][center_y+x].add_covering(covering_type);
                    }
                    for(int i=0;i<items.size();i++){
                        if(items[i].x==center_x-y && items[i].y==center_y+x){
                            if(random_range(0,99)<covering_chance){
                                items[i].coverings.push_back(covering_type);
                            }
                        }
                    }
                }
            }
            if(center_x+y>=0 && center_x+y<=level_x-1 && center_y-x>=0 && center_y-x<=level_y-1){
                if(tiles[center_x+y][center_y-x].type!=TILE_TYPE_LIQUID && tiles[center_x+y][center_y-x].type!=TILE_TYPE_FOUNTAIN){
                    if(random_range(0,99)<covering_chance){
                        tiles[center_x+y][center_y-x].add_covering(covering_type);
                    }
                    for(int i=0;i<items.size();i++){
                        if(items[i].x==center_x+y && items[i].y==center_y-x){
                            if(random_range(0,99)<covering_chance){
                                items[i].coverings.push_back(covering_type);
                            }
                        }
                    }
                }
            }
            if(center_x-y>=0 && center_x-y<=level_x-1 && center_y-x>=0 && center_y-x<=level_y-1){
                if(tiles[center_x-y][center_y-x].type!=TILE_TYPE_LIQUID && tiles[center_x-y][center_y-x].type!=TILE_TYPE_FOUNTAIN){
                    if(random_range(0,99)<covering_chance){
                        tiles[center_x-y][center_y-x].add_covering(covering_type);
                    }
                    for(int i=0;i<items.size();i++){
                        if(items[i].x==center_x-y && items[i].y==center_y-x){
                            if(random_range(0,99)<covering_chance){
                                items[i].coverings.push_back(covering_type);
                            }
                        }
                    }
                }
            }

            if(diameter<0){
                diameter+=4*x+6;
            }
            else{
                diameter+=4*(x-y)+10;
                y--;
            }
            x++;
        }

        for(short fill_y=center_y-radius;fill_y<center_y+radius;fill_y++){
            for(short fill_x=center_x-radius;fill_x<center_x+radius;fill_x++){
                if(fill_x>0 && fill_x<level_x-1 && fill_y>0 && fill_y<level_y-1){
                    if(tiles[fill_x-1][fill_y].has_covering(covering_type) && tiles[fill_x+1][fill_y].has_covering(covering_type) && tiles[fill_x][fill_y-1].has_covering(covering_type) && tiles[fill_x][fill_y+1].has_covering(covering_type)){
                        if(tiles[fill_x][fill_y].type!=TILE_TYPE_LIQUID && tiles[fill_x][fill_y].type!=TILE_TYPE_FOUNTAIN){
                            if(random_range(0,99)<covering_chance){
                                tiles[fill_x][fill_y].add_covering(covering_type);
                            }
                            for(int i=0;i<items.size();i++){
                                if(items[i].x==fill_x && items[i].y==fill_y){
                                    if(random_range(0,99)<covering_chance){
                                        items[i].coverings.push_back(covering_type);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
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

    //If the light is permanent.
    if(this->fog[x][y]!=FOG_PERMANENT){
        //Standard light.
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
}
