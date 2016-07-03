


#include "graphic_entity.hpp"

namespace diesel
{


graphic_entity::graphic_entity(const string& filename, int width, int height, int tilex, int tiley)
: sprite(filename, width, height, tilex, tiley)
{}



bool graphic_entity::is_drawn () const
{
    return true;
}


void graphic_entity::draw(drawing_context* ctx)
{
    ctx->draw_sprite(&this->sprite);
}


void graphic_entity::move(int deltax, int deltay)
{
    this->sprite.rect.x += deltax;
    this->sprite.rect.y += deltay;
}









tiled_entity::tiled_entity(const string& filename, int width, int height, int tilex, int tiley, int array_x, int array_y)
: graphic_entity(filename, width, height, tilex, tiley), array_x(array_x), array_y(array_y)
{
    this->tile_array = new tile_entry* [array_y];

}

tiled_entity::~tiled_entity()
{
    delete this->tile_array;
}

void tiled_entity::draw(drawing_context* ctx)
{
    SDL_Rect dest = this->sprite.sprite_rect;
    for (int y = 0; y < this->array_y; y++)
        for (int x = 0; x < this->array_x; x++)
            if (this->tile_array[y][x].drawn)
            {
                dest.x = x * dest.w;
                dest.y = y * dest.h;
                ctx->draw_sprite_tile(&this->sprite, &dest, this->tile_array[y][x].tile_x, this->tile_array[y][x].tile_y);
            }
}

}
