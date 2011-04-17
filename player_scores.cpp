/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "player.h"
#include "version.h"
#include "dungeon_location.h"
#include "dungeon.h"

#include <fstream>
#include <ctime>

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost::algorithm;

void Player::save_game_log_entry(short cause_of_death,string killer,string killer_item,string killer_article,string killer_item_article){
    ofstream save_log("game_log",ifstream::app);

    if(save_log!=NULL){
        time_t now;
        struct tm *tm_now;
        char buff[BUFSIZ];
        now=time(NULL);
        tm_now=localtime(&now);

        strftime(buff,sizeof buff,"%d",tm_now);

        string day=buff;
        string initial_day_digit="";
        initial_day_digit+=day[0];

        //Remove the leading zero, if any.
        if(atoi(initial_day_digit.c_str())==0){
            erase_head(day,1);
        }

        string number_ending="";

        if(atoi(day.c_str())==11 || atoi(day.c_str())==12 || atoi(day.c_str())==13){
            number_ending="th";
        }
        else if(atoi(day.c_str())%10==1){
            number_ending="st";
        }
        else if(atoi(day.c_str())%10==2){
            number_ending="nd";
        }
        else if(atoi(day.c_str())%10==3){
            number_ending="rd";
        }
        else{
            number_ending="th";
        }

        end_date="the ";
        end_date+=day;
        end_date+=number_ending;
        strftime(buff,sizeof buff," day of %B, %Y",tm_now);
        end_date+=buff;

        strftime(buff,sizeof buff,"%H:%M:%S",tm_now);

        end_time=buff;

        //Determine whether the player won the game.

        won=false;

        runestones=0;

        for(int i=0;i<inventory.size();i++){
            if(inventory[i].name=="Runestone"){
                runestones++;
            }
        }

        if(runestones>=3){
            won=true;
        }

        //Calculate score.

        score=0;

        //Add any points that were earned during play.
        score+=score_from_game;

        //Add the gold value of all inventory items.
        for(int i=0;i<inventory.size();i++){
            score+=inventory[i].monetary_value*inventory[i].stack;

            //Add a score bonus if the item is a Runestone.
            if(inventory[i].name=="Runestone"){
                score+=100;
            }
        }

        //Add the dungeon level bonus.
        score+=(DUNGEON_DEPTH*50)-((max_level+1)*50);

        //Add the experience level bonus.
        score+=25*experience_level;

        //Add the attribute bonuses.
        for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
            score+=10*attributes[i];
        }

        //Add the skill bonuses.
        for(int i=0;i<SKILL_MAGIC_SUMMONING+1;i++){
            score+=5*skills[i][SKILL_EXPERIENCE_LEVEL];
        }

        //Double the total score if the player won.
        if(won){
            score*=2;
        }

        save_log<<"name:"<<name<<"\n";
        save_log<<"version:"<<AutoVersion::MAJOR<<"."<<AutoVersion::MINOR<<"\n";
        save_log<<"score:"<<score<<"\n";
        save_log<<"dungeon level:"<<current_level+1<<"\n";
        save_log<<"highest dungeon level:"<<max_level+1<<"\n";
        save_log<<"experience level:"<<experience_level<<"\n";
        save_log<<"health:"<<return_health()<<"/"<<return_health_max()<<"\n";
        ///save_log<<"mana:"<<return_mana()<<"/"<<return_mana_max()<<"\n";
        save_log<<"deaths:"<<deaths<<"\n";
        save_log<<"game start:"<<start_date<<" at "<<start_time<<"\n";
        save_log<<"game end:"<<end_date<<" at "<<end_time<<"\n";
        save_log<<"class:"<<class_name<<"\n";
        save_log<<"race:"<<race_name<<"\n";

        /**if(gender==GENDER_MALE){
            save_log<<"gender:male"<<"\n";
        }
        else if(gender==GENDER_FEMALE){
            save_log<<"gender:female"<<"\n";
        }
        else if(gender==GENDER_NEUTRAL){
            save_log<<"gender:neutral"<<"\n";
        }*/

        if(cause_of_death==CAUSE_OF_DEATH_MELEE){
            death_message="Killed by ";
            death_message+=killer_article;
            death_message+=" ";
            death_message+=killer;
            death_message+=".";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_THROWN){
            death_message="Killed by ";
            death_message+=killer_item_article;
            death_message+=" ";
            death_message+=killer_item;
            death_message+=" thrown by ";
            death_message+=killer_article;
            death_message+=" ";
            death_message+=killer;
            death_message+=".";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_RANGED){
            death_message="Killed by ";
            death_message+=killer_item_article;
            death_message+=" ";
            death_message+=killer_item;
            death_message+=" fired by ";
            death_message+=killer_article;
            death_message+=" ";
            death_message+=killer;
            death_message+=".";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_THIRST){
            death_message="Died of thirst.";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_LAVA){
            death_message="Incinerated by molten lava.";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_DROWN){
            death_message="Drowned.";
        }

        //If the player died.
        if(cause_of_death!=CAUSE_OF_DEATH_NONE){
            save_log<<death_message<<"\n";
        }
        //If the player escaped but did not win.
        else if(cause_of_death==CAUSE_OF_DEATH_NONE && !won){
            save_log<<"Escaped from the Lair."<<"\n";
        }
        //If the player escaped and won.
        else if(cause_of_death==CAUSE_OF_DEATH_NONE && won){
            save_log<<"Escaped from the Lair with "<<runestones<<" Runestones."<<"\n";
        }

        save_log<<"\n\n";

        save_log.close();
        save_log.clear();
    }
    else{
        fprintf(stderr,"Error saving game log entry.\n");
    }
}
