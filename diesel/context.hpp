

#include "entity.hpp"
#include "graphics.hpp"

#include <map>
using std::map;


namespace diesel
{


class update_context
{
private:
    entity_pool drawn_pool;
    entity_pool undrawn_pool;

public:

    void add_entity (entity* ent);
    void remove_entity (entity* ent);

    void update ();
    void draw (drawing_context* ctx);

};


class drawing_context
{
private:
    SDL_Window* window;
    SDL_Surface* screen;
    SDL_Renderer* renderer;

    char* window_title;
    int window_width, window_height;

    map<const char*, SDL_Texture*> loaded_textures;

public:
    drawing_context(char* window_title, int window_width, int window_height);
    ~drawing_context();

    void start_graphics();
    void end_graphics();

    void clear();
    void present();

    SDL_Surface* load_surface(const char* filename);
    SDL_Texture* load_texture(const char* filename);
    SDL_Texture* surface_to_texture(SDL_Surface* surf);

    SDL_Texture* get_texture(const char* filename);

    void draw_texture(SDL_Texture* tex, SDL_Rect* dst);
    void draw_sub_texture(SDL_Texture* tex, SDL_Rect* src, SDL_Rect* dst);

    void draw_sprite(named_sprite* sprite);

};


}


