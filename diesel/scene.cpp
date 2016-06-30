

#include "scene.hpp"

#include <iostream>
#include <fstream>

using std::ifstream;
using std::cout;
using std::endl;


namespace diesel
{


bool scene::is_drawn () const
{
    return true;
}


void scene::set_update_context(update_context* ctx)
{
    this->new_entities = ctx;
}
update_context* scene::get_update_context() const
{
    return this->new_entities;
}

bool scene::is_ready() const
{
    return this->graphics_loaded && this->entities_loaded;
}



// placeholder functions for when a scene doesn't require any loading
void scene::load_graphics(drawing_context* ctx)
{
    this->set_graphics_loaded();
}
void scene::load_entities(update_context* ctx)
{
    this->set_entities_loaded();
}



void scene::set_graphics_loaded(bool graphics_loaded)
{
    this->graphics_loaded = graphics_loaded;
}
void scene::set_entities_loaded(bool entities_loaded)
{
    this->entities_loaded = entities_loaded;
}

void scene::update(update_context* ctx)
{
    if (not this->entities_loaded)
        this->load_entities(this->new_entities);
    else if (this->entities_loaded && this->graphics_loaded)
        ctx->remove_entity(this);
}

void scene::draw(drawing_context* ctx)
{
    if (not this->graphics_loaded)
        this->load_graphics(ctx);
}





file_scene::file_scene(const string& graphics_filename, const string& entities_filename, dynamic_loader* loader)
{
    this->graphics_filename = graphics_filename;
    this->entities_filename = entities_filename;
    this->loader = loader;
}


vector<string> file_scene::load_file(const string& filename)
{
    ifstream stream;
    stream.open(filename);

    vector<string> result;

    string line;
    while (std::getline(stream, line))
    {
        result.push_back(line);
    }

    stream.close();

    return result;
}

void file_scene::load_graphics(drawing_context* ctx)
{
    if (this->graphics_filename.length() > 0)
    {
        vector<string> lines = this->load_file(this->graphics_filename);

        // force load each texture
        for (vector<string>::const_iterator iter = lines.begin(), iter_end = lines.end(); iter != iter_end; iter++)
            if ((iter->length() > 0) && (iter->at(0) != '#'))
            {
                cout << "loading: " << *iter << endl;
                ctx->get_texture(*iter);
            }
    }

    this->set_graphics_loaded();
}



void file_scene::load_entities(update_context* ctx)
{
    if (this->entities_filename.length() > 0)
    {
        if (this->loader == nullptr)
            throw dynamic_loading_exception(("attempt to dynamically load entities from ["+this->entities_filename+"] without loader").c_str());

        vector<string> lines = this->load_file(this->entities_filename);

        for (vector<string>::const_iterator iter = lines.begin(), iter_end = lines.end(); iter != iter_end; iter++)
            if ((iter->length() > 0) && (iter->at(0) != '#'))
            {
                ctx->add_entity(this->load_entity(*iter));
//                this->load_entity(*iter);
            }
    }

    this->set_entities_loaded();
}


entity* file_scene::load_entity(const string& entity_string)
{
    int offset = 0;
    int found = entity_string.find("(");
    if (found == string::npos)
        throw dynamic_loading_exception(("missing '(' token: ["+entity_string+"]").c_str());

    string classname = entity_string.substr(offset, found - offset);
    offset = found + 1;

    found = entity_string.find(")", offset);
    if (found == string::npos)
        throw dynamic_loading_exception(("missing ')' token: ["+entity_string+"]").c_str());

    string args_string = entity_string.substr(offset, found - offset);

    dynamic_value args = this->parse_args(args_string);

    cout << "classname: " << classname << ", args: " << args_string << endl;
//    for (dynamic_value::iterator iter = args.begin(), iter_end = args.end(); iter != iter_end; iter++)
//    {
//        cout << iter->first << " => " << iter->second.get_type() << endl;
//    }

    return this->loader->load(classname, args);
}

dynamic_value file_scene::parse_args(const string& args_string)
{
    dynamic_value args;

    int offset = 0;
    int found = args_string.find(",");
    while (found != string::npos)
    {
        while ((args_string.at(offset) == ' ') || (args_string.at(offset) == '\t')
                || (args_string.at(offset) == '\r') || (args_string.at(offset) == '\n'))
        {
            offset++;
        }
        this->parse_arg(args, args_string.substr(offset, found - offset));
        offset = found + 1;
        found = args_string.find(",", offset);
    }
    while ((args_string.at(offset) == ' ') || (args_string.at(offset) == '\t')
            || (args_string.at(offset) == '\r') || (args_string.at(offset) == '\n'))
    {
        offset++;
    }
    this->parse_arg(args, args_string.substr(offset));

    return args;
}


void file_scene::parse_arg(dynamic_value& args, const string& arg_string)
{
    int found = arg_string.find("=");
    if (found == string::npos)
        throw dynamic_loading_exception(("missing '=' token: ["+arg_string+"]").c_str());

    dynamic_value val = this->parse_val(arg_string.substr(found + 1));
    args.set(arg_string.substr(0, found), val);
}



dynamic_value file_scene::parse_val(const string& val_string)
{
    if ((val_string.at(0) == '"') || (val_string.at(0) == '\''))
    {
        return dynamic_value(val_string.substr(1, val_string.length() - 2));
    }
    else
    {
        return dynamic_value(std::stoi(val_string));
    }
}



}



