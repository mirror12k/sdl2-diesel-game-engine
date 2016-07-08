


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


void graphic_entity::move_to(int x, int y)
{
    this->sprite.rect.x = x;
    this->sprite.rect.y = y;
}








tiled_entity::tiled_entity(const string& filename, int width, int height, int tilex, int tiley, int array_x, int array_y)
: graphic_entity(filename, width, height, tilex, tiley), array_x(array_x), array_y(array_y)
{
    this->tile_array = new tile_entry* [array_y];
    for (int i = 0; i < array_y; i++)
        this->tile_array[i] = new tile_entry [array_x];
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


void tiled_entity::load_data(const string& data)
{
    const uint8_t* source = (const uint8_t*)data.data();
    for (int y = 0; y < this->array_y; y++)
        for (int x = 0; x < this->array_x; x++)
            if ((source[(y * this->array_x + x) * 2] == 0) && (source[(y * this->array_x + x) * 2 + 1] == 0))
            {
                this->tile_array[y][x].drawn = false;
            }
            else
            {
                this->tile_array[y][x].tile_x = source[(y * this->array_x + x) * 2];
                this->tile_array[y][x].tile_y = source[(y * this->array_x + x) * 2 + 1];
                this->tile_array[y][x].drawn = true;
            }
}


void tiled_entity::set_value(int x, int y, uint8_t tile_x, uint8_t tile_y, bool drawn)
{
    if ((x >= this->array_x) || (y >= this->array_y))
        throw "error out of bounds";
    this->tile_array[y][x].tile_x = tile_x;
    this->tile_array[y][x].tile_y = tile_y;
    this->tile_array[y][x].drawn = drawn;
}


}
