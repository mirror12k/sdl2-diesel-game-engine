
#include "entity.hpp"


namespace diesel
{


entity::~entity()
{}

bool entity::is_drawn () const
{
    return false;
}

void entity::update(update_context* ctx)
{}
void entity::draw(drawing_context* ctx)
{}



void entity::on_added(update_context* ctx)
{}
void entity::on_removed(update_context* ctx)
{}


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

    this->update_removals(ctx);
}

void entity_pool::draw(drawing_context* ctx)
{
    for (list<entity*>::iterator iter = this->entity_pool.begin(), iter_end = this->entity_pool.end(); iter != iter_end; iter++)
    {
        (*iter)->draw(ctx);
    }
}

void entity_pool::update_removals(update_context* ctx)
{
    for (list<entity*>::iterator remove_iter = this->to_remove_pool.begin(), remove_iter_end = this->to_remove_pool.end();
            remove_iter != remove_iter_end; remove_iter++)
    {
        for (list<entity*>::iterator iter = this->entity_pool.begin(), iter_end = this->entity_pool.end(); iter != iter_end; iter++)
        {
            if (*iter == *remove_iter)
            {
                (*iter)->on_removed(ctx);
                delete *iter;
                iter = this->entity_pool.erase(iter);
                break;
            }
        }
    }
    this->to_remove_pool.clear();

    for (list<entity*>::iterator iter = this->to_add_pool.begin(), iter_end = this->to_add_pool.end(); iter != iter_end; iter++)
    {
        this->entity_pool.push_back(*iter);
        (*iter)->on_added(ctx);
    }
    this->to_add_pool.clear();
//    this->entity_pool.splice(this->entity_pool.begin(), this->to_add_pool);
}



void entity_pool::add_entity (entity* ent)
{
    this->to_add_pool.push_back(ent);
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


