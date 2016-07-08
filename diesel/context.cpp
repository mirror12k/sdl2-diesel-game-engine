
#include "context.hpp"


namespace diesel
{


void update_context::add_entity(entity* ent)
{
    if (service* srv = dynamic_cast<service*>(ent))
        this->active_services[srv->get_service_name()] = srv;

    if (ent->is_drawn())
    {
        this->drawn_pool.add_entity(ent);
    }
    else
    {
        this->undrawn_pool.add_entity(ent);
    }
}


void update_context::remove_entity(entity* ent)
{
    if (service* srv = dynamic_cast<service*>(ent))
        this->active_services.erase(this->active_services.find(srv->get_service_name()));

    if (ent->is_drawn())
        this->drawn_pool.remove_entity(ent);
    else
        this->undrawn_pool.remove_entity(ent);
}

void update_context::remove_all()
{
    this->drawn_pool.remove_all();
    this->undrawn_pool.remove_all();
}


void update_context::update ()
{
    this->undrawn_pool.update(this);
    this->drawn_pool.update(this);
}

void update_context::draw (drawing_context* ctx)
{
    this->drawn_pool.draw(ctx);
}


const uint8_t* update_context::get_keyboard_state()
{
    return SDL_GetKeyboardState(NULL);
}



}



