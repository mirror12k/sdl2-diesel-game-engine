
#include "entity.hpp"


namespace diesel
{


void entity::update(update_context* ctx) {}
void entity::draw(drawing_context* ctx) {}





entity_pool::~entity_pool()
{
    this->remove_all();
}


void entity_pool::update(update_context* ctx)
{
    for (list<entity*>::iterator iter = this->entity_pool.begin(), iter_end = this->entity_pool.end(); iter != iter_end; iter++)
    {
//        printf("updating ent\n");
        (*iter)->update(ctx);
    }

    this->update_removals();
}

void entity_pool::draw(drawing_context* ctx)
{
    for (list<entity*>::iterator iter = this->entity_pool.begin(), iter_end = this->entity_pool.end(); iter != iter_end; iter++)
    {
        (*iter)->draw(ctx);
    }
}

void entity_pool::update_removals()
{
    for (list<entity*>::iterator remove_iter = this->to_remove_pool.begin(), remove_iter_end = this->to_remove_pool.end();
        remove_iter != remove_iter_end; remove_iter++)
    {
        for (list<entity*>::iterator iter = this->entity_pool.begin(), iter_end = this->entity_pool.end(); iter != iter_end; iter++)
        {
            if (*iter == *remove_iter)
            {
                delete *iter;
                iter = this->entity_pool.erase(iter);
            }
        }
    }

    this->to_remove_pool.clear();
}



void entity_pool::add_entity (entity* ent)
{
    this->entity_pool.push_back(ent);
}

void entity_pool::remove_entity (entity* ent)
{
    this->to_remove_pool.push_back(ent);
}

void entity_pool::remove_all ()
{
    for (list<entity*>::iterator iter = this->entity_pool.begin(), iter_end = this->entity_pool.end(); iter != iter_end; iter++)
    {
        delete (*iter);
    }

    this->entity_pool.clear();
}

}


