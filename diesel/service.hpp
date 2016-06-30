
#pragma once


#include "entity.hpp"
#include "dynamic_loading.hpp"


namespace diesel
{


template <class entity_type>
class service : public dynamicly_loadable_entity
{
private:
    list<entity_type*> registered_entites;
    string service_name;
    bool update_entities;

public:
    service(const string& service_name, bool update_entities=true);

    string get_service_name() const;

    void register_entity(entity_type* ent);
    void unregister_entity(entity_type* ent);

    void update(update_context* ctx);

    virtual void service_entity(update_context* ctx, entity_type* ent) = 0;
};





template<class entity_type>
service<entity_type>::service(const string& service_name, bool update_entities)
: service_name(service_name), update_entities(update_entities)
{}


template<class entity_type>
string service<entity_type>::get_service_name() const
{
    return this->service_name;
}

template<class entity_type>
void service<entity_type>::register_entity(entity_type* ent)
{
    this->registered_entites.push_back(ent);
}


// must remove on-demand because there is no promise that the entity will remain later
template<class entity_type>
void service<entity_type>::unregister_entity(entity_type* ent)
{
    for (typename list<entity_type*>::iterator iter = this->registered_entites.begin(), iter_end = this->registered_entites.end();
            iter != iter_end; iter++)
        if (*iter == ent)
        {
            this->registered_entites.erase(iter);
            break;
        }
}



template<class entity_type>
void service<entity_type>::update(update_context* ctx)
{
    if (this->update_entities)
        for (typename list<entity_type*>::iterator iter = this->registered_entites.begin(), iter_end = this->registered_entites.end();
                iter != iter_end; iter++)
            this->service_entity(ctx, *iter);
}





}


