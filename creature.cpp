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
#include "max_item_stack_size.h"

using namespace std;

Creature::Creature(){
    move_state=NONE;

    input_directional=DIRECTIONAL_COMMAND_NONE;
    input_inventory=INVENTORY_COMMAND_NONE;
    inventory_input_state=0;
    two_part_inventory_input_state=0;
    command_standard=COMMAND_NONE;

    item_info=-1;

    initiate_move=false;

    is_player=false;

    gender=GENDER_MALE;

    race=-1;
    race_name="";

    prefix_article="a";

    ///class_name="";
    class_name="<class name>";

    health_max=100;
    health=health_max;

    mana_max=10;
    mana=mana_max;

    experience_level=1;

    experience=0;
    experience_max=300;

    thirst=THIRST_THIRSTY-500;

    base_damage_melee_min=0;
    base_damage_melee_max=0;

    base_damage_ranged_min=0;
    base_damage_ranged_max=0;

    base_damage_thrown_min=0;
    base_damage_thrown_max=0;

    for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
        attributes[i]=1;
    }

    for(int i=0;i<SKILL_MAGIC_SUMMONING+1;i++){
        skills[i][SKILL_EXPERIENCE_LEVEL]=1;
        skills[i][SKILL_EXPERIENCE]=0;
        skills[i][SKILL_EXPERIENCE_MAX]=200;
    }

    for(int i=0;i<3;i++){
        focused_skills[i]=-1;
    }

    for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
        attribute_level_bonuses[i]=0;
    }

    carry_capacity=0;

    movement_speed=0;
    next_move=movement_speed;

    //Randomly choose a facing direction.
    int random=random_range(FOV_EAST,FOV_SOUTHEAST);
    facing=(fov_direction_type)random;
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

bool Creature::give_item(string item_name,int stack_size){
    if(stack_size>MAX_ITEM_STACK_SIZE){
        stack_size=MAX_ITEM_STACK_SIZE;
    }

    short item_category=-1;
    int item_index=-1;

    bool item_found=false;

    for(int i=0;i<templates.template_items.size() && !item_found;i++){
        for(int n=0;n<templates.template_items[i].size() && !item_found;n++){
            if(templates.template_items[i][n].name==item_name){
                item_category=i;
                item_index=n;
                item_found=true;
            }
        }
    }

    //If the specified item can be added.
    if(item_category!=-1 && item_index!=-1 && item_category<templates.template_items.size() && item_index<templates.template_items[item_category].size()){
        //If the inventory is not full, add the item.
        if(inventory.size()<INVENTORY_MAX_SIZE){
            //Generate the item.
            Item temp_item;

            //Apply the selected template to the item.
            temp_item=templates.template_items[item_category][item_index];

            //If this item's template has only certain materials allowed.
            if(templates.template_items[item_category][item_index].allowed_materials.size()>0){
                templates.determine_item_material(&temp_item,item_category,item_index);
            }

            //Run the item's setup function.
            temp_item.setup();

            //Apply the stack size.
            if(temp_item.stackable){
                temp_item.stack=stack_size;
            }

            //Check to see if there is an identical item already in the inventory.
            inventory_match match_check=check_for_inventory_match(&temp_item);

            //If there is already an identical item in the inventory, and the item is stackable.
            if(match_check.match_found && temp_item.stackable){
                //If adding the item's stack to the inventory item would put the inventory item's stack over the max stack size.
                if(inventory[match_check.inventory_slot].stack+temp_item.stack>MAX_ITEM_STACK_SIZE){
                    //The amount to add to the inventory item's stack.
                    int amount_to_add=MAX_ITEM_STACK_SIZE-inventory[match_check.inventory_slot].stack;

                    //Add what can be added to the inventory item.
                    inventory[match_check.inventory_slot].stack+=amount_to_add;

                    //Remove the amount from the temp item's stack.
                    temp_item.stack-=amount_to_add;

                    //Add another item to inventory with the remaining stack size.
                    give_item(temp_item.name,temp_item.stack);
                }
                //If the inventory item has enough space in its stack to hold the whole thing.
                else{
                    inventory[match_check.inventory_slot].stack+=temp_item.stack;
                }
            }
            //If there is no identical item in the inventory, or the item is not stackable.
            else{
                //Determine an inventory letter for the item.

                //Assign the item an available inventory letter.
                temp_item.inventory_letter=assign_inventory_letter();

                //Add the item to the inventory items vector.
                inventory.push_back(temp_item);

                //Assign an identifier to the item.
                inventory[inventory.size()-1].assign_identifier();

                //Assign an owner identifier to the item.
                inventory[inventory.size()-1].owner=identifier;
            }

            return true;
        }
    }

    return false;
}

int Creature::give_item(Item* item){
    //If the inventory is not full, add the item.
    if(inventory.size()<INVENTORY_MAX_SIZE){
        //Check to see if there is an identical item already in the inventory.
        inventory_match match_check=check_for_inventory_match(item);

        //If there is already an identical item in the inventory, and the item is stackable.
        if(match_check.match_found && item->stackable){
            //If adding the item's stack to the inventory item would put the inventory item's stack over the max stack size.
            if(inventory[match_check.inventory_slot].stack+item->stack>MAX_ITEM_STACK_SIZE){
                //The amount to add to the inventory item's stack.
                int amount_to_add=MAX_ITEM_STACK_SIZE-inventory[match_check.inventory_slot].stack;

                //Add what can be added to the inventory item.
                inventory[match_check.inventory_slot].stack+=amount_to_add;

                //Remove the amount from the temp item's stack.
                item->stack-=amount_to_add;

                //Add another item to inventory with the remaining stack size.
                give_item(item);
            }
            //If the inventory item has enough space in its stack to hold the whole thing.
            else{
                inventory[match_check.inventory_slot].stack+=item->stack;
            }

            return match_check.inventory_slot;
        }
        //If there is no identical item in the inventory, or the item is not stackable.
        else{
            //Determine an inventory letter for the item.

            //If the item already has an inventory letter, this means the player assigned one to it, and then dropped it.
            //So we only do this check for the player.
            if(is_player && item->inventory_letter!=0){
                //If the item's inventory letter is available.
                if(check_inventory_letter_availability(item->inventory_letter)){
                    //Remove the item's inventory letter from the inventory letters list.
                    assign_inventory_letter(item->inventory_letter);
                }
                //If the item's inventory letter is unavailable.
                else{
                    //Assign the item an available inventory number.
                    item->inventory_letter=assign_inventory_letter();
                }
            }
            //If the creature is not the player, or the item has no inventory letter already assigned.
            else{
                //Assign the item an available inventory number.
                item->inventory_letter=assign_inventory_letter();
            }

            //Add the item to the inventory items vector.
            inventory.push_back(*item);

            //Assign an identifier to the new item.
            inventory[inventory.size()-1].assign_identifier();

            //Assign an owner identifier to the new item.
            inventory[inventory.size()-1].owner=identifier;

            return inventory.size()-1;
        }
    }

    return -1;
}

item_template_data Creature::return_item_template(string item_name){
    item_template_data item_template;

    item_template.category=-1;
    item_template.index=-1;

    bool item_found=false;

    for(int i=0;i<templates.template_items.size() && !item_found;i++){
        for(int n=0;n<templates.template_items[i].size() && !item_found;n++){
            if(templates.template_items[i][n].name==item_name){
                item_template.category=i;
                item_template.index=n;
                item_found=true;
            }
        }
    }

    return item_template;
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
        if(inventory[i].category==category){
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

        bool matching_coverings=true;

        for(int n=COVERING_ICE;n<COVERING_BLOOD_DRIED+1;n++){
            if(inventory[i].has_covering(n)!=item_to_check->has_covering(n)){
                matching_coverings=false;
                break;
            }
        }

        if(matching_coverings && inventory[i].name==item_to_check->name && inventory[i].plural_name==item_to_check->plural_name && inventory[i].color==item_to_check->color &&
           inventory[i].appearance==item_to_check->appearance && inventory[i].weight==item_to_check->weight && inventory[i].race==item_to_check->race &&
           inventory[i].dye==item_to_check->dye && inventory[i].writing==item_to_check->writing && inventory[i].category==item_to_check->category &&
           inventory[i].material==item_to_check->material && inventory[i].weapon_category==item_to_check->weapon_category && inventory[i].armor_category==item_to_check->armor_category){
            //Even if there is a match, we ignore the item if its stack size is at the max.
            if(inventory[i].stack<MAX_ITEM_STACK_SIZE){
                match_check.match_found=true;
                match_check.inventory_slot=i;
                break;
            }
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
    for(int i=0;i<coverings.size();i++){
        if(!coverings[i].process_turn()){
            coverings.erase(coverings.begin()+i);
            i--;
        }
    }

    //Process the inventory items' turn stuff.
    for(int i=0;i<inventory.size();i++){
        inventory[i].process_turn();
    }

    /**if(rc_regain_mana()){
        int mana_regen=1;

        //Apply the acumen bonus.
        if(rc_regain_mana_bonus(return_attribute_acumen())){
            mana_regen+=2;
        }

        //Apply the comprehension bonus.
        if(rc_regain_mana_bonus(return_attribute_comprehension())){
            mana_regen++;
        }

        mana+=mana_regen;

        if(return_mana()>return_mana_max()){
            mana=mana_max;
        }
    }*/

    if(rc_regain_health()){
        health++;

        if(return_health()>return_health_max()){
            health=health_max;
        }
    }
}

void Creature::process_move(){
    int thirst_change=1;

    //Apply the overencumbered thirst penalty, if any.

    //If the creature is somewhat burdened.
    if(return_inventory_weight()>=return_carry_capacity()*1.5+1 && return_inventory_weight()<=return_carry_capacity()*2.0){
        thirst_change=random_range(1,2);
    }
    //If the creature is quite burdened.
    else if(return_inventory_weight()>=return_carry_capacity()*2.0+1 && return_inventory_weight()<=return_carry_capacity()*2.5){
        thirst_change=random_range(1,2);
    }
    //If the creature is heavily burdened.
    else if(return_inventory_weight()>=return_carry_capacity()*2.5+1 && return_inventory_weight()<=return_carry_capacity()*3.0){
        thirst_change=2;
    }
    //If the creature is overburdened.
    else if(return_inventory_weight()>=return_carry_capacity()*3.0+1){
        thirst_change=random_range(2,3);
    }

    change_thirst(true,thirst_change);
}

void Creature::change_thirst(bool increase,short amount){
    //If thirst is increasing.
    if(increase){
        if(rc_gain_thirst()){
            thirst+=amount;
        }

        //Handle thirst states. These are only handled when thirst increases.

        //If the creature is fainting.
        if(thirst>=THIRST_FAINTING && thirst<THIRST_DEATH){
            ///I need to add 'if the creature is not currently fainted' here.
            if(rc_thirst_faint()){
                ///Faint.
            }
        }
        //If the creature has reached the point of death from thirst.
        else if(thirst>=THIRST_DEATH+(return_attribute_hardiness()*15)){
            if(rc_thirst_lose_health()){
                health--;

                if(return_health()<=0){
                    //The creature dies of thirst.
                    die(CAUSE_OF_DEATH_THIRST);
                }
            }
        }
    }
    //If thirst is decreasing.
    else{
        thirst-=amount;

        //If thirst falls below the thirst floor.
        if(thirst<THIRST_FLOOR){
            //Set the thirst to the thirst floor.
            thirst=THIRST_FLOOR;
        }
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
        thirst_state="dying of thirst";
    }

    return thirst_state;
}

string Creature::return_encumbrance_state(){
    string state="";

    //If the creature is unburdened.
    if(return_inventory_weight()<=return_carry_capacity()){
        state="unburdened";
    }
    //If the creature is slightly burdened.
    else if(return_inventory_weight()>=return_carry_capacity()+1 && return_inventory_weight()<=return_carry_capacity()*1.5){
        state="slightly burdened";
    }
    //If the creature is somewhat burdened.
    else if(return_inventory_weight()>=return_carry_capacity()*1.5+1 && return_inventory_weight()<=return_carry_capacity()*2.0){
        state="somewhat burdened";
    }
    //If the creature is quite burdened.
    else if(return_inventory_weight()>=return_carry_capacity()*2.0+1 && return_inventory_weight()<=return_carry_capacity()*2.5){
        state="quite burdened";
    }
    //If the creature is heavily burdened.
    else if(return_inventory_weight()>=return_carry_capacity()*2.5+1 && return_inventory_weight()<=return_carry_capacity()*3.0){
        state="heavily burdened";
    }
    //If the creature is overburdened.
    else if(return_inventory_weight()>=return_carry_capacity()*3.0+1){
        state="overburdened";
    }

    return state;
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
    weight=templates.template_races[race_to_apply].weight;
}

void Creature::remove_race(short race_to_remove){
    race=-1;
}

string Creature::return_full_name(){
    string full_name=race_name;

    if(boost::algorithm::trim_copy(name).length()>0){
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

        case TILE_TYPE_WALL: case TILE_TYPE_SOLID: case TILE_TYPE_SECRET_DOOR:
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

        case TILE_TYPE_WALL: case TILE_TYPE_SOLID: case TILE_TYPE_SECRET_DOOR:
            ///
            break;

        case TILE_TYPE_DOOR_CLOSED:
            //Try to open door.
            if(rc_open_door()){
                vector_levels[current_level].tiles[check_x][check_y].type=TILE_TYPE_DOOR_OPEN;
                update_text_log("The door opens.",is_player);
            }
            else{
                update_text_log("The door refuses to budge.",is_player);
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

        //If no attack occurred.
        if(!combat_occurred){
            int points_gained=0;
            double inventory_weight=0.0;

            //Exercise the speed skill.

            if(rc_exercise_skill_speed()){
                points_gained=1;
                inventory_weight=return_inventory_weight();
                if(inventory_weight<10.0){
                    inventory_weight=10.0;
                }
                //Apply the encumbrance bonus to the speed skill increase.
                points_gained+=25.0/inventory_weight;
                gain_skill_experience(SKILL_SPEED,points_gained);
            }

            //Exercise the armor skill.

            if(rc_exercise_skill_armor()){
                points_gained=0;
                inventory_weight=return_inventory_weight(ITEM_ARMOR);
                //Apply the encumbrance bonus to the armor skill increase.
                points_gained+=inventory_weight/75.0;
                gain_skill_experience(SKILL_ARMOR,points_gained);
            }

            //Move the creature.
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

                    update_text_log("You stumble upon a hidden passage!",is_player);
                }
            }
        }
    }
}

void Creature::check_this_tile(){
    //If the tile the creature is on is liquid.
    if(vector_levels[current_level].tiles[x][y].type==TILE_TYPE_LIQUID){
        //If the liquid is water, and not covered in ice.
        if(vector_levels[current_level].tiles[x][y].material==MATERIAL_WATER && !vector_levels[current_level].tiles[x][y].has_covering(COVERING_ICE)){
            add_covering(COVERING_WATER);
            die(CAUSE_OF_DEATH_DROWN);
        }
        //If the liquid is lava.
        else if(vector_levels[current_level].tiles[x][y].material==MATERIAL_LAVA){
            die(CAUSE_OF_DEATH_LAVA);
        }
    }
}
