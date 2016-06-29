
#include "dynamic_loading.hpp"



namespace diesel
{


void dynamicly_loadable_entity::load_dynamic(const dynamic_object_value& args)
{
    printf("debug error\n");
}



dynamicly_loadable_entity* dynamic_loader::load(const string& classname, const dynamic_object_value& args)
{
    return this->registry[classname]->instantiate(args);
}



}



