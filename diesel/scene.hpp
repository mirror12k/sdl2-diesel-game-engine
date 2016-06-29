
#pragma once

#include "context.hpp"
#include "graphics.hpp"

#include <string>
using std::string;
#include <vector>
using std::vector;



namespace diesel
{



class scene
{
public:
    virtual void load_graphics(drawing_context* ctx);
    virtual void load_entities(update_context* ctx);
};


class file_scene : public scene
{
private:
    string graphics_filename;
    string entities_filename;
public:
    file_scene(const string& graphics_filename, const string& entities_filename);

    vector<string> load_file(const string& filename);

    virtual void load_graphics(drawing_context* ctx);
};




}




