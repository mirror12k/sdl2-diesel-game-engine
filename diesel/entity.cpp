
#include "entity.hpp"


#include <stdio.h>

namespace diesel
{


void entity::update() {}
void entity::draw() {}





entity_pool::~entity_pool()
{
    this->remove_all();
}


void entity_pool::update()
{
    this->update_removals();

    for (list<entity*>::iterator iter = this->entity_pool.begin(), iter_end = this->entity_pool.end(); iter != iter_end; iter++)
    {
//        printf("updating ent\n");
        (*iter)->update();
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


