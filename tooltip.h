#ifndef tooltip_h
#define tooltip_h

#include <SDL.h>

#include <string>

class Tooltip{
    private:
    public:
    //The constructor takes starting coordinates.
    Tooltip();

    void set_dimensions();

    //
    void render();

    bool on;

    std::string message;

    //
    short x,y;

    //
    short w,h;
};

#endif
