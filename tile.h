#ifndef tile_h
#define tile_h

#include "object.h"

#include <vector>

class Tile: public Object{
    private:
    public:
    Tile();

    //Render the tile to the screen.
    void render(std::vector< std::vector<bool> >* tile_rendered);

    //The actual physical type of the tile.
    //In other words, what the tile IS.
    short type;

    //The material that the tile is composed of.
    //In other words, what the tile is MADE OF.
    short material;
};

#endif
