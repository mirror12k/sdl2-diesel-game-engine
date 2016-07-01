
#pragma once


#include "entity.hpp"
#include "dynamic_loading.hpp"


namespace diesel
{


class service : public dynamicly_loadable_entity
{
private:
    string service_name;

public:
    service(const string& service_name);
    string get_service_name() const;
};






template <class entity_type>
class entity_service : public service
{
protected:
    list<entity_type*> registered_entites;

public:
    entity_service(const string& service_name);

    void register_entity(entity_type* ent);
    void unregister_entity(entity_type* ent);
};



template<class entity_type>
entity_service<entity_type>::entity_service(const string& service_name)
: service(service_name)
{}

template<class entity_type>
void entity_service<entity_type>::register_entity(entity_type* ent)
{
    this->registered_entites.push_back(ent);
}
// must remove on-demand because there is no promise that the entity will remain later
template<class entity_type>
void entity_service<entity_type>::unregister_entity(entity_type* ent)
{
    for (typename list<entity_type*>::iterator iter = this->registered_entites.begin(), iter_end = this->registered_entites.end();
            iter != iter_end; iter++)
        if (*iter == ent)
        {
            this->registered_entites.erase(iter);
            break;
        }
}




template <class entity_type>
class iterating_entity_service : public entity_service<entity_type>
{
public:
    iterating_entity_service(const string& service_name);

    virtual void update(update_context* ctx);
    virtual void process_entity(update_context* ctx, entity_type* ent) = 0;
};


template<class entity_type>
iterating_entity_service<entity_type>::iterating_entity_service(const string& service_name)
: entity_service<entity_type>(service_name)
{}



template<class entity_type>
void iterating_entity_service<entity_type>::update(update_context* ctx)
{
    for (typename list<entity_type*>::iterator iter = this->registered_entites.begin(), iter_end = this->registered_entites.end();
            iter != iter_end; iter++)
        this->process_entity(ctx, *iter);
}





}


