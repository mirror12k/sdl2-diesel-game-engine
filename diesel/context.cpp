
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


SDL_Surface* drawing_context::load_surface(const char* filename)
{
    SDL_Surface* surf = IMG_Load(filename);
    if (surf == nullptr) {
        printf("image load error: %s\n", IMG_GetError());
        exit(1);
    } else {
        SDL_Surface* processed_surf = SDL_ConvertSurfaceFormat(surf, this->screen->format->format, 0);
        SDL_FreeSurface(surf);
         if (surf == nullptr) {
            printf("image conversion error\n");
            exit(1);
        } else {
            return processed_surf;
        }
    }
}


SDL_Texture* drawing_context::load_texture(const char* filename)
{
    SDL_Surface* surf = this->load_surface(filename);
    SDL_Texture* tex = this->surface_to_texture(surf);
    SDL_FreeSurface(surf);

    return tex;
}

SDL_Texture* drawing_context::surface_to_texture(SDL_Surface* surf)
{
    SDL_Texture* tex = SDL_CreateTextureFromSurface(this->renderer, surf);
    if (tex == nullptr) {
        printf("texture loading error\n");
        exit(1);
    }
    return tex;
}

SDL_Texture* drawing_context::get_texture(const char* filename)
{
    map<const char*, SDL_Texture*>::iterator result = this->loaded_textures.find(filename);
    if (result == this->loaded_textures.end()) {
        SDL_Texture* tex = this->load_texture(filename);
        this->loaded_textures[filename] = tex;
        return tex;
    } else {
        return result->second;
    }
}




void drawing_context::draw_texture(SDL_Texture* tex, SDL_Rect* dst)
{
    SDL_RenderCopy(this->renderer, tex, nullptr, dst);
}

void drawing_context::draw_sub_texture(SDL_Texture* tex, SDL_Rect* src, SDL_Rect* dst)
{
    SDL_RenderCopy(this->renderer, tex, src, dst);
}


void drawing_context::draw_sprite(named_sprite* sprite)
{
    if (sprite->texture == nullptr) {
        sprite->texture = this->get_texture(sprite->filename);
    }
    this->draw_sub_texture(sprite->texture, &sprite->sprite_rect, &sprite->rect);
}


}



