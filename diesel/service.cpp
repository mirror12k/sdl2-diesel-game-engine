
#include "service.hpp"


namespace diesel
{



service::service(const string& service_name)
: service_name(service_name)
{}

string service::get_service_name() const
{
    return this->service_name;
}



}


