/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "tile.h"
#include "render.h"
#include "world.h"
#include "collision.h"

using namespace std;

Tile::Tile(){
    //Set the tile's variables:

    x=0;
    y=0;

    type=TILE_TYPE_WALL;
    material=MATERIAL_STONE;
}

void Tile::render(vector< vector<bool> >* tile_rendered){
    //If nothing has been rendered here yet.
    if(!tile_rendered->at(x)[y]){
        //If the tile is in camera bounds.
        if(return_absolute_x()>=player.camera_x-TILE_SIZE && return_absolute_x()<=player.camera_x+player.camera_w && return_absolute_y()>=player.camera_y-TILE_SIZE && return_absolute_y()<=player.camera_y+player.camera_h){
            if(vector_levels[current_level].fog[x][y]>FOG_BLACK || player.option_dev){
                short color=COLOR_WHITE;
                double opacity=1.0;

                if(vector_levels[current_level].fog[x][y]==FOG_FOG && !player.option_dev){
                    opacity=0.5;
                }

                //Set color.
                if(material==MATERIAL_CLOTH){
                    color=COLOR_CLOTH;
                }
                else if(material==MATERIAL_LEATHER){
                    color=COLOR_LEATHER;
                }
                else if(material==MATERIAL_COPPER){
                    color=COLOR_COPPER;
                }
                else if(material==MATERIAL_BRASS){
                    color=COLOR_BRASS;
                }
                else if(material==MATERIAL_BRONZE){
                    color=COLOR_BRONZE;
                }
                else if(material==MATERIAL_IRON){
                    color=COLOR_IRON;
                }
                else if(material==MATERIAL_STEEL){
                    color=COLOR_STEEL;
                }
                else if(material==MATERIAL_SILVER){
                    color=COLOR_SILVER;
                }
                else if(material==MATERIAL_GOLD){
                    color=COLOR_GOLD;
                }
                else if(material==MATERIAL_WOOD){
                    color=COLOR_WOOD;
                }
                else if(material==MATERIAL_BONE){
                    color=COLOR_BONE;
                }
                else if(material==MATERIAL_CORPSE){
                    color=COLOR_CORPSE;
                }
                else if(material==MATERIAL_FOOD){
                    color=COLOR_FOOD;
                }
                else if(material==MATERIAL_STONE){
                    color=COLOR_STONE;
                }
                else if(material==MATERIAL_DIRT){
                    color=COLOR_DIRT;
                }
                else if(material==MATERIAL_GLASS){
                    color=COLOR_GLASS;
                }
                else if(material==MATERIAL_PAPER){
                    color=COLOR_PAPER;
                }
                else if(material==MATERIAL_WATER){
                    color=COLOR_WATER;
                }
                else if(material==MATERIAL_LAVA){
                    color=COLOR_LAVA;
                }
                /**else if(material==MATERIAL_ICE){
                    color=COLOR_ICE;
                }
                else if(material==MATERIAL_FROST){
                    color=COLOR_FROST;
                }*/

                if(type==TILE_TYPE_WALL){
                    msg="\x80";
                }
                else if(type==TILE_TYPE_FLOOR){
                    msg="\xFA";
                }
                else if(type==TILE_TYPE_SOLID){
                    msg="\xB2";
                }
                else if(type==TILE_TYPE_LIQUID){
                    msg="~";
                }
                else if(type==TILE_TYPE_DOWN_STAIRS){
                    msg=">";
                }
                else if(type==TILE_TYPE_UP_STAIRS){
                    msg="<";
                }
                else if(type==TILE_TYPE_DOOR_CLOSED){
                    msg="+";
                }
                else if(type==TILE_TYPE_DOOR_OPEN){
                    msg="\xFE";
                }
                else if(type==TILE_TYPE_FOUNTAIN){
                    msg="{";
                }
                else if(type==TILE_TYPE_SECRET_DOOR){
                    if(player.option_dev){
                        msg="\xD7";
                    }
                    else{
                        msg="\x80";
                    }
                }
                else if(type==TILE_TYPE_TRAP){
                    msg="^";
                }

                font.show((int)(return_absolute_x()-player.camera_x),(int)(return_absolute_y()-player.camera_y),msg,color,opacity);

                tile_rendered->at(x)[y]=true;
            }
        }
    }
}
