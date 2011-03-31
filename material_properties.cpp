/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "material_properties.h"

using namespace std;

double specific_gravities[MATERIAL_LAVA+1];

double densities[MATERIAL_LAVA+1];

void prepare_specific_gravities(){
    specific_gravities[MATERIAL_WOOD]=0.7;
    specific_gravities[MATERIAL_PAPER]=0.9;
    specific_gravities[MATERIAL_DIRT]=1.4;
    specific_gravities[MATERIAL_LEATHER]=0.95;
    specific_gravities[MATERIAL_CLOTH]=1.32;
    specific_gravities[MATERIAL_BONE]=1.92;
    specific_gravities[MATERIAL_FOOD]=2.0;
    specific_gravities[MATERIAL_GLASS]=2.4;
    specific_gravities[MATERIAL_STONE]=2.8;
    specific_gravities[MATERIAL_CORPSE]=3.84;
    specific_gravities[MATERIAL_BRONZE]=7.4;
    specific_gravities[MATERIAL_STEEL]=7.7;
    specific_gravities[MATERIAL_IRON]=7.03;
    specific_gravities[MATERIAL_BRASS]=8.4;
    specific_gravities[MATERIAL_COPPER]=8.89;
    specific_gravities[MATERIAL_SILVER]=10.5;
    specific_gravities[MATERIAL_GOLD]=19.32;
    specific_gravities[MATERIAL_WATER]=1.0;
    specific_gravities[MATERIAL_LAVA]=2.0;
}

void prepare_densities(){
    densities[MATERIAL_WOOD]=0.6;
    densities[MATERIAL_PAPER]=0.7;
    densities[MATERIAL_DIRT]=1.2;
    densities[MATERIAL_LEATHER]=1.3;
    densities[MATERIAL_CLOTH]=1.31;
    densities[MATERIAL_BONE]=1.7;
    densities[MATERIAL_FOOD]=2.0;
    densities[MATERIAL_GLASS]=2.4;
    densities[MATERIAL_STONE]=2.6;
    densities[MATERIAL_CORPSE]=3.4;
    densities[MATERIAL_BRONZE]=7.4;
    densities[MATERIAL_STEEL]=7.85;
    densities[MATERIAL_IRON]=7.87;
    densities[MATERIAL_BRASS]=8.43;
    densities[MATERIAL_COPPER]=8.93;
    densities[MATERIAL_SILVER]=10.49;
    densities[MATERIAL_GOLD]=19.32;
    densities[MATERIAL_WATER]=1.0;
    densities[MATERIAL_LAVA]=2.6;
}

string material_to_string(short material){
    string line="";

    if(material==MATERIAL_CLOTH){
        line="cloth";
    }
    else if(material==MATERIAL_LEATHER){
        line="leather";
    }
    else if(material==MATERIAL_COPPER){
        line="copper";
    }
    else if(material==MATERIAL_BRASS){
        line="brass";
    }
    else if(material==MATERIAL_BRONZE){
        line="bronze";
    }
    else if(material==MATERIAL_IRON){
        line="iron";
    }
    else if(material==MATERIAL_STEEL){
        line="steel";
    }
    else if(material==MATERIAL_SILVER){
        line="silver";
    }
    else if(material==MATERIAL_GOLD){
        line="gold";
    }
    else if(material==MATERIAL_WOOD){
        line="wood";
    }
    else if(material==MATERIAL_BONE){
        line="bone";
    }
    else if(material==MATERIAL_CORPSE){
        line="corpse";
    }
    else if(material==MATERIAL_FOOD){
        line="food";
    }
    else if(material==MATERIAL_STONE){
        line="stone";
    }
    else if(material==MATERIAL_DIRT){
        line="dirt";
    }
    else if(material==MATERIAL_GLASS){
        line="glass";
    }
    else if(material==MATERIAL_PAPER){
        line="paper";
    }
    else if(material==MATERIAL_WATER){
        line="water";
    }
    else if(material==MATERIAL_LAVA){
        line="lava";
    }

    return line;
}

string color_to_string(short color){
    string line="";

    if(color==COLOR_RAINBOW){
        line="rainbow";
    }

    else if(color==COLOR_BLACK){
        line="black";
    }
    else if(color==COLOR_GRAY){
        line="gray";
    }
    else if(color==COLOR_LIGHT_GRAY){
        line="light gray";
    }
    else if(color==COLOR_DARK_GRAY){
        line="dark gray";
    }

    else if(color==COLOR_PAYNES_GRAY){
        line="payne's gray";
    }

    else if(color==COLOR_WHITE){
        line="white";
    }
    else if(color==COLOR_BROWN){
        line="brown";
    }
    else if(color==COLOR_DARK_BROWN){
        line="dark brown";
    }
    else if(color==COLOR_SEPIA){
        line="sepia";
    }
    else if(color==COLOR_SANDY_BROWN){
        line="sandy brown";
    }
    else if(color==COLOR_WHEAT){
        line="wheat";
    }
    else if(color==COLOR_TAN){
        line="tan";
    }
    else if(color==COLOR_PALE_BROWN){
        line="pale brown";
    }
    else if(color==COLOR_BURNT_SIENNA){
        line="burnt sienna";
    }

    else if(color==COLOR_YELLOW){
        line="yellow";
    }
    else if(color==COLOR_SYSTEM){
        line="system";
    }

    else if(color==COLOR_ORANGE){
        line="orange";
    }
    else if(color==COLOR_ORANGE_PEEL){
        line="orange peel";
    }
    else if(color==COLOR_ORANGE_CARROT){
        line="carrot orange";
    }
    else if(color==COLOR_ORANGE_BURNT){
        line="burnt orange";
    }
    else if(color==COLOR_PUMPKIN){
        line="pumpkin";
    }

    else if(color==COLOR_RED){
        line="red";
    }
    else if(color==COLOR_DARK_RED){
        line="dark red";
    }
    else if(color==COLOR_FLAME){
        line="flame";
    }

    else if(color==COLOR_PINK){
        line="pink";
    }
    else if(color==COLOR_ROSE){
        line="rose";
    }
    else if(color==COLOR_SHOCKING_PINK){
        line="shocking pink";
    }

    else if(color==COLOR_PURPLE){
        line="purple";
    }
    else if(color==COLOR_VIOLET){
        line="violet";
    }
    else if(color==COLOR_INDIGO){
        line="indigo";
    }

    else if(color==COLOR_BLUE){
        line="blue";
    }
    else if(color==COLOR_DARK_BLUE){
        line="dark blue";
    }
    else if(color==COLOR_SKY_BLUE){
        line="sky blue";
    }
    else if(color==COLOR_UN_BLUE){
        line="un blue";
    }

    else if(color==COLOR_GREEN){
        line="green";
    }
    else if(color==COLOR_JUNGLE){
        line="jungle";
    }
    else if(color==COLOR_SPRING){
        line="spring";
    }

    else if(color==COLOR_CLOTH){
        line="cloth";
    }
    else if(color==COLOR_LEATHER){
        line="leather";
    }
    else if(color==COLOR_COPPER){
        line="copper";
    }
    else if(color==COLOR_BRASS){
        line="brass";
    }
    else if(color==COLOR_BRONZE){
        line="bronze";
    }
    else if(color==COLOR_IRON){
        line="iron";
    }
    else if(color==COLOR_STEEL){
        line="steel";
    }
    else if(color==COLOR_SILVER){
        line="silver";
    }
    else if(color==COLOR_GOLD){
        line="gold";
    }
    else if(color==COLOR_WOOD){
        line="wood";
    }
    else if(color==COLOR_BONE){
        line="bone";
    }
    else if(color==COLOR_CORPSE){
        line="corpse";
    }
    else if(color==COLOR_FOOD){
        line="food";
    }
    else if(color==COLOR_STONE){
        line="stone";
    }
    else if(color==COLOR_DIRT){
        line="dirt";
    }
    else if(color==COLOR_GLASS){
        line="glass";
    }
    else if(color==COLOR_PAPER){
        line="paper";
    }
    else if(color==COLOR_WATER){
        line="water";
    }
    else if(color==COLOR_LAVA){
        line="lava";
    }

    else if(color==COLOR_ICE){
        line="ice";
    }
    else if(color==COLOR_FROST){
        line="frost";
    }

    return line;
}
