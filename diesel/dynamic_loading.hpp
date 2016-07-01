
#pragma once



#include "entity.hpp"
#include "dynamic_values.hpp"


//#include <map>
//using std::map;



namespace diesel
{




class dynamic_loading_exception : public exception
{
public:
    dynamic_loading_exception(const char* reason);
    ~dynamic_loading_exception();

    const char* what() const noexcept;

private:
    const char* msg;
};







class dynamicly_loadable_entity : public entity
{
public:
    virtual void load_dynamic(const dynamic_value& args);
};


template <class type>
class dynamic_class_instantiator
{
public:
    virtual dynamicly_loadable_entity* instantiate(const dynamic_value& args) const;
};



template <class type>
dynamicly_loadable_entity* dynamic_class_instantiator<type>::instantiate(const dynamic_value& args) const
{
    dynamicly_loadable_entity* instance = new type();
    instance->load_dynamic(args);
    return instance;
}





class dynamic_loader
{
private:
    map<string, dynamic_class_instantiator<dynamicly_loadable_entity>*> registry;
public:
    template <class type>
    void register_class(const string& classname);
    dynamicly_loadable_entity* load(const string& classname, const dynamic_value& args);
};



template <class type>
void dynamic_loader::register_class(const string& classname)
{
    this->registry[classname] = (dynamic_class_instantiator<dynamicly_loadable_entity>*)(new dynamic_class_instantiator<type>());
}



}



