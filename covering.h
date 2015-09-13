/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef covering_h
#define covering_h

class Covering{
    private:
    public:

    //The type of covering.
    short type;

    //The age of this covering.
    int age;

    Covering(short get_type);

    //Handle anything that needs handling on each game turn.
    //Returns true if the covering is still in existence.
    //Returns false if the covering is no longer in existence.
    bool process_turn();
};

#endif
