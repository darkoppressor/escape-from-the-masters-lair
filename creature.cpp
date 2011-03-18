/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "quit.h"
#include "save_load.h"
#include "message_log.h"
#include "random_chance.h"

using namespace std;

Creature::Creature(){
    move_state=NONE;

    input_directional=DIRECTIONAL_COMMAND_NONE;
    input_inventory=INVENTORY_COMMAND_NONE;
    inventory_input_state=0;
    command_standard=COMMAND_NONE;

    initiate_move=false;

    is_player=false;

    alive=true;

    gender=GENDER_MALE;

    race=-1;
    race_name="";

    health_max=100;
    health=health_max;

    mana_max=10;
    mana=mana_max;

    experience_level=1;

    experience=0;
    experience_max=100;

    thirst=THIRST_NOT_THIRSTY;

    base_damage_melee_min=1;
    base_damage_melee_max=5;

    base_damage_ranged_min=2;
    base_damage_ranged_max=6;

    base_damage_thrown_min=1;
    base_damage_thrown_max=3;

    for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
        attributes[i]=1;
    }

    for(int i=0;i<SKILL_MAGIC_SUMMONING+1;i++){
        skills[i]=1;
    }

    movement_speed=25;
    next_move=movement_speed;

    //Randomly choose a facing direction.
    int random=random_range(FOV_EAST,FOV_SOUTHEAST);
    facing=(fov_direction_type)random;
    light_on=false;
    source_data.permanent=false;

    for(int i=97;i<=122;i++){
        inventory_letters.push_back((char)i);
    }
    for(int i=65;i<=90;i++){
        inventory_letters.push_back((char)i);
    }

    for(int i=EQUIP_HEAD;i<EQUIP_HOLD_LEFT+1;i++){
        equipment.push_back(0);
    }
}

void Creature::create_money_item(){
    Item temp_item;

    temp_item.inventory_letter='$';
    temp_item.stack=0;
    temp_item.stackable=true;
    temp_item.name="gold piece";
    temp_item.plural_name="gold pieces";
    temp_item.appearance='$';
    temp_item.color=COLOR_GOLD;
    temp_item.weight=0;
    temp_item.monetary_value=1;
    temp_item.category=ITEM_OTHER;
    temp_item.material=MATERIAL_GOLD;

    temp_item.damage_min_melee=1;
    temp_item.damage_max_melee=1;

    temp_item.damage_min_thrown=1;
    temp_item.damage_max_thrown=1;

    temp_item.damage_min_ranged=0;
    temp_item.damage_max_ranged=0;

    inventory.push_back(temp_item);
    inventory[inventory.size()-1].assign_identifier();
}

void Creature::create_light_item(){
    Item temp_item;

    temp_item.category=ITEM_OTHER;
    temp_item.name="lantern";
    temp_item.plural_name="lanterns";
    temp_item.writing="";
    temp_item.appearance='\xE8';
    temp_item.stackable=false;
    temp_item.weight=59;
    temp_item.monetary_value=1;
    temp_item.color=COLOR_BRASS;
    temp_item.material=MATERIAL_BRASS;

    temp_item.damage_min_melee=1;
    temp_item.damage_max_melee=69;

    temp_item.damage_min_thrown=1;
    temp_item.damage_max_thrown=35;

    temp_item.damage_min_ranged=0;
    temp_item.damage_max_ranged=0;

    temp_item.fov_radius=10;
    temp_item.beam=false;
    temp_item.fov_angle=90;

    //Assign the item an available inventory letter.
    temp_item.inventory_letter=assign_inventory_letter();

    inventory.push_back(temp_item);

    //Assign an identifier to the item.
    inventory[inventory.size()-1].assign_identifier();

    //Assign an owner identifier to the item.
    inventory[inventory.size()-1].owner=identifier;
}

void Creature::assign_identifier(){
    identifier=game.assign_identifier(OBJECT_CREATURE);
}

void Creature::return_identifier(){
    game.return_identifier(OBJECT_CREATURE,identifier);
    identifier=0;
}

int Creature::item_category_in_inventory(short category){
    int number_of_items=0;

    for(int i=0;i<inventory.size();i++){
        if(inventory[i].category==category && inventory[i].inventory_letter!='$'){
            number_of_items++;
        }
    }

    return number_of_items;
}

inventory_match Creature::check_for_inventory_match(Item* item_to_check){
    inventory_match match_check;
    match_check.match_found=false;

    //Look through all of the inventory items.
    for(int i=0;i<inventory.size();i++){
        ///I might later need to modify what constitutes a match.
        ///I definitely need to add in effects matching eventually.
        if(inventory[i].name==item_to_check->name && inventory[i].plural_name==item_to_check->plural_name && inventory[i].color==item_to_check->color &&
           inventory[i].appearance==item_to_check->appearance && inventory[i].weight==item_to_check->weight && inventory[i].race==item_to_check->race &&
           inventory[i].dye==item_to_check->dye && inventory[i].writing==item_to_check->writing && inventory[i].category==item_to_check->category &&
           inventory[i].material==item_to_check->material && inventory[i].weapon_category==item_to_check->weapon_category && inventory[i].armor_category==item_to_check->armor_category){
            match_check.match_found=true;
            match_check.inventory_slot=i;
            break;
        }
    }

    return match_check;
}

bool Creature::check_inventory_letter_availability(char letter_to_check){
    //Look through all of the available inventory letters.
    for(int i=0;i<inventory_letters.size();i++){
        //If the letter matches the checked letter.
        if(inventory_letters[i]==letter_to_check){
            //The letter is available.
            return true;
        }
    }

    //The letter was not found, and is thus unavailable.
    return false;
}

char Creature::assign_inventory_letter(char letter_remove){
    //If no letter has been specified.
    if(letter_remove==0){
        char new_letter=inventory_letters[0];

        inventory_letters.erase(inventory_letters.begin());

        return new_letter;
    }
    //If a letter has been specified.
    else{
        char new_letter=0;

        //Look through all of the inventory letters.
        for(int i=0;i<inventory_letters.size();i++){
            //If this letter matches the specified letter.
            if(inventory_letters[i]==letter_remove){
                new_letter=inventory_letters[i];

                inventory_letters.erase(inventory_letters.begin()+i);
            }
        }

        return new_letter;
    }
}

void Creature::return_inventory_letter(char returning_letter){
    inventory_letters.push_back(returning_letter);
}

void Creature::process_turn(){
    if(rc_regain_mana()){
        if(++mana>mana_max){
            mana=mana_max;
        }
    }

    handle_thirst();
}

void Creature::handle_thirst(){
    if(rc_gain_thirst()){
        thirst++;
    }

    ///There should probably be a minimum for thirst for technical reasons.
    ///This will need to be implemented when I add a way to lower thirst.

    //If the creature is bloated.
    if(thirst<=THIRST_BLOATED){
    }
    //If the creature is satiated.
    else if(thirst>=THIRST_SATIATED && thirst<THIRST_NOT_THIRSTY){
    }
    //If the creature is not thirsty.
    else if(thirst>=THIRST_NOT_THIRSTY && thirst<THIRST_THIRSTY){
    }
    //If the creature is thirsty.
    else if(thirst>=THIRST_THIRSTY && thirst<THIRST_WEAK){
    }
    //If the creature is weak.
    else if(thirst>=THIRST_WEAK && thirst<THIRST_FAINTING){
    }
    //If the creature is fainting.
    else if(thirst>=THIRST_FAINTING && thirst<THIRST_DEATH){
    }
    //If the creature is dead.
    else if(thirst>=THIRST_DEATH){
        die(CAUSE_OF_DEATH_THIRST,"","");
    }
}

string Creature::return_thirst_state(){
    string thirst_state="";

    //If the creature is bloated.
    if(thirst<=THIRST_BLOATED){
        thirst_state="bloated";
    }
    //If the creature is satiated.
    else if(thirst>=THIRST_SATIATED && thirst<THIRST_NOT_THIRSTY){
        thirst_state="satiated";
    }
    //If the creature is not thirsty.
    else if(thirst>=THIRST_NOT_THIRSTY && thirst<THIRST_THIRSTY){
        thirst_state="not thirsty";
    }
    //If the creature is thirsty.
    else if(thirst>=THIRST_THIRSTY && thirst<THIRST_WEAK){
        thirst_state="thirsty";
    }
    //If the creature is weak.
    else if(thirst>=THIRST_WEAK && thirst<THIRST_FAINTING){
        thirst_state="weak from thirst";
    }
    //If the creature is fainting.
    else if(thirst>=THIRST_FAINTING && thirst<THIRST_DEATH){
        thirst_state="fainting from lack of water";
    }
    //If the creature is dead.
    else if(thirst>=THIRST_DEATH){
        thirst_state="dead of thirst";
    }

    return thirst_state;
}

void Creature::apply_race(short race_to_apply){
    //If the creature currently has a race (it always will after the first call to apply_race()).
    if(race!=-1){
        //Remove the old race.
        remove_race(race);
    }

    race=race_to_apply;

    race_name=templates.template_races[race_to_apply].name;
    appearance=templates.template_races[race_to_apply].appearance;
    color=templates.template_races[race_to_apply].color;
}

void Creature::remove_race(short race_to_remove){
}

string Creature::return_full_name(){
    string full_name=race_name;

    if(name.length()>0){
        full_name+=" named ";
        full_name+=name;
    }

    return full_name;
}

bool Creature::check_movement(short check_x,short check_y){
    //Keep the creature in the level map's bounds.
    if(check_x<0 || check_x>vector_levels[current_level].level_x-1){
        return false;
    }
    if(check_y<0 || check_y>vector_levels[current_level].level_y-1){
        return false;
    }

    switch(vector_levels[current_level].tiles[check_x][check_y].type){
        //If no special tiles are encountered.
        default:
            return true;
            break;

        case TILE_TYPE_WALL: case TILE_TYPE_SOLID: case TILE_TYPE_LIQUID: case TILE_TYPE_SECRET_DOOR:
            return false;
            break;
    }
}

void Creature::execute_movement(short check_x,short check_y){
    bool clear_for_move=false;

    switch(vector_levels[current_level].tiles[check_x][check_y].type){
        //If no special tiles are encountered.
        default:
            clear_for_move=true;
            break;

        case TILE_TYPE_WALL: case TILE_TYPE_SOLID: case TILE_TYPE_LIQUID: case TILE_TYPE_SECRET_DOOR:
            ///
            break;

        case TILE_TYPE_DOOR_CLOSED:
            //Try to open door.
            if(rc_open_door()){
                vector_levels[current_level].tiles[check_x][check_y].type=TILE_TYPE_DOOR_OPEN;
                update_text_log(string_door_opens.c_str(),is_player);
            }
            else{
                update_text_log(string_door_wont_move.c_str(),is_player);
            }
            break;
    }

    //If the tile is clear for movement, check for something to attack with melee before moving to it.
    if(clear_for_move){
        bool combat_occurred=false;

        //If the creature is a monster.
        if(!is_player){
            //Check for the player.
            if(player.alive && check_x==player.x && check_y==player.y){
                //Attack the player.
                attack_melee(&player);
                combat_occurred=true;
            }
            //If an attack has not yet occurred.
            if(!combat_occurred){
                //Check for a monster.
                for(int i=0;i<vector_levels[current_level].monsters.size();i++){
                    //The monster should ignore itself.
                    if(this!=&vector_levels[current_level].monsters[i]){
                        if(vector_levels[current_level].monsters[i].alive && check_x==vector_levels[current_level].monsters[i].x && check_y==vector_levels[current_level].monsters[i].y){
                            //Don't attack the monster.
                            //However, we still say a combat occurred, so the monster can't move into this space.
                            combat_occurred=true;
                            break;
                        }
                    }
                }
            }
        }
        //If the creature is the player.
        else{
            //Check for a monster.
            for(int i=0;i<vector_levels[current_level].monsters.size();i++){
                if(vector_levels[current_level].monsters[i].alive && check_x==vector_levels[current_level].monsters[i].x && check_y==vector_levels[current_level].monsters[i].y){
                    //Attack the monster.
                    attack_melee(&vector_levels[current_level].monsters[i]);
                    combat_occurred=true;
                    break;
                }
            }
        }

        //If no attack occurred, move the creature.
        if(!combat_occurred){
            x=check_x;
            y=check_y;
        }
    }
}

void Creature::search(){
    int search_radius=1;

    //Check all tiles within search_radius tiles of the creature.
    for(int int_x=x-search_radius;int_x<x+search_radius+1;int_x++){
        for(int int_y=y-search_radius;int_y<y+search_radius+1;int_y++){
            //As long as the tile being checked is within the level's boundaries.
            if(int_x>=0 && int_x<=vector_levels[current_level].level_x-1 && int_y>=0 && int_y<=vector_levels[current_level].level_y-1){
                //If the tile is a secret door, and the creature passes the secret door discovery check.
                if(vector_levels[current_level].tiles[int_x][int_y].type==TILE_TYPE_SECRET_DOOR && rc_discover_secret_door()){
                    //The secret door becomes a floor tile.
                    vector_levels[current_level].tiles[int_x][int_y].type=TILE_TYPE_FLOOR;

                    update_text_log(string_discover_secret_door.c_str(),is_player);
                }
            }
        }
    }
}
