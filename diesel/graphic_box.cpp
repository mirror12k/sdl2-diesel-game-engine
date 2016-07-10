
#include "graphic_box.hpp"

namespace diesel
{



graphic_box::graphic_box(const string& filename, int width, int height, int box_width, int box_height, int tilex, int tiley)
: graphic_entity(filename, width, height, tilex, tiley),
box_width(box_width), box_height(box_height)
{}


void graphic_box::draw(graphics_context* ctx)
{
    ctx->load_named_sprite(&this->sprite);

    SDL_Rect dest = this->sprite.rect;
    ctx->draw_sprite_tile(&this->sprite, &dest, 0, 0);
    dest.y = this->sprite.rect.y + (this->box_height - 1) * dest.h;
    ctx->draw_sprite_tile(&this->sprite, &dest, 0, 2);
    dest.x = this->sprite.rect.x + (this->box_width - 1) * dest.w;
    ctx->draw_sprite_tile(&this->sprite, &dest, 2, 2);
    dest.y = this->sprite.rect.y;
    ctx->draw_sprite_tile(&this->sprite, &dest, 2, 0);


    dest.y = this->sprite.rect.y;
    for (int x = 1; x < this->box_width - 1; x++)
    {
        dest.x = this->sprite.rect.x + x * dest.w;
        ctx->draw_sprite_tile(&this->sprite, &dest, 1, 0);
    }

    dest.y = this->sprite.rect.y + (this->box_height - 1) * dest.h;
    for (int x = 1; x < this->box_width - 1; x++)
    {
        dest.x = this->sprite.rect.x + x * dest.w;
        ctx->draw_sprite_tile(&this->sprite, &dest, 1, 2);
    }

    dest.x = this->sprite.rect.x;
    for (int y = 1; y < this->box_height - 1; y++)
    {
        dest.y = this->sprite.rect.y + y * dest.h;
        ctx->draw_sprite_tile(&this->sprite, &dest, 0, 1);
    }

    dest.x = this->sprite.rect.x + (this->box_width - 1) * dest.w;
    for (int y = 1; y < this->box_height - 1; y++)
    {
        dest.y = this->sprite.rect.y + y * dest.h;
        ctx->draw_sprite_tile(&this->sprite, &dest, 2, 1);
    }


    for (int x = 1; x < this->box_width - 1; x++)
    {
        for (int y = 1; y < this->box_height - 1; y++)
        {
            dest.x = this->sprite.rect.x + x * dest.w;
            dest.y = this->sprite.rect.y + y * dest.h;
            ctx->draw_sprite_tile(&this->sprite, &dest, 1, 1);
        }
    }
}



}
