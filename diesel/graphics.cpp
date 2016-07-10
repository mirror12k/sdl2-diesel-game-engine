
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

void named_sprite::set_frame(int frame)
{
    this->current_frame_x = frame % this->tiles_x;
    this->current_frame_y = frame / this->tiles_x;

    this->sprite_rect.x = this->sprite_rect.w * this->current_frame_x;
    this->sprite_rect.y = this->sprite_rect.h * this->current_frame_y;
}







named_font::named_font(const string& filename)
: filename(filename)
{}





int reference_counted::reference()
{
    return ++this->reference_count;
}

int reference_counted::unreference()
{
    return --this->reference_count;
}





texture_reference::texture_reference(SDL_Texture* texture, const SDL_Rect& sprite_rect)
: texture(texture), sprite_rect(sprite_rect)
{}



referenced_sprite::referenced_sprite(const referenced_sprite& other)
{
    if (this->source)
        this->unreference();
    this->reference(other.source);
}

referenced_sprite::referenced_sprite()
{}
referenced_sprite::referenced_sprite(texture_reference* ref)
{
    this->reference(ref);
}

referenced_sprite::~referenced_sprite()
{
    if (this->source)
        this->unreference();
}
referenced_sprite& referenced_sprite::operator=(const referenced_sprite& other)
{
    if (this->source)
        this->unreference();
    this->reference(other.source);
    return *this;
}



void referenced_sprite::reference(texture_reference* source)
{
    if (this->source)
        this->unreference();

    this->source = source;
    this->texture = source->texture;
    this->sprite_rect = source->sprite_rect;
    this->rect = source->sprite_rect;
}

void referenced_sprite::unreference()
{
    this->source->unreference();
    this->source = nullptr;
    this->texture = nullptr;
}


rendered_text_sprite::rendered_text_sprite(const string& font_filename, const string& text)
: font(font_filename), text(text)
{}

void rendered_text_sprite::set_text(const string& text)
{
    this->text = text;
    if (this->source)
        this->unreference();
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


    if (TTF_Init())
    {
        printf("failed to initialize SDL2 TTF: %s\n", TTF_GetError());
        exit(1);
    }
}

void drawing_context::end_graphics()
{
    TTF_Quit();

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

    if (tex == nullptr)
    {
        printf("image load error: %s\n", IMG_GetError());
        exit(1);
    }

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





void drawing_context::set_texture_alpha(SDL_Texture* tex, uint8_t alpha)
{
    if (SDL_SetTextureAlphaMod(tex, alpha) != 0)
    {
        printf("error setting sprite alpha: %s\n", SDL_GetError());
        exit(1);
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




void drawing_context::load_named_sprite(named_sprite* sprite)
{
    if (sprite->texture)
        return;
    else
        sprite->texture = this->get_texture(sprite->filename);
}




void drawing_context::draw_sprite(graphic_sprite* sprite)
{
    this->draw_sub_texture(sprite->texture, &sprite->sprite_rect, &sprite->rect);
}

void drawing_context::draw_sprite_alpha(graphic_sprite* sprite, uint8_t alpha)
{
    this->set_texture_alpha(sprite->texture, alpha);
    this->draw_sub_texture(sprite->texture, &sprite->sprite_rect, &sprite->rect);
    this->set_texture_alpha(sprite->texture, 255);
}


void drawing_context::draw_sprite_offset(graphic_sprite* sprite, int offsetx, int offsety)
{
    SDL_Rect dest = sprite->rect;
    dest.x += offsetx;
    dest.y += offsety;
    this->draw_sub_texture(sprite->texture, &sprite->sprite_rect, &dest);
}

void drawing_context::draw_sprite_rect(graphic_sprite* sprite, int offsetx, int offsety)
{
    SDL_Rect dest;
    dest.x = sprite->rect.x + offsetx;
    dest.y = sprite->rect.y + offsety;
    dest.w = sprite->sprite_rect.w;
    dest.h = sprite->sprite_rect.h;

    this->draw_sub_texture(sprite->texture, &sprite->sprite_rect, &dest);
}


void drawing_context::draw_sprite_tile(graphic_sprite* sprite, SDL_Rect* dest, int tile_x, int tile_y)
{
    SDL_Rect rect = sprite->sprite_rect;
    rect.x = rect.w * tile_x;
    rect.y = rect.h * tile_y;

    this->draw_sub_texture(sprite->texture, &rect, dest);
}





TTF_Font* drawing_context::load_ttf_font(const string& filename)
{
    TTF_Font* font = TTF_OpenFont(filename.c_str(), filename.size());
    if (font == nullptr)
    {
        printf("error loading ttf font '%s': %s\n", filename.c_str(), TTF_GetError());
        exit(1);
    }
    return font;
}

TTF_Font* drawing_context::get_font(const string& filename)
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


void drawing_context::load_named_font(named_font* font)
{
    if (font->font)
        return;
    else
        font->font = this->get_font(font->filename);
}

void drawing_context::load_rendered_text_sprite(rendered_text_sprite* sprite)
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



void drawing_context::render_font_text(referenced_sprite* sprite, named_font* font, const string& text, const SDL_Color& color)
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



texture_reference* drawing_context::create_texture_reference(SDL_Texture* tex, const SDL_Rect& rect)
{
    texture_reference* ref = new texture_reference(tex, rect);
    this->referenced_textures.push_back(ref);
    return ref;
}



}



