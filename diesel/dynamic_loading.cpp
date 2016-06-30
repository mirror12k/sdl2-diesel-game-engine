
#include "dynamic_loading.hpp"



namespace diesel
{



dynamic_loading_exception::dynamic_loading_exception(const char* reason)
{
    char* buffer = new char[2048];
    snprintf(buffer, 2048, "dynamic loading exception: %s", reason);
    this->msg = buffer;
}

dynamic_loading_exception::~dynamic_loading_exception()
{
    delete this->msg;
}

const char* dynamic_loading_exception::what() const noexcept
{
    return this->msg;
}





void dynamicly_loadable_entity::load_dynamic(const dynamic_value& args)
{}



dynamicly_loadable_entity* dynamic_loader::load(const string& classname, const dynamic_value& args)
{
    if (this->registry.find(classname) == this->registry.end())
        throw dynamic_loading_exception(("attempt to load unknown dynamic class [" + classname + "]").c_str());
    else
        return this->registry[classname]->instantiate(args);
}



}



