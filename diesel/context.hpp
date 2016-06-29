
#pragma once



#include "entity.hpp"
#include "graphics.hpp"


namespace diesel
{


class update_context
{
private:
    entity_pool drawn_pool;
    entity_pool undrawn_pool;

public:

    void add_entity (entity* ent);
    void remove_entity (entity* ent);
    void remove_all();

    void update ();
    void draw (drawing_context* ctx);

    const uint8_t* get_keyboard_state();

};


}


