
#pragma once

#include "entity.hpp"
#include "context.hpp"
#include "graphics.hpp"
#include "dynamic_loading.hpp"

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

    update_context* new_entities;

public:

    bool is_drawn () const;

    void set_update_context(update_context* ctx);
    update_context* get_update_context() const;

    bool is_ready() const;

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

    dynamic_loader* loader;
public:
    file_scene(const string& graphics_filename, const string& entities_filename="", dynamic_loader* loader=nullptr);

    vector<string> load_file(const string& filename);

    virtual void load_graphics(drawing_context* ctx);
    virtual void load_entities(update_context* ctx);

    entity* load_entity(const string& entity_string);
    dynamic_object_value parse_args(const string& args_string);
    void parse_arg(dynamic_object_value& args, const string& arg_string);
    dynamic_value parse_val(const string& val_string);
};




}




