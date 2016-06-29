
#pragma once

#include "entity.hpp"
#include "context.hpp"
#include "graphics.hpp"

#include <string>
using std::string;
#include <vector>
using std::vector;



namespace diesel
{



class scene : public entity
{
private:
    bool graphics_loaded = false;
    bool entities_loaded = false;
public:

    bool is_drawn () const;

    virtual void load_graphics(drawing_context* ctx);
    virtual void load_entities(update_context* ctx);

    void set_graphics_loaded(bool graphics_loaded=true);
    void set_entities_loaded(bool entities_loaded=true);

    virtual void update(update_context* ctx);
    virtual void draw(drawing_context* ctx);

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




