
#include "graphics.hpp"


namespace diesel
{


named_sprite::named_sprite(const string& filename, int w, int h, int tiles_x, int tiles_y)
{
    this->filename = filename;
    this->sprite_rect.w = w;
    this->sprite_rect.h = h;
    this->sprite_rect.x = 0;
    this->sprite_rect.y = 0;

    this->rect.w = w;
    this->rect.h = h;
    this->rect.x = 0;
    this->rect.y = 0;

    this->tiles_x = tiles_x;
    this->tiles_y = tiles_y;
}

void named_sprite::step_frame()
{
    if (++this->current_frame_x == this->tiles_x) {
        this->current_frame_x = 0;
        this->sprite_rect.x = 0;
        if (++this->current_frame_y == this->tiles_y)
        {
            this->current_frame_y = 0;
            this->sprite_rect.y = 0;
        } else {
            this->sprite_rect.y += this->sprite_rect.h;
        }
    } else {
        this->sprite_rect.x += this->sprite_rect.w;
    }
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
//    SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
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


SDL_Surface* drawing_context::load_surface(const string& filename)
{
    SDL_Surface* surf = IMG_Load(filename.c_str());
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


SDL_Texture* drawing_context::load_texture(const string& filename)
{
    SDL_Texture* tex = IMG_LoadTexture(this->renderer, filename.c_str());
//    SDL_Surface* surf = this->load_surface(filename);
//    SDL_Texture* tex = this->surface_to_texture(surf);
//    SDL_FreeSurface(surf);

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

SDL_Texture* drawing_context::get_texture(const string& filename)
{
    map<string, SDL_Texture*>::iterator result = this->loaded_textures.find(filename);
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
//    SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
//    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
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



void drawing_context::draw_sprite_tile(named_sprite* sprite, SDL_Rect* dest, int tile_x, int tile_y)
{
    SDL_Rect rect = sprite->sprite_rect;
    rect.x = rect.w * tile_x;
    rect.y = rect.h * tile_y;

    this->draw_sub_texture(sprite->texture, &rect, dest);
}





}



