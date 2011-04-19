/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "player.h"
#include "world.h"
#include "starting_values.h"

using namespace std;

void Player::game_start_random_all(){
    game_start_random_race();
    game_start_random_skills();
    game_start_random_items();
}

void Player::game_start_random_race(){
    race=random_range(0,templates.template_races.size()-1);

    get_race.clear();
}

void Player::game_start_random_skills(){
    focused_skills[0]=-1;
    focused_skills[1]=-1;
    focused_skills[2]=-1;

    //Set the player's focused skills randomly.
    for(int i=0;i<3;){
        //Choose a random skill.
        ///short random_skill=random_range(SKILL_BLADED_WEAPONS,SKILL_MAGIC_SUMMONING);
        ///Disable some skills.
        short random_skill=random_range(SKILL_BLADED_WEAPONS,SKILL_ARMOR);
        while(random_skill==SKILL_SECURITY || random_skill==SKILL_STEALTH || random_skill==SKILL_FIGHTING || random_skill==SKILL_DODGING){
            random_skill=random_range(SKILL_BLADED_WEAPONS,SKILL_ARMOR);
        }
        ///

        //If the random skill is different from all of the focused skills.
        if(random_skill!=focused_skills[0] && random_skill!=focused_skills[1] && random_skill!=focused_skills[2]){
            focused_skills[i]=random_skill;
            i++;
        }
    }

    done_focusing_skills=true;
}

void Player::game_start_random_items(){
    starting_items_gold=STARTING_ITEMS_GOLD;
    starting_items.clear();

    for(int i=0;i<100 && starting_items_gold>0;i++){
        //Randomly select an item for purchase.
        int random_item=random_range(0,available_starting_items.size()-1);

        //Determine whether this item has already been purchased.
        bool item_already_purchased=false;

        for(int n=0;n<starting_items.size();n++){
            if(random_item==starting_items[n]){
                item_already_purchased=true;
                break;
            }
        }

        //If the item has not already been purchased.
        if(!item_already_purchased){
            //Determine the monetary value of the item.
            int item_value=0;

            item_template_data item_template=return_item_template(available_starting_items[random_item]);

            item_value=templates.template_items[item_template.category][item_template.index].monetary_value;

            //If we have enough money for this item.
            if(item_value<=starting_items_gold){
                //Add the purchased item.
                starting_items.push_back(random_item);

                //Remove the item's value from the starting gold.
                starting_items_gold-=item_value;
            }
        }
    }

    done_buying_start_items=true;

    update_class_name();
}

void Player::game_start_good_all(){
    game_start_good_race();
    game_start_good_skills();
    game_start_good_items();
}

void Player::game_start_good_race(){
    //Pick a good race.

    int random=random_range(0,2);

    string new_race="";

    if(random==0){
        new_race="human";
    }
    else if(random==1){
        new_race="yak";
    }
    else if(random==2){
        new_race="lynx";
    }

    //Find the race template index for the chosen race.
    for(int i=0;i<templates.template_races.size();i++){
        if(templates.template_races[i].name==new_race){
            race=i;
            break;
        }
    }

    get_race.clear();
}

void Player::game_start_good_skills(){
    focused_skills[0]=-1;
    focused_skills[1]=-1;
    focused_skills[2]=-1;

    //If the player has an affiinity for one or more skills, choose those first.

    for(int i=0;i<SKILL_MAGIC_SUMMONING+1;i++){
        //If the player has less than the standard max experience for this skill, he has a natural affinity for it.
        if(skills[i][SKILL_EXPERIENCE_MAX]<STARTING_SKILL_EXPERIENCE_MAX){
            for(int n=0;n<3;n++){
                if(focused_skills[n]==-1){
                    focused_skills[n]=i;
                    break;
                }
            }
        }
    }

    for(int n=0;n<3;n++){
        if(focused_skills[n]==-1){
            focused_skills[n]=SKILL_BLADED_WEAPONS;
            break;
        }
    }
    for(int n=0;n<3;n++){
        if(focused_skills[n]==-1){
            focused_skills[n]=SKILL_ARMOR;
            break;
        }
    }
    for(int n=0;n<3;n++){
        if(focused_skills[n]==-1){
            focused_skills[n]=SKILL_SPEED;
            break;
        }
    }

    done_focusing_skills=true;
}

void Player::game_start_good_items(){
    starting_items_gold=STARTING_ITEMS_GOLD;
    starting_items.clear();

    ///

    done_buying_start_items=true;

    update_class_name();
}
