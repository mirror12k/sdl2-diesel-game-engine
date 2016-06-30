


#include "graphic_entity.hpp"

namespace diesel
{


graphic_entity::graphic_entity(string filename, int x, int y)
: sprite(filename, x, y)
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


}
