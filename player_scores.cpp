/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "player.h"
#include "version.h"
#include "dungeon_location.h"
#include "dungeon.h"
#include "highscore.h"

#include <fstream>
#include <ctime>

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost::algorithm;

void Player::save_highscore(string score_string){
    //Load the highscores.

    vector<Highscore> highscores;

    ifstream load("highscores",ifstream::in);

    if(load!=NULL){
        //As long as we haven't reached the end of the file.
        while(!load.eof()){
            string line="";

            //Grab the next line of the file.
            getline(load,line);

            //Clear initial whitespace from the line.
            trim(line);

            //If the line begins a highscore entry.
            if(icontains(line,"<highscore>")){
                highscores.push_back(Highscore());

                //As long as we haven't reached the end of the file.
                while(!load.eof()){
                    //Grab the next line of the file.
                    getline(load,line);

                    //Clear initial whitespace from the line.
                    trim(line);

                    //Score.
                    if(icontains(line,"<score>")){
                        line.erase(0,7);

                        highscores[highscores.size()-1].score=atoi(line.c_str());
                    }
                    //Score string.
                    else if(icontains(line,"<score string>")){
                        line.erase(0,14);

                        highscores[highscores.size()-1].score_string=line;

                        break;
                    }
                }
            }
        }

        load.close();
        load.clear();
    }
    else{
        fprintf(stderr,"Error loading highscores file.\n");
    }

    for(int i=0;i<highscores.size();i++){
        replace_all(highscores[i].score_string,"<NEWLINE>","\n");
    }

    bool score_inserted=false;

    //Look through all of the highscores.
    for(int i=0;i<highscores.size();i++){
        //If the new score is higher than this highscore.
        if(score>highscores[i].score){
            highscores.insert(highscores.begin()+i,Highscore());
            highscores[i].score=score;
            highscores[i].score_string=score_string;
            score_inserted=true;
            break;
        }
    }

    if(!score_inserted){
        highscores.push_back(Highscore());
        highscores[highscores.size()-1].score=score;
        highscores[highscores.size()-1].score_string=score_string;
    }

    //Trim the highscores list to 10 entries.
    while(highscores.size()>10){
        highscores.erase(highscores.end());
    }

    for(int i=0;i<highscores.size();i++){
        replace_all(highscores[i].score_string,"\n","<NEWLINE>");
    }

    ofstream save("highscores",ifstream::out);

    if(save!=NULL){
        for(int i=0;i<highscores.size();i++){
            save<<"<highscore>\n";
            save<<"<score>";
            save<<highscores[i].score;
            save<<"\n<score string>";
            save<<highscores[i].score_string;
            save<<"\n</highscore>\n";
        }
    }
    else{
        fprintf(stderr,"Error saving highscores file.\n");
    }
}

void Player::save_game_log_entry(short cause_of_death,string killer,string killer_item,string killer_article,string killer_item_article){
    stringstream score_string("");

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

    score_string<<"name:"<<name<<"\n";
    score_string<<"version:"<<AutoVersion::MAJOR<<"."<<AutoVersion::MINOR<<"\n";
    score_string<<"score:"<<score<<"\n";
    score_string<<"dungeon level:"<<current_level+1<<"\n";
    score_string<<"highest dungeon level:"<<max_level+1<<"\n";
    score_string<<"experience level:"<<experience_level<<"\n";
    score_string<<"health:"<<return_health()<<"/"<<return_health_max()<<"\n";
    ///score_string<<"mana:"<<return_mana()<<"/"<<return_mana_max()<<"\n";
    score_string<<"deaths:"<<deaths<<"\n";
    score_string<<"game start:"<<start_date<<" at "<<start_time<<"\n";
    score_string<<"game end:"<<end_date<<" at "<<end_time<<"\n";
    score_string<<"class:"<<class_name<<"\n";
    score_string<<"race:"<<race_name<<"\n";

    /**if(gender==GENDER_MALE){
        score_string<<"gender:male"<<"\n";
    }
    else if(gender==GENDER_FEMALE){
        score_string<<"gender:female"<<"\n";
    }
    else if(gender==GENDER_NEUTRAL){
        score_string<<"gender:neutral"<<"\n";
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
    else if(cause_of_death==CAUSE_OF_DEATH_BURNED_BY_LIGHT){
        death_message="Burned up in the light.";
    }

    //If the player died.
    if(cause_of_death!=CAUSE_OF_DEATH_NONE){
        score_string<<death_message<<"\n";
    }
    //If the player escaped but did not win.
    else if(cause_of_death==CAUSE_OF_DEATH_NONE && !won){
        score_string<<"Escaped from the Lair."<<"\n";
    }
    //If the player escaped and won.
    else if(cause_of_death==CAUSE_OF_DEATH_NONE && won){
        score_string<<"Escaped from the Lair with "<<runestones<<" Runestones."<<"\n";
    }

    score_string<<"\n\n";

    //Only update the game log and highscores if this game was non-cheating.
    if(!flag_cheater){
        ofstream save_log("game_log",ifstream::app);

        if(save_log!=NULL){
            save_log<<score_string.str();

            save_log.close();
            save_log.clear();
        }
        else{
            fprintf(stderr,"Error saving game log entry.\n");
        }

        save_highscore(score_string.str());
    }
}
