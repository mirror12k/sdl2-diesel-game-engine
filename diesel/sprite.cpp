
#include "sprite.hpp"



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






}


