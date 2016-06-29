
#pragma once

#include "context.hpp"
#include "graphics.hpp"



namespace diesel
{



class scene
{
public:
    virtual void load_graphics(drawing_context* ctx);
    virtual void load_scene(update_context* ctx);
};


class file_scene
{
public:
    file_scene(const char* graphics_filename, const char* entities_filename);

};




}




