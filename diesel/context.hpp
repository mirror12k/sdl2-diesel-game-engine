

#include "entity.hpp"

#include <SDL2/SDL.h>


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

public:
    drawing_context(char* window_title, int window_width, int window_height);
    ~drawing_context();

    void start_graphics();
    void end_graphics();

    void clear();
    void present();

};


}


