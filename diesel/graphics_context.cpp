
#include "graphics_context.hpp"


namespace diesel
{



graphics_exception::graphics_exception(const string& reason)
: generic_exception(reason)
{}





graphics_context::graphics_context(const string& window_title, int window_width, int window_height)
: window_title(window_title), window_width(window_width), window_height(window_height)
{}

graphics_context::~graphics_context()
{
    if (this->window != nullptr)
        this->end_graphics();
}


void graphics_context::start_graphics()
{
    this->window = SDL_CreateWindow(this->window_title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        this->window_width, this->window_height,
        SDL_WINDOW_OPENGL);


    if (this->window == nullptr)
    {
        throw graphics_exception("failed to create window: "+string(SDL_GetError()));
    }

    this->screen = SDL_GetWindowSurface(this->window);
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
//    SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);


    if (TTF_Init())
    {
        throw graphics_exception("failed to initialize SDL2 TTF: "+string(TTF_GetError()));
    }
}

void graphics_context::end_graphics()
{
    TTF_Quit();

    SDL_DestroyWindow(this->window);
    this->window = nullptr;
    this->screen = nullptr;
    this->renderer = nullptr;
}


void graphics_context::clear()
{
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(this->renderer, NULL);
}

void graphics_context::present()
{
    SDL_RenderPresent(this->renderer);
}


SDL_Surface* graphics_context::load_surface(const string& filename)
{
    SDL_Surface* surf = IMG_Load(filename.c_str());
    if (surf == nullptr) {
        throw graphics_exception("image load error: "+string(IMG_GetError()));
    } else {
        SDL_Surface* processed_surf = SDL_ConvertSurfaceFormat(surf, this->screen->format->format, 0);
        SDL_FreeSurface(surf);
         if (surf == nullptr) {
            throw graphics_exception("image conversion error: "+string(SDL_GetError()));
        } else {
            return processed_surf;
        }
    }
}


SDL_Texture* graphics_context::load_texture(const string& filename)
{
    SDL_Texture* tex = IMG_LoadTexture(this->renderer, filename.c_str());
//    SDL_Surface* surf = this->load_surface(filename);
//    SDL_Texture* tex = this->surface_to_texture(surf);
//    SDL_FreeSurface(surf);

    if (tex == nullptr)
        throw graphics_exception("image load error: "+string(IMG_GetError()));

    return tex;
}

SDL_Texture* graphics_context::surface_to_texture(SDL_Surface* surf)
{
    SDL_Texture* tex = SDL_CreateTextureFromSurface(this->renderer, surf);
    if (tex == nullptr) {
        throw graphics_exception("surface to texture error: "+string(SDL_GetError()));
    }
    return tex;
}

SDL_Texture* graphics_context::get_texture(const string& filename)
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





void graphics_context::set_texture_alpha(SDL_Texture* tex, uint8_t alpha)
{
    if (SDL_SetTextureAlphaMod(tex, alpha) != 0)
    {
        throw graphics_exception("error setting sprite alpha: "+string(SDL_GetError()));
    }
}



void graphics_context::draw_texture(SDL_Texture* tex, SDL_Rect* dst)
{
//    SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
//    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(this->renderer, tex, nullptr, dst);
}

void graphics_context::draw_sub_texture(SDL_Texture* tex, SDL_Rect* src, SDL_Rect* dst)
{
    SDL_RenderCopy(this->renderer, tex, src, dst);
}




void graphics_context::load_named_sprite(named_sprite* sprite)
{
    if (sprite->texture)
        return;
    else
        sprite->texture = this->get_texture(sprite->filename);
}




void graphics_context::draw_sprite(graphic_sprite* sprite)
{
    this->draw_sub_texture(sprite->texture, &sprite->sprite_rect, &sprite->rect);
}

void graphics_context::draw_sprite_alpha(graphic_sprite* sprite, uint8_t alpha)
{
    this->set_texture_alpha(sprite->texture, alpha);
    this->draw_sub_texture(sprite->texture, &sprite->sprite_rect, &sprite->rect);
    this->set_texture_alpha(sprite->texture, 255);
}


void graphics_context::draw_sprite_offset(graphic_sprite* sprite, int offsetx, int offsety)
{
    SDL_Rect dest = sprite->rect;
    dest.x += offsetx;
    dest.y += offsety;
    this->draw_sub_texture(sprite->texture, &sprite->sprite_rect, &dest);
}

void graphics_context::draw_sprite_rect(graphic_sprite* sprite, int offsetx, int offsety)
{
    SDL_Rect dest;
    dest.x = sprite->rect.x + offsetx;
    dest.y = sprite->rect.y + offsety;
    dest.w = sprite->sprite_rect.w;
    dest.h = sprite->sprite_rect.h;

    this->draw_sub_texture(sprite->texture, &sprite->sprite_rect, &dest);
}


void graphics_context::draw_sprite_tile(graphic_sprite* sprite, SDL_Rect* dest, int tile_x, int tile_y)
{
    SDL_Rect rect = sprite->sprite_rect;
    rect.x = rect.w * tile_x;
    rect.y = rect.h * tile_y;

    this->draw_sub_texture(sprite->texture, &rect, dest);
}





TTF_Font* graphics_context::load_ttf_font(const string& filename)
{
    TTF_Font* font = TTF_OpenFont(filename.c_str(), filename.size());
    if (font == nullptr)
    {
        throw graphics_exception("error loading ttf font '"+filename+"': "+string(TTF_GetError()));
    }
    return font;
}

TTF_Font* graphics_context::get_font(const string& filename)
{
    map<string, TTF_Font*>::iterator result = this->loaded_fonts.find(filename);
    if (result == this->loaded_fonts.end())
    {
        TTF_Font* font = this->load_ttf_font(filename);
        this->loaded_fonts[filename] = font;
        return font;
    }
    else
    {
        return result->second;
    }
}


void graphics_context::load_named_font(named_font* font)
{
    if (font->font)
        return;
    else
        font->font = this->get_font(font->filename);
}

void graphics_context::load_rendered_text_sprite(rendered_text_sprite* sprite)
{
    this->load_named_font(&sprite->font);

    if (sprite->texture)
    {
        return;
    }
    else
    {
        this->render_font_text(sprite, &sprite->font, sprite->text);
    }
}



void graphics_context::render_font_text(referenced_sprite* sprite, named_font* font, const string& text, const SDL_Color& color)
{
    this->load_named_font(font);

    SDL_Surface* surf = TTF_RenderText_Blended(font->font, text.c_str(), color);
    SDL_Texture* tex = this->surface_to_texture(surf);

    SDL_Rect sprite_rect;
    sprite_rect.x = 0;
    sprite_rect.y = 0;
    sprite_rect.w = surf->w;
    sprite_rect.h = surf->h;

    SDL_FreeSurface(surf);

    texture_reference* ref = this->create_texture_reference(tex, sprite_rect);
    sprite->reference(ref);
}



texture_reference* graphics_context::create_texture_reference(SDL_Texture* tex, const SDL_Rect& rect)
{
    texture_reference* ref = new texture_reference(tex, rect);
    this->referenced_textures.push_back(ref);
    return ref;
}


int graphics_context::garbage_collect_texture_references()
{
    int garbage_collected = 0;

    for (list<texture_reference*>::iterator iter = this->referenced_textures.begin(), iter_end = this->referenced_textures.end();
            iter != iter_end;)
    {
        if ((*iter)->reference_count == 0)
        {
            SDL_DestroyTexture((*iter)->texture);
            delete *iter;
            iter = this->referenced_textures.erase(iter);
            garbage_collected++;
        }
        else
        {
             iter++;
        }
    }

    return garbage_collected;
}



}



