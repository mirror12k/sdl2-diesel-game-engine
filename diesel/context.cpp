
#include "context.hpp"


namespace diesel
{


void update_context::add_entity(entity* ent)
{
    if (ent->is_drawn())
    {
        printf("drawn pool\n");
        this->drawn_pool.add_entity(ent);
    }
    else
    {
        printf("undrawn pool\n");
        this->undrawn_pool.add_entity(ent);
    }
}


void update_context::remove_entity(entity* ent)
{
    if (ent->is_drawn())
        this->drawn_pool.remove_entity(ent);
    else
        this->undrawn_pool.remove_entity(ent);
}


void update_context::update ()
{
    this->undrawn_pool.update(this);
    this->drawn_pool.update(this);
}

void update_context::draw (drawing_context* ctx)
{
    this->drawn_pool.draw(ctx);
}




drawing_context::drawing_context(char* window_title, int window_width, int window_height)
: window_title(window_title), window_width(window_width), window_height(window_height)
{}

drawing_context::~drawing_context()
{
    if (this->window != nullptr)
        this->end_graphics();
}


void drawing_context::start_graphics()
{
    this->window = SDL_CreateWindow(this->window_title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        this->window_width, this->window_height,
        SDL_WINDOW_OPENGL);


    if (this->window == nullptr)
    {
        printf("failed to create window\n");
        exit(1);
    }

    this->screen = SDL_GetWindowSurface(this->window);
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
}

void drawing_context::end_graphics()
{
    SDL_DestroyWindow(this->window);
    this->window = nullptr;
    this->screen = nullptr;
    this->renderer = nullptr;
}


void drawing_context::clear()
{
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(this->renderer, NULL);
}

void drawing_context::present()
{
    SDL_RenderPresent(this->renderer);
}

}



