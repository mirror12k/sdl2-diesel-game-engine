
#pragma once

#include "graphics.hpp"
#include "entity.hpp"
#include "context.hpp"

#include "dynamic_loading.hpp"

namespace diesel
{



class graphic_entity : public dynamicly_loadable_entity
{
public:
    named_sprite sprite;

    graphic_entity(const string& filename, int width, int height, int tilex=1, int tiley=1);

    bool is_drawn () const;

    virtual void draw(drawing_context* ctx);

    void move(int deltax, int deltay);
    void move_to(int x, int y);
};


typedef struct {
    bool drawn;
    uint8_t tile_x, tile_y;
} tile_entry;

class tiled_entity : public graphic_entity
{
protected:
    tile_entry** tile_array;
    int array_x, array_y;
public:
    tiled_entity(const string& filename, int width, int height, int tilex, int tiley, int array_x, int array_y);
    ~tiled_entity();

    virtual void draw(drawing_context* ctx);

    void load_data(const string& data);

    void set_value(int x, int y, uint8_t tile_x, uint8_t tile_y, bool drawn=true);
};



}


