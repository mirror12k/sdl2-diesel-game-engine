
#include "entity.hpp"


#include <stdio.h>

namespace diesel
{


void entity::update() {}
void entity::draw() {}




void entity_pool::update()
{
    for (list<entity*>::iterator iter = this->entity_pool.begin(), iter_end = this->entity_pool.end(); iter != iter_end; iter++)
    {
        printf("updating ent\n");
        (*iter)->update();
    }
}

void entity_pool::add_entity (entity* ent)
{
    this->entity_pool.push_back(ent);
}

}


