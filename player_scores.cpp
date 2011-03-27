/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "player.h"
#include "version.h"
#include "dungeon_location.h"

#include <fstream>
#include <ctime>

using namespace std;

void Player::save_game_log_entry(short cause_of_death,string killer,string killer_item){
    ofstream save_log("game_log",ifstream::app);

    if(save_log!=NULL){
        time_t now;
        struct tm *tm_now;
        char buff[BUFSIZ];
        now=time(NULL);
        tm_now=localtime(&now);
        strftime(buff,sizeof buff,"%Y-%m-%d %H:%M:%S",tm_now);

        //Calculate score.

        int score=0;

        //Add in all gold and the gold value of all inventory items.
        for(int i=0;i<inventory.size();i++){
            score+=inventory[i].monetary_value*inventory[i].stack;
        }

        save_log<<"name:"<<name<<"\n";
        save_log<<"version:"<<AutoVersion::MAJOR<<"."<<AutoVersion::MINOR<<"\n";
        save_log<<"score:"<<score<<"\n";
        save_log<<"dungeon level:"<<current_level+1<<"\n";
        save_log<<"maximum dungeon level:"<<max_level+1<<"\n";
        save_log<<"experience level:"<<experience_level<<"\n";
        save_log<<"health:"<<health<<"/"<<health_max<<"\n";
        save_log<<"mana:"<<mana<<"/"<<mana_max<<"\n";
        ///save_log<<"deaths:"<<deaths<<"\n";
        save_log<<"game start:"<<start_date<<"\n";
        save_log<<"game end:"<<buff<<"\n";
        ///save_log<<"class:"<<class_name<<"\n";
        save_log<<"race:"<<race_name<<"\n";

        if(gender==GENDER_MALE){
            save_log<<"gender:male"<<"\n";
        }
        else if(gender==GENDER_FEMALE){
            save_log<<"gender:female"<<"\n";
        }
        else if(gender==GENDER_NEUTRAL){
            save_log<<"gender:neutral"<<"\n";
        }

        if(cause_of_death==CAUSE_OF_DEATH_MELEE){
            save_log<<"Killed by a "<<killer<<"."<<"\n";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_THROWN){
            save_log<<"Killed by a "<<killer_item<<" thrown by a "<<killer<<"."<<"\n";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_RANGED){
            save_log<<"Killed by a "<<killer_item<<" fired by a "<<killer<<"."<<"\n";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_THIRST){
            save_log<<"Died of thirst."<<"\n";
        }

        save_log<<"\n\n";

        save_log.close();
        save_log.clear();
    }
    else{
        fprintf(stderr,"Error saving game log entry.\n");
    }
}
