
#include "graphics.hpp"


namespace diesel
{


named_sprite::named_sprite(const char* filename, int w, int h, int tiles_x, int tiles_y)
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
        if (++this->current_frame_y == this->tiles_y)
            this->current_frame_y = 0;
    }
}

}



