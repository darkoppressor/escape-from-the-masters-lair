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
        while(random_skill==SKILL_SECURITY || random_skill==SKILL_STEALTH){
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

    int random=random_range(0,5);

    string new_race="";

    if(random==0){
        new_race="high elf";
    }
    else if(random==1){
        new_race="dwarf";
    }
    else if(random==2){
        new_race="gnome";
    }
    else if(random==3){
        new_race="wood elf";
    }
    else if(random==4){
        new_race="yak";
    }
    else if(random==5){
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

    for(int i=SKILL_BLADED_WEAPONS;i<SKILL_MAGIC_SUMMONING+1;i++){
        //If the player has less than the standard max experience for this skill, he has a natural affinity for it.
        if(skills[i][SKILL_EXPERIENCE_MAX]+templates.template_races[race].skills[i][SKILL_EXPERIENCE_MAX]<STARTING_SKILL_EXPERIENCE_MAX){
            for(int n=0;n<3;n++){
                if(focused_skills[n]==-1){
                    focused_skills[n]=i;
                    break;
                }
            }
        }
    }

    //Then, choose 3 decent skills if they are not already chosen.

    bool armor_chosen=false;
    for(int n=0;n<3;n++){
        if(focused_skills[n]==SKILL_ARMOR){
            armor_chosen=true;
            break;
        }
    }
    if(!armor_chosen){
        for(int n=0;n<3;n++){
            if(focused_skills[n]==-1){
                focused_skills[n]=SKILL_ARMOR;
                break;
            }
        }
    }

    bool speed_chosen=false;
    for(int n=0;n<3;n++){
        if(focused_skills[n]==SKILL_SPEED){
            speed_chosen=true;
            break;
        }
    }
    if(!speed_chosen){
        for(int n=0;n<3;n++){
            if(focused_skills[n]==-1){
                focused_skills[n]=SKILL_SPEED;
                break;
            }
        }
    }

    bool bladed_weapons_chosen=false;
    for(int n=0;n<3;n++){
        if(focused_skills[n]==SKILL_BLADED_WEAPONS){
            bladed_weapons_chosen=true;
            break;
        }
    }
    if(!bladed_weapons_chosen){
        for(int n=0;n<3;n++){
            if(focused_skills[n]==-1){
                focused_skills[n]=SKILL_BLADED_WEAPONS;
                break;
            }
        }
    }

    //Finally, fill out our skills with random ones, if necessary.

    for(int i=0;i<3;){
        if(focused_skills[i]==-1){
            //Choose a random skill.
            ///short random_skill=random_range(SKILL_BLADED_WEAPONS,SKILL_MAGIC_SUMMONING);
            ///Disable some skills.
            short random_skill=random_range(SKILL_BLADED_WEAPONS,SKILL_ARMOR);
            while(random_skill==SKILL_SECURITY || random_skill==SKILL_STEALTH){
                random_skill=random_range(SKILL_BLADED_WEAPONS,SKILL_ARMOR);
            }
            ///

            //If the random skill is different from all of the focused skills.
            if(random_skill!=focused_skills[0] && random_skill!=focused_skills[1] && random_skill!=focused_skills[2]){
                focused_skills[i]=random_skill;
                i++;
            }
        }
        else{
            i++;
        }
    }

    done_focusing_skills=true;
}

void Player::game_start_good_items(){
    starting_items_gold=STARTING_ITEMS_GOLD;
    starting_items.clear();

    //Randomly select an item for purchase.
    string item_wanted="";

    int random_item=-1;

    bool need_ammo=false;

    for(int n=SKILL_BLADED_WEAPONS;n<SKILL_MAGIC_SUMMONING+1+20;n++){
        item_wanted="";

        //While we are looking at skills.
        if(n<=SKILL_MAGIC_SUMMONING && is_focused_skill(n)){
            if(n==SKILL_BLADED_WEAPONS){
                item_wanted="simple wooden dagger";
            }
            else if(n==SKILL_BLUNT_WEAPONS){
                item_wanted="simple wooden mace";
            }
            else if(n==SKILL_STABBING_WEAPONS){
                item_wanted="simple wooden spear";
            }
            else if(n==SKILL_LAUNCHER_WEAPONS){
                if(!need_ammo){
                    item_wanted="simple wooden bow";
                    n--;
                    need_ammo=true;
                }
                else{
                    item_wanted="simple wooden arrow";
                }
            }
            else if(n==SKILL_THROWN_WEAPONS){
                item_wanted="simple bone shuriken";
            }
        }
        else if(n>SKILL_MAGIC_SUMMONING){
            item_wanted=available_starting_items[random_range(13,available_starting_items.size()-1)];
        }

        if(item_wanted.length()>0){
            for(int i=0;i<available_starting_items.size();i++){
                if(available_starting_items[i]==item_wanted){
                    random_item=i;
                    break;
                }
            }

            //Determine whether this item has already been purchased.
            bool item_already_purchased=false;

            for(int i=0;i<starting_items.size();i++){
                if(random_item==starting_items[i]){
                    item_already_purchased=true;
                    break;
                }
            }

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
    }

    done_buying_start_items=true;

    update_class_name();
}
