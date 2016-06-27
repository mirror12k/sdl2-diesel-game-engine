

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
public:
    void update ();
    void add_entity (entity* ent);
    void remove_entity (entity* ent);
};


}


