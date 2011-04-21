/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "message_log.h"
#include "world.h"

#include <string>

using namespace std;

void update_text_log(const char *message,bool display,unsigned char message_type){
    //If we want to display this message, we update the message log.
    //Otherwise, ignore this call.
    if(display){
        //Format the message according to its type.

        //System message.
        if(message_type==MESSAGE_SYSTEM){
            string temp_message="\x1F";
            temp_message+=message;
            message=temp_message.c_str();
        }

        //Forget the oldest string.
        player.text_log.erase(player.text_log.begin());

        //Remember this string.
        player.text_log.push_back(message);

        //Reset the text log display position to the top.
        player.text_log_display_position=player.text_log.size()-1;

        //Don't log system messsages.
        if(message_type!=MESSAGE_SYSTEM){
            ///This is commented out for the release version.
            /**ofstream save_log("message_log.txt",ifstream::app);

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
            }*/
        }
    }
}
