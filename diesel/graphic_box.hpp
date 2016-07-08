
#pragma once

#include "graphic_entity.hpp"

namespace diesel
{



class graphic_box : public graphic_entity
{
protected:
    int box_width, box_height;
public:
    graphic_box(const string& filename, int width, int height, int box_width, int box_height, int tilex=3, int tiley=3);

    virtual void draw(drawing_context* ctx);
};



}




