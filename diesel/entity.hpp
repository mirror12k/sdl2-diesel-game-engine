

#include <list>


using std::list;


namespace diesel
{


class entity
{
public:
    bool is_drawn = 0;

    virtual void update ();
    virtual void draw ();
};




class entity_pool : public entity
{
private:
    list<entity*> entity_pool;
    list<entity*> to_remove_pool;

public:
    ~entity_pool();


    void update ();
    // removes any entities from the pool which are scheduled for removal
    // called automatically at the start of update()
    void update_removals ();

    // adds a new entity to the entity pool
    void add_entity (entity* ent);
    // marks an entity for removal and eventual deletion
    // safe (but expensive) to remove same entity multiple times
    void remove_entity (entity* ent);
    // immediately removes and deletes all entities
    void remove_all ();
};


}


