

#include "scene.hpp"

#include <iostream>
#include <fstream>

using std::ifstream;
using std::cout;
using std::endl;


namespace diesel
{


// placeholder functions for when a scene doesn't require any loading
void scene::load_graphics(drawing_context* ctx)
{}
void scene::load_entities(update_context* ctx)
{}




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
        cout << "got line: " << line << endl;
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
}


}



