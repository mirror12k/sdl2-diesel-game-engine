
#pragma once



#include <list>
using std::list;



#include <stdio.h>



namespace diesel
{

class update_context;
class drawing_context;


class entity
{
public:
    virtual ~entity();

    virtual bool is_drawn () const;

    virtual void update (update_context* ctx);
    virtual void draw (drawing_context* ctx);


    virtual void on_added(update_context* ctx);
    virtual void on_removed(update_context* ctx);
};




class entity_pool : public entity
{
private:
    list<entity*> entity_pool;
    list<entity*> to_remove_pool;
    list<entity*> to_add_pool;

public:
    virtual ~entity_pool();


    void update (update_context* ctx);
    void draw (drawing_context* ctx);

    // removes any entities from the pool which are scheduled for removal
    // called automatically at the start of update()
    void update_removals (update_context* ctx);

    // adds a new entity to the entity pool
    void add_entity (entity* ent);
    // marks an entity for removal and eventual deletion
    // safe (but expensive) to remove same entity multiple times
    void remove_entity (entity* ent);
    // immediately removes and deletes all entities
    void remove_all ();
};


}


