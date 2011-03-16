/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef message_log_h
#define message_log_h

#include "enumerations.h"

void update_text_log(const char *message,bool called_by_player,unsigned char message_type=MESSAGE_STANDARD);

#endif
