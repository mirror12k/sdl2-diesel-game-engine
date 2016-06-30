
#pragma once



#include "entity.hpp"
#include "graphics.hpp"
#include "service.hpp"


#include <string>
using std::string;

#include <map>
using std::map;


namespace diesel
{


class update_context
{
private:
    entity_pool drawn_pool;
    entity_pool undrawn_pool;

    map <string, service*> active_services;

public:

    void add_entity (entity* ent);
    void remove_entity (entity* ent);
    void remove_all();

    void update ();
    void draw (drawing_context* ctx);

    const uint8_t* get_keyboard_state();

    template <class service_type>
    service_type* get_service(const string& service_name);

};



template <class service_type>
service_type* update_context::get_service(const string& service_name)
{
    return (service_type*)this->active_services[service_name];
}


}


