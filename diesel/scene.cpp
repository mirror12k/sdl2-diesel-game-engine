

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
        this->load_entities(ctx);
    else if (this->entities_loaded && this->graphics_loaded)
        ctx->remove_entity(this);
}

void scene::draw(drawing_context* ctx)
{
    if (not this->graphics_loaded)
        this->load_graphics(ctx);
}





file_scene::file_scene(const string& graphics_filename, const string& entities_filename)
{
    this->graphics_filename = graphics_filename;
    this->entities_filename = entities_filename;
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
        vector<string> files = this->load_file(this->graphics_filename);

        // force load each texture
        for (vector<string>::const_iterator iter = files.begin(), iter_end = files.end(); iter != iter_end; iter++)
            if ((iter->length() > 0) && (iter->at(0) != '#'))
            {
                cout << "loading: " << *iter << endl;
                ctx->get_texture(*iter);
            }
    }

    this->set_graphics_loaded();
}


}



