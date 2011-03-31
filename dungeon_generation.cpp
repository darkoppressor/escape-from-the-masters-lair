/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "game.h"
#include "world.h"
#include "random_number_generator.h"
#include "random_chance.h"

using namespace std;

/**short Game::tile_check(short check_x,short check_y){
    if(check_x>0 && check_x<level_x-1 && check_y>0 && check_y<level_y-1){
        if(generated_tiles[check_x][check_y].type==TILE_TYPE_DOOR_CLOSED){
            return TILE_TYPE_WALL;
        }
        else{
            return generated_tiles[check_x][check_y].type;
        }
    }

    //If no tile was found, this tile is outside the level bounds.
    return TILE_TYPE_WALL;
}*/

void Game::flood_fill_reachable(int x,int y){
    //If the tile is within level bounds.
    if(x>=0 && x<=generated_level_x-1 && y>=0 && y<=generated_level_y-1 &&
    //And if the tile is walkable.
    generated_tiles[x][y].type!=TILE_TYPE_WALL && generated_tiles[x][y].type!=TILE_TYPE_SOLID && generated_tiles[x][y].type!=TILE_TYPE_LIQUID &&
    //And if the tile is not already marked as reachable.
    !tiles_check[x][y]){
        tiles_check[x][y]=true;
    }
    else{
        return;
    }

    flood_fill_reachable(x+1,y);
    flood_fill_reachable(x-1,y);
    flood_fill_reachable(x,y+1);
    flood_fill_reachable(x,y-1);
}

bool Game::all_tiles_reachable(){
    tiles_check.clear();

    tiles_check.resize(generated_level_x,vector<bool>(generated_level_y));

    for(short int_y=0;int_y<generated_level_y;int_y++){
        for(short int_x=0;int_x<generated_level_x;int_x++){
            tiles_check[int_x][int_y]=false;
        }
    }

    bool flood_fill_processed=false;

    //Find a walkable tile.
    for(short int_y=0;!flood_fill_processed && int_y<generated_level_y;int_y++){
        for(short int_x=0;!flood_fill_processed && int_x<generated_level_x;int_x++){
            //If the tile is walkable.
            if(generated_tiles[int_x][int_y].type!=TILE_TYPE_WALL && generated_tiles[int_x][int_y].type!=TILE_TYPE_SOLID && generated_tiles[int_x][int_y].type!=TILE_TYPE_LIQUID){
                flood_fill_reachable(int_x,int_y);
                flood_fill_processed=true;
            }
        }
    }

    //If no walkable tile was found.
    if(!flood_fill_processed){
        return false;
    }

    for(short int_y=0;int_y<generated_level_y;int_y++){
        for(short int_x=0;int_x<generated_level_x;int_x++){
            //If the tile is walkable.
            if(generated_tiles[int_x][int_y].type!=TILE_TYPE_WALL && generated_tiles[int_x][int_y].type!=TILE_TYPE_SOLID && generated_tiles[int_x][int_y].type!=TILE_TYPE_LIQUID &&
            //And if the tile is not marked as reachable.
            !tiles_check[int_x][int_y]){
                return false;
            }
        }
    }

    return true;
}

void Game::level_theme_rooms(){
    struct feature_data{
        short max_room_size_x;
        short min_room_size_x;
        short max_room_size_y;
        short min_room_size_y;

        short max_hall_length;
        short min_hall_length;
    };
    feature_data features[2];

    //Rectangular room.
    features[0].max_room_size_x=16;
    features[0].min_room_size_x=6;
    features[0].max_room_size_y=16;
    features[0].min_room_size_y=6;

    //Circular room.
    features[1].max_room_size_x=7;
    features[1].min_room_size_x=4;
    features[1].max_room_size_y=7;
    features[1].min_room_size_y=4;

    short feature=0;

    draw_rectangle_room(generated_level_x/2,generated_level_y/2,6,6,TILE_TYPE_FLOOR,MATERIAL_STONE);

    //Number of created rooms.
    short rooms_done=1;
    //Used to prevent an infinite loop.
    short iterations=0;
    //Number of rooms to create.
    ///short number_of_rooms=random_range((generated_level_x*generated_level_y)/1000,(generated_level_x*generated_level_y)/100);
    short number_of_rooms=random_range((generated_level_x*generated_level_y)/768,(generated_level_x*generated_level_y)/512);

    //0 = rectangular only
    //1 = circular only
    //2 = both
    short room_types=random_range(0,2);

    //0 = Left.
    //1 = Up.
    //2 = Right.
    //3 = Down.
    short direction=-1;

    short check_x,check_y,check_size_x,check_size_y;
    short hall_check_x,hall_check_y,hall_check_size_x,hall_check_size_y;

    //Create number_of_rooms rooms.
    while(rooms_done<number_of_rooms){
        short x,y;

        //Pick a random tile.
        x=random_range(0,generated_level_x-1);
        y=random_range(0,generated_level_y-1);

        direction=-1;

        //If the tile is a wall.
        if(generated_tiles[x][y].type==TILE_TYPE_WALL){
            //If this is the left wall of a room.
            if(x<generated_level_x-1 && generated_tiles[x+1][y].type==TILE_TYPE_FLOOR){
                //We want to build to the left.
                direction=0;
            }
            //If this is the top wall of a room.
            else if(y<generated_level_y-1 && generated_tiles[x][y+1].type==TILE_TYPE_FLOOR){
                //We want to build up.
                direction=1;
            }
            //If this is the right wall of a room.
            else if(x>0 && generated_tiles[x-1][y].type==TILE_TYPE_FLOOR){
                //We want to build to the right.
                direction=2;
            }
            //If this is the bottom wall of a room.
            else if(y>0 && generated_tiles[x][y-1].type==TILE_TYPE_FLOOR){
                //We want to build down.
                direction=3;
            }

            if(direction!=-1){
                //Decide on a new feature to build.
                int random_int=random_range(0,99);
                if(random_int>=0 && random_int<75){
                    feature=0;
                }
                else if(random_int>=75 && random_int<100){
                    feature=1;
                }

                //
                if(room_types==0){
                    feature=0;
                }
                else if(room_types==1){
                    feature=1;
                }

                ///feature=1;

                if(feature==0){
                    check_size_x=random_range(features[feature].min_room_size_x,features[feature].max_room_size_x);
                    check_size_y=random_range(features[feature].min_room_size_y,features[feature].max_room_size_y);
                }
                else if(feature==1){
                    check_size_x=random_range(features[feature].min_room_size_x,features[feature].max_room_size_x);
                    check_size_y=check_size_x;
                }
                hall_check_size_x=random_range(1,12);
                hall_check_size_y=random_range(1,12);

                if(direction==0){
                    check_x=x-check_size_x-hall_check_size_x;
                    check_y=y;

                    hall_check_x=x-hall_check_size_x;
                    hall_check_y=y;
                    hall_check_size_y=1;
                }
                else if(direction==1){
                    check_x=x;
                    check_y=y-check_size_y-hall_check_size_y;

                    hall_check_x=x;
                    hall_check_y=y-hall_check_size_y;
                    hall_check_size_x=1;
                }
                else if(direction==2){
                    check_x=x+hall_check_size_x;
                    check_y=y;

                    hall_check_x=x;
                    hall_check_y=y;
                    hall_check_size_y=1;
                }
                else if(direction==3){
                    check_x=x;
                    check_y=y+hall_check_size_y;

                    hall_check_x=x;
                    hall_check_y=y;
                    hall_check_size_x=1;
                }

                //If the checked rectangle is within the level bounds.
                if(check_x>=0 && check_y>=0 && check_x+check_size_x<generated_level_x-1 && check_y+check_size_y<generated_level_y-1){
                    if(feature==0){
                        if(check_rectangle(check_x,check_y,check_size_x,check_size_y,TILE_TYPE_WALL) && check_rectangle(hall_check_x,hall_check_y,hall_check_size_x,hall_check_size_y,TILE_TYPE_WALL)){
                            rooms_done++;
                            //Draw room.
                            draw_rectangle_room(check_x,check_y,check_size_x,check_size_y,TILE_TYPE_FLOOR,MATERIAL_STONE);
                            //Draw corridor.
                            draw_rectangle_room(hall_check_x,hall_check_y,hall_check_size_x,hall_check_size_y,TILE_TYPE_FLOOR,MATERIAL_STONE);
                        }
                    }
                    else if(feature==1){
                        if(check_circle(check_x,check_y,check_size_x,TILE_TYPE_WALL) && check_rectangle(hall_check_x,hall_check_y,hall_check_size_x,hall_check_size_y,TILE_TYPE_WALL)){
                            rooms_done++;
                            //Draw room.
                            draw_circle(check_x,check_y,check_size_x,TILE_TYPE_FLOOR,MATERIAL_STONE);
                            //Draw corridor.
                            draw_rectangle_room(hall_check_x,hall_check_y,hall_check_size_x,hall_check_size_y,TILE_TYPE_FLOOR,MATERIAL_STONE);
                        }
                    }
                }
            }
        }
        //Ensure that we don't get stuck in an infinite loop.
        //Probably not needed for a grid based generator, but I'm leaving it in for now.
        if(++iterations>=100000){
            break;
        }
    }
}

void Game::flood_fill_caves(int x,int y){
    //If the tile is within level bounds.
    if(x>=0 && x<=generated_level_x-1 && y>=0 && y<=generated_level_y-1 &&
    //And if the tile is a wall.
    generated_tiles[x][y].type==TILE_TYPE_WALL &&
    //And if the tile is not already marked as checked.
    !tiles_check[x][y]){
        tiles_check[x][y]=true;

        if(random_range(0,99)<25){
            generated_tiles[x][y].type=TILE_TYPE_FLOOR;
        }
    }
    else{
        return;
    }

    flood_fill_caves(x+1,y);
    flood_fill_caves(x-1,y);
    flood_fill_caves(x,y+1);
    flood_fill_caves(x,y-1);
}

void Game::level_theme_caves(){
    tiles_check.clear();

    tiles_check.resize(generated_level_x,vector<bool>(generated_level_y));

    for(short int_y=0;int_y<generated_level_y;int_y++){
        for(short int_x=0;int_x<generated_level_x;int_x++){
            tiles_check[int_x][int_y]=false;
        }
    }

    short start_x=(generated_level_x-1)/2;
    short start_y=(generated_level_y-1)/2;

    for(short int_y=start_y;int_y<start_y+5;int_y++){
        for(short int_x=start_x;int_x<start_x+5;int_x++){
            generated_tiles[int_x][int_y].type=TILE_TYPE_FLOOR;
        }
    }

    flood_fill_caves(start_x-5,start_y-5);
}

void Game::level_theme_big_room(){
    for(short int_y=0;int_y<generated_level_y;int_y++){
        for(short int_x=0;int_x<generated_level_x;int_x++){
            generated_tiles[int_x][int_y].type=TILE_TYPE_FLOOR;
        }
    }
}

void Game::generate_level(){
    //Start the level generation timer.
    Timer timer_level_gen;
    timer_level_gen.start();

    //The number of attempts before this level was generated successfully.
    int gen_attempts=0;

    fprintf(stdout,"Generating dungeon level...\n");

    //**********************************//
    // Begin randomly generating level! //
    //**********************************//

    //****************//
    // Dungeon level. //
    //****************//

    //Keep generating levels until one is generated in which all walkable tiles can be reached.
    do{
        gen_attempts++;

        //Clear the vectors.
        generated_items.clear();
        generated_monsters.clear();
        generated_tiles.clear();

        //Determine the level's dimensions.
        generated_level_x=random_range(LEVEL_X_MIN,LEVEL_X_MAX);
        generated_level_y=random_range(LEVEL_Y_MIN,LEVEL_Y_MAX);

        //Determine the level's starting temperature.
        /**generated_temperature=random_range(0,TEMP_ROOM_TEMPERATURE);
        int random=random_range(0,1);
        if(random==0){
            generated_temperature*=-1;
        }*/
        generated_temperature=TEMP_ROOM_TEMPERATURE;

        //Start with a clean slate.

        generated_tiles.resize(generated_level_x,vector<Tile>(generated_level_y));

        for(short int_y=0;int_y<generated_level_y;int_y++){
            for(short int_x=0;int_x<generated_level_x;int_x++){
                generated_tiles[int_x][int_y].x=int_x;
                generated_tiles[int_x][int_y].y=int_y;
                generated_tiles[int_x][int_y].type=TILE_TYPE_WALL;
                generated_tiles[int_x][int_y].material=MATERIAL_STONE;
            }
        }

        int level_theme=random_range(0,99);

        if(level_theme>=0 && level_theme<80){
            level_theme_rooms();
        }
        else if(level_theme>=80 && level_theme<95){
            ///level_theme_caves();
            level_theme_rooms();
        }
        else if(level_theme>=95 && level_theme<100){
            level_theme_big_room();
        }
        ///level_theme_caves();

        //
        for(short i=0;i<2;i++){
            //Remove any walls that share multiple rooms.
            short random_type;
            short random_material;
            for(short y=0;y<generated_level_y;y++){
                for(short x=0;x<generated_level_x;x++){
                    if(generated_tiles[x][y].type==TILE_TYPE_WALL){
                        int random_int=random_range(0,99);
                        //Replace the wall with floor.
                        if(random_int>=0 && random_int<25){
                            random_type=TILE_TYPE_FLOOR;
                            random_material=MATERIAL_STONE;
                        }
                        //Replace the wall with a closed door.
                        else if(random_int>=25 && random_int<100){
                            random_type=TILE_TYPE_DOOR_CLOSED;
                            random_material=MATERIAL_WOOD;
                        }
                        if(x>0 && generated_tiles[x-1][y].type!=TILE_TYPE_WALL && x<generated_level_x-1 && generated_tiles[x+1][y].type!=TILE_TYPE_WALL){
                            generated_tiles[x][y].type=random_type;
                            generated_tiles[x][y].material=random_material;
                        }
                        else if(y>0 && generated_tiles[x][y-1].type!=TILE_TYPE_WALL && y<generated_level_y-1 && generated_tiles[x][y+1].type!=TILE_TYPE_WALL){
                            generated_tiles[x][y].type=random_type;
                            generated_tiles[x][y].material=random_material;
                        }
                    }
                }
            }

            //Remove any doors that shouldn't exist.
            for(short y=0;y<generated_level_y;y++){
                for(short x=0;x<generated_level_x;x++){
                    if(generated_tiles[x][y].type==TILE_TYPE_DOOR_CLOSED){
                        if(((x>0 && generated_tiles[x-1][y].type==TILE_TYPE_WALL) || x==0) && ((x<generated_level_x-1 && generated_tiles[x+1][y].type==TILE_TYPE_WALL) || x==generated_level_x-1)){
                        }
                        else if(((y>0 && generated_tiles[x][y-1].type==TILE_TYPE_WALL) || y==0) && ((y<generated_level_y-1 && generated_tiles[x][y+1].type==TILE_TYPE_WALL) || y==generated_level_y-1)){
                        }
                        else{
                            generated_tiles[x][y].type=TILE_TYPE_FLOOR;
                            generated_tiles[x][y].material=MATERIAL_STONE;
                        }
                    }
                }
            }
        }

        //Any floor tiles adjacent to the level bounds should be made walls.
        for(short y=0;y<generated_level_y;y++){
            for(short x=0;x<generated_level_x;x++){
                if(generated_tiles[x][y].type==TILE_TYPE_FLOOR){
                    if(x==0 || x==generated_level_x-1 || y==0 || y==generated_level_y-1){
                        generated_tiles[x][y].type=TILE_TYPE_WALL;
                    }
                }
            }
        }

        //Any wall tiles not adjacent to something other than wall tiles and solid tiles should be replaced with solid.
        for(short y=0;y<generated_level_y;y++){
            for(short x=0;x<generated_level_x;x++){
                if(generated_tiles[x][y].type==TILE_TYPE_WALL){
                    if(check_rectangle2(x-1,y-1,3,3,TILE_TYPE_WALL,TILE_TYPE_SOLID)){
                        generated_tiles[x][y].type=TILE_TYPE_SOLID;
                        generated_tiles[x][y].material=MATERIAL_DIRT;
                    }
                }
            }
        }

        //Add liquid.

        //The maximum number.
        int max_liquids=random_range((generated_level_x*generated_level_y)/10000,(generated_level_x*generated_level_y)/5000);
        //The maximum number of tries.
        int random_amount_liquids=random_range((generated_level_x*generated_level_y)/4,(generated_level_x*generated_level_y)/2);
        //The number created successfully.
        int liquids_created=0;

        for(int i=0;i<random_amount_liquids;i++){
            short x,y;

            //Choose a random location in the level.
            x=random_range(0,generated_level_x-1);
            y=random_range(0,generated_level_y-1);

            int radius=random_range(1,5);

            //If the tiles at the random position are appropriate tiles.
            if(check_circle(x,y,radius,TILE_TYPE_FLOOR)){
                draw_circle(x,y,radius,TILE_TYPE_LIQUID,MATERIAL_WATER);
                liquids_created++;
            }

            //If the number generated has exceeded the maximum.
            if(liquids_created>max_liquids){
                break;
            }
        }
    }while(!all_tiles_reachable());

    //Replace some closed doors with secret doors.
    for(short y=0;y<generated_level_y;y++){
        for(short x=0;x<generated_level_x;x++){
            if(generated_tiles[x][y].type==TILE_TYPE_DOOR_CLOSED){
                if(rc_levelgen_secret_door()){
                    generated_tiles[x][y].type=TILE_TYPE_SECRET_DOOR;
                    generated_tiles[x][y].material=MATERIAL_STONE;
                }
            }
        }
    }

    //Replace some closed doors with open doors.
    for(short y=0;y<generated_level_y;y++){
        for(short x=0;x<generated_level_x;x++){
            if(generated_tiles[x][y].type==TILE_TYPE_DOOR_CLOSED){
                if(rc_levelgen_open_door()){
                    generated_tiles[x][y].type=TILE_TYPE_DOOR_OPEN;
                }
            }
        }
    }

    //Place the up stairs.
    while(true){
        short x,y;

        x=random_range(1,generated_level_x-2);
        y=random_range(1,generated_level_y-2);
        if(generated_tiles[x][y].type==TILE_TYPE_FLOOR){/// && generated_tiles[x][y].material==MATERIAL_STONE
            generated_tiles[x][y].type=TILE_TYPE_UP_STAIRS;
            ///generated_tiles[x][y].material=MATERIAL_STONE;
            break;
        }
    }

    //Place the down stairs.
    while(true){
        short x,y;

        x=random_range(1,generated_level_x-2);
        y=random_range(1,generated_level_y-2);
        if(generated_tiles[x][y].type==TILE_TYPE_FLOOR){/// && generated_tiles[x][y].material==MATERIAL_STONE
            generated_tiles[x][y].type=TILE_TYPE_DOWN_STAIRS;
            ///generated_tiles[x][y].material=MATERIAL_STONE;
            break;
        }
    }

    //Add fountains.

    //The maximum number.
    int max_fountains=random_range((generated_level_x*generated_level_y)/15000,(generated_level_x*generated_level_y)/10000);
    //The maximum number of tries.
    int random_amount_fountains=random_range((generated_level_x*generated_level_y)/10,(generated_level_x*generated_level_y)/4);
    //The number created successfully.
    int fountains_created=0;

    for(int i=0;i<random_amount_fountains;i++){
        short x,y;

        //Choose a random location in the level.
        x=random_range(0,generated_level_x-1);
        y=random_range(0,generated_level_y-1);

        //If the tile at the random position is an appropriate tile.
        ///It should also check to make sure there is not more than one other fountain within a small radius.
        if(generated_tiles[x][y].type==TILE_TYPE_FLOOR){
            generated_tiles[x][y].type=TILE_TYPE_FOUNTAIN;
            generated_tiles[x][y].material=MATERIAL_WATER;
            fountains_created++;
        }

        //If the number generated has exceeded the maximum.
        if(fountains_created>max_fountains){
            break;
        }
    }

    //Add traps.

    //The maximum number.
    int max_traps=random_range((generated_level_x*generated_level_y)/25000,(generated_level_x*generated_level_y)/20000);
    //The maximum number of tries.
    int random_amount_traps=random_range((generated_level_x*generated_level_y)/10,(generated_level_x*generated_level_y)/4);
    //The number created successfully.
    int traps_created=0;

    for(int i=0;i<random_amount_traps;i++){
        short x,y;

        //Choose a random location in the level.
        x=random_range(0,generated_level_x-1);
        y=random_range(0,generated_level_y-1);

        //If the tile at the random position is an appropriate tile.
        ///It should also check to make sure there is not more than one other trap within a small radius.
        if(generated_tiles[x][y].type==TILE_TYPE_FLOOR){
            generated_tiles[x][y].type=TILE_TYPE_TRAP;
            ///generated_tiles[x][y].material=MATERIAL_WOOD;
            traps_created++;
        }

        //If the number generated has exceeded the maximum.
        if(traps_created>max_traps){
            break;
        }
    }

    //Modify the material of the tiles according to the level's temperature.
    /**for(short y=0;y<generated_level_y;y++){
        for(short x=0;x<generated_level_x;x++){
            //If the temperature is frosty.
            if(generated_temperature<=TEMP_FREEZING && generated_temperature>=-10){
                switch(generated_tiles[x][y].type){
                    case TILE_TYPE_WALL: case TILE_TYPE_FLOOR: case TILE_TYPE_SOLID: case TILE_TYPE_DOWN_STAIRS:
                    case TILE_TYPE_UP_STAIRS: case TILE_TYPE_DOOR_CLOSED: case TILE_TYPE_DOOR_OPEN: case TILE_TYPE_SECRET_DOOR:
                        generated_tiles[x][y].material=MATERIAL_FROST;
                        break;

                    case TILE_TYPE_LIQUID: case TILE_TYPE_FOUNTAIN:
                        generated_tiles[x][y].material=MATERIAL_ICE;
                        break;
                }
            }

            //If the temperature is icy.
            else if(generated_temperature<-10){
                switch(generated_tiles[x][y].type){
                    case TILE_TYPE_WALL: case TILE_TYPE_FLOOR: case TILE_TYPE_SOLID: case TILE_TYPE_DOWN_STAIRS:
                    case TILE_TYPE_UP_STAIRS: case TILE_TYPE_DOOR_CLOSED: case TILE_TYPE_DOOR_OPEN: case TILE_TYPE_SECRET_DOOR:
                    case TILE_TYPE_LIQUID: case TILE_TYPE_FOUNTAIN:
                        generated_tiles[x][y].material=MATERIAL_ICE;
                        break;
                }
            }
        }
    }*/

    //Add the items.

    //The maximum number of items.
    int max_items=random_range((generated_level_x*generated_level_y)/500,(generated_level_x*generated_level_y)/250);
    //The maximum number of tries.
    int random_amount_items=random_range((generated_level_x*generated_level_y)/10,(generated_level_x*generated_level_y)/4);

    for(int i=0;i<random_amount_items;i++){
        short x,y;

        //Choose a random location in the level.
        x=random_range(0,generated_level_x-1);
        y=random_range(0,generated_level_y-1);

        //Randomly determine the item category.
        ///For now, equal chance for all categories.
        int random_item_category=random_range(ITEM_WEAPON,ITEM_OTHER);

        //Randomly select an item from the chosen category's template.
        ///For now, equal chance for all items within category.
        int random_item_template=random_range(0,templates.template_items[random_item_category].size()-1);

        //If the item is not spawnable.
        if(!templates.template_items[random_item_category][random_item_template].spawnable){
            //Skip this item.
            continue;
        }

        //Randomly select a stack size.
        int random_item_stack=1;
        //If the item is stackable.
        if(templates.template_items[random_item_category][random_item_template].stackable){
            int stackable=random_range(0,99);
            if(stackable<10){
                random_item_stack=random_range(1,3);
            }
        }

        //If the tile at the random position is an appropriate tile for an item.
        ///It should also check to make sure there is not more than one other item within a small radius.
        if(generated_tiles[x][y].type==TILE_TYPE_FLOOR){
            //Generate the item.
            generated_items.push_back(Item());

            //Assign an identifier to the item.
            generated_items[generated_items.size()-1].assign_identifier();

            //Apply the selected template to the item.
            generated_items[generated_items.size()-1]=templates.template_items[random_item_category][random_item_template];

            //Run the item's setup function.
            generated_items[generated_items.size()-1].setup();

            //Apply the randomly selected stack size.
            generated_items[generated_items.size()-1].stack=random_item_stack;

            //Set the newly generated item's position.
            generated_items[generated_items.size()-1].x=x;
            generated_items[generated_items.size()-1].y=y;
        }

        //If the number of items generated has exceeded the maximum.
        if(generated_items.size()>max_items){
            break;
        }
    }

    //Add the monsters.

    //The maximum number of monsters.
    int max_monsters=random_range((generated_level_x*generated_level_y)/2000,(generated_level_x*generated_level_y)/1000);
    //The maximum number of tries.
    int random_amount_monsters=random_range((generated_level_x*generated_level_y)/100,(generated_level_x*generated_level_y)/25);

    for(int i=0;i<random_amount_monsters;i++){
        short x,y;

        //Choose a random location in the level.
        x=random_range(0,generated_level_x-1);
        y=random_range(0,generated_level_y-1);

        //Randomly select a race from the races template.
        ///For now, equal chance of all races.
        int random_race_template=random_range(0,templates.template_races.size()-1);

        //If the tile at the random position is an appropriate tile for a monster.
        ///It should also check to make sure there is not more than one other monster within a small radius.
        if(generated_tiles[x][y].type==TILE_TYPE_FLOOR){
            //Generate the monster.
            generated_monsters.push_back(Monster());

            //Assign an identifier to the monster.
            generated_monsters[generated_monsters.size()-1].assign_identifier();

            //Apply the selected template to the monster.
            generated_monsters[generated_monsters.size()-1].apply_race(random_race_template);

            //Set the monster's base stats.
            generated_monsters[generated_monsters.size()-1].set_base_stats(vector_levels.size());

            //Set the newly generated monster's position.
            generated_monsters[generated_monsters.size()-1].x=x;
            generated_monsters[generated_monsters.size()-1].y=y;

            //Set the monster's starting inventory.
            generated_monsters[generated_monsters.size()-1].set_inventory();
        }

        //If the number of monsters generated has exceeded the maximum.
        if(generated_monsters.size()>max_monsters){
            break;
        }
    }

    //*********************************//
    // Done randomly generating level! //
    //*********************************//

    //Locate and set the starting position for the player.
    for(short y=0;y<generated_level_y;y++){
        for(short x=0;x<generated_level_x;x++){
            if(generated_tiles[x][y].type==TILE_TYPE_UP_STAIRS){
                //Set the player's starting position.
                player.x=generated_tiles[x][y].x;
                player.y=generated_tiles[x][y].y;
            }
        }
    }

    //Create the new level.
    vector_levels.push_back(Level(generated_level_x,generated_level_y));

    //Transfer all of the generated level data to the new level.

    vector_levels[vector_levels.size()-1].temperature=generated_temperature;

    for(short y=0;y<generated_level_y;y++){
        for(short x=0;x<generated_level_x;x++){
            vector_levels[vector_levels.size()-1].tiles[x][y].x=generated_tiles[x][y].x;
            vector_levels[vector_levels.size()-1].tiles[x][y].y=generated_tiles[x][y].y;
            vector_levels[vector_levels.size()-1].tiles[x][y].type=generated_tiles[x][y].type;
            vector_levels[vector_levels.size()-1].tiles[x][y].material=generated_tiles[x][y].material;

            //All tiles start out black.
            vector_levels[vector_levels.size()-1].fog[x][y]=FOG_BLACK;
        }
    }

    //Transfer all of the generated item data to the new level.
    for(int i=0;i<generated_items.size();i++){
        vector_levels[vector_levels.size()-1].items.push_back(generated_items[i]);
    }

    //Transfer all of the generated monster data to the new level.
    for(int i=0;i<generated_monsters.size();i++){
        vector_levels[vector_levels.size()-1].monsters.push_back(Monster());
        vector_levels[vector_levels.size()-1].monsters[i]=generated_monsters[i];
    }

    unsigned int level_gen_time=timer_level_gen.get_ticks();
    double seconds=(double)level_gen_time/1000.0;
    fprintf(stdout,"Finished generating dungeon level in %f seconds and %i try(s).\n",seconds,gen_attempts);
}

void Game::draw_rectangle_room(short start_x,short start_y,short size_x,short size_y,short type,short material){
    //Create the room.
    for(short y=start_y;y<start_y+size_y;y++){
        for(short x=start_x;x<start_x+size_x;x++){
            if(x>=0 && x<=generated_level_x-1 && y>=0 && y<=generated_level_y-1){
                generated_tiles[x][y].type=type;
                generated_tiles[x][y].material=material;
            }
        }
    }
}

void Game::clear_rectangle(short center_x,short center_y,short size,short type_to_clear,short type_replacement,short material_replacement){
    for(short y=center_y-size;y<center_y+size;y++){
        for(short x=center_x-size;x<center_x+size;x++){
            if(generated_tiles[x][y].type==type_to_clear){
                generated_tiles[x][y].type=type_replacement;
                generated_tiles[x][y].material=material_replacement;
            }
        }
    }
}

bool Game::check_rectangle(short start_x,short start_y,short size_x,short size_y,short type_wanted){
    for(short y=start_y;y<start_y+size_y;y++){
        for(short x=start_x;x<start_x+size_x;x++){
            if(x>=0 && x<=generated_level_x-1 && y>=0 && y<=generated_level_y-1 && generated_tiles[x][y].type!=type_wanted){
                return false;
            }
        }
    }
    return true;
}

bool Game::check_rectangle2(short start_x,short start_y,short size_x,short size_y,short type_wanted,short type_wanted2){
    for(short y=start_y;y<start_y+size_y;y++){
        for(short x=start_x;x<start_x+size_x;x++){
            if(x>=0 && x<=generated_level_x-1 && y>=0 && y<=generated_level_y-1 && (generated_tiles[x][y].type!=type_wanted && generated_tiles[x][y].type!=type_wanted2)){
                return false;
            }
        }
    }
    return true;
}

bool Game::check_circle(short center_x,short center_y,short max_radius,short type_wanted){
    short radius=0;

    for(;radius<max_radius;radius++){
        short offset=1;
        short x=0;
        short y=radius-offset;

        short diameter=3-2*(radius-offset);

        while(y>=x){
            if(center_x+x>=0 && center_x+x<=generated_level_x-1 && center_y+y>=0 && center_y+y<=generated_level_y-1){
                if(generated_tiles[center_x+x][center_y+y].type!=type_wanted){
                    return false;
                }
            }
            if(center_x-x>=0 && center_x-x<=generated_level_x-1 && center_y+y>=0 && center_y+y<=generated_level_y-1){
                if(generated_tiles[center_x-x][center_y+y].type!=type_wanted){
                    return false;
                }
            }
            if(center_x+x>=0 && center_x+x<=generated_level_x-1 && center_y-y>=0 && center_y-y<=generated_level_y-1){
                if(generated_tiles[center_x+x][center_y-y].type!=type_wanted){
                    return false;
                }
            }
            if(center_x-x>=0 && center_x-x<=generated_level_x-1 && center_y-y>=0 && center_y-y<=generated_level_y-1){
                if(generated_tiles[center_x-x][center_y-y].type!=type_wanted){
                    return false;
                }
            }

            if(center_x+y>=0 && center_x+y<=generated_level_x-1 && center_y+x>=0 && center_y+x<=generated_level_y-1){
                if(generated_tiles[center_x+y][center_y+x].type!=type_wanted){
                    return false;
                }
            }
            if(center_x-y>=0 && center_x-y<=generated_level_x-1 && center_y+x>=0 && center_y+x<=generated_level_y-1){
                if(generated_tiles[center_x-y][center_y+x].type!=type_wanted){
                    return false;
                }
            }
            if(center_x+y>=0 && center_x+y<=generated_level_x-1 && center_y-x>=0 && center_y-x<=generated_level_y-1){
                if(generated_tiles[center_x+y][center_y-x].type!=type_wanted){
                    return false;
                }
            }
            if(center_x-y>=0 && center_x-y<=generated_level_x-1 && center_y-x>=0 && center_y-x<=generated_level_y-1){
                if(generated_tiles[center_x-y][center_y-x].type!=type_wanted){
                    return false;
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
    }
    return true;
}

void Game::draw_circle(short center_x,short center_y,short max_radius,short type,short material){
    short radius=0;

    for(;radius<max_radius;radius++){
        short offset=1;
        short x=0;
        short y=radius-offset;

        short diameter=3-2*(radius-offset);

        while(y>=x){
            if(center_x+x>=0 && center_x+x<=generated_level_x-1 && center_y+y>=0 && center_y+y<=generated_level_y-1){
                generated_tiles[center_x+x][center_y+y].type=type;
                generated_tiles[center_x+x][center_y+y].material=material;
            }
            if(center_x-x>=0 && center_x-x<=generated_level_x-1 && center_y+y>=0 && center_y+y<=generated_level_y-1){
                generated_tiles[center_x-x][center_y+y].type=type;
                generated_tiles[center_x-x][center_y+y].material=material;
            }
            if(center_x+x>=0 && center_x+x<=generated_level_x-1 && center_y-y>=0 && center_y-y<=generated_level_y-1){
                generated_tiles[center_x+x][center_y-y].type=type;
                generated_tiles[center_x+x][center_y-y].material=material;
            }
            if(center_x-x>=0 && center_x-x<=generated_level_x-1 && center_y-y>=0 && center_y-y<=generated_level_y-1){
                generated_tiles[center_x-x][center_y-y].type=type;
                generated_tiles[center_x-x][center_y-y].material=material;
            }

            if(center_x+y>=0 && center_x+y<=generated_level_x-1 && center_y+x>=0 && center_y+x<=generated_level_y-1){
                generated_tiles[center_x+y][center_y+x].type=type;
                generated_tiles[center_x+y][center_y+x].material=material;
            }
            if(center_x-y>=0 && center_x-y<=generated_level_x-1 && center_y+x>=0 && center_y+x<=generated_level_y-1){
                generated_tiles[center_x-y][center_y+x].type=type;
                generated_tiles[center_x-y][center_y+x].material=material;
            }
            if(center_x+y>=0 && center_x+y<=generated_level_x-1 && center_y-x>=0 && center_y-x<=generated_level_y-1){
                generated_tiles[center_x+y][center_y-x].type=type;
                generated_tiles[center_x+y][center_y-x].material=material;
            }
            if(center_x-y>=0 && center_x-y<=generated_level_x-1 && center_y-x>=0 && center_y-x<=generated_level_y-1){
                generated_tiles[center_x-y][center_y-x].type=type;
                generated_tiles[center_x-y][center_y-x].material=material;
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
                if(fill_x>0 && fill_x<generated_level_x-1 && fill_y>0 && fill_y<generated_level_y-1){
                    if(generated_tiles[fill_x-1][fill_y].type==type && generated_tiles[fill_x+1][fill_y].type==type && generated_tiles[fill_x][fill_y-1].type==type && generated_tiles[fill_x][fill_y+1].type==type){
                        if(generated_tiles[fill_x-1][fill_y].material==material && generated_tiles[fill_x+1][fill_y].material==material && generated_tiles[fill_x][fill_y-1].material==material && generated_tiles[fill_x][fill_y+1].material==material){
                            generated_tiles[fill_x][fill_y].type=type;
                            generated_tiles[fill_x][fill_y].material=material;
                        }
                    }
                }
            }
        }
    }
}
