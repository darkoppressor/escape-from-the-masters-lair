/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "message_log.h"
#include "world.h"

#include <string>

using namespace std;

void update_text_log(const char *message,bool called_by_player,unsigned char message_type){
    //If the player called this function, we update the message log.
    //Otherwise, ignore this call.
    if(called_by_player){
        //Format the message according to its type.

        //System message.
        if(message_type==MESSAGE_SYSTEM){
            string temp_message="\x1F";
            temp_message+=message;
            message=temp_message.c_str();
        }

        //Print the message on this instance's screen.
        for(short i=0;i<player.text_limit;i++){
            if(i==player.text_limit-1){
                player.text_log[i]=message;
            }
            else{
                player.text_log[i]=player.text_log[i+1];
            }
        }

        //Don't log system messsages.
        if(message_type!=MESSAGE_SYSTEM){
            ofstream save_log("message_log.txt",ifstream::app);

            if(save_log!=NULL){
                time_t now;
                struct tm *tm_now;
                char buff[BUFSIZ];
                now=time(NULL);
                tm_now=localtime(&now);
                strftime(buff,sizeof buff,"[%Y-%m-%d %H:%M:%S] ",tm_now);

                save_log<<buff<<message<<"\n";

                save_log.close();
                save_log.clear();
            }
            else{
                fprintf(stderr,"Error logging message.\n");
            }
        }
    }
}
