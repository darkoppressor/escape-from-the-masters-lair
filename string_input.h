/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef string_input_h
#define string_input_h

#include <string>
#include <SDL.h>

#include <boost/algorithm/string.hpp>

class string_input{
    private:
    public:
    //Storage string.
    std::string str1;

    string_input();

    ~string_input();

    void handle_events();
};

#endif
