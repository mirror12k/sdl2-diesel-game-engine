


#include "dynamic_values.hpp"


namespace diesel
{






dynamic_value_exception::dynamic_value_exception(dynamic_value_type expected, dynamic_value_type received)
: expected(expected), received(received)
{
    char* buffer = new char[2048];
    snprintf(buffer, 2048, "incorrect type when accessing dynamic value: '%s' expected, '%s' received",
        this->dynamic_type_to_string(expected), this->dynamic_type_to_string(received));
    this->msg = buffer;
}

dynamic_value_exception::~dynamic_value_exception()
{
    delete this->msg;
}

const char* dynamic_value_exception::dynamic_type_to_string(dynamic_value_type type)
{
    switch (type)
    {
    case DYNAMIC_VALUE_INT:
        return "DYNAMIC_VALUE_INT";
    case DYNAMIC_VALUE_FLOAT:
        return "DYNAMIC_VALUE_FLOAT";
    case DYNAMIC_VALUE_STRING:
        return "DYNAMIC_VALUE_STRING";
    case DYNAMIC_VALUE_REF:
        return "DYNAMIC_VALUE_REF";
    default:
        return "*UNKNOWN DYNAMIC TYPE*";
    }
}

const char* dynamic_value_exception::what() const noexcept
{
    return this->msg;
}











dynamic_value::dynamic_value()
: type(DYNAMIC_VALUE_NONE)
{}

dynamic_value::dynamic_value(int value_int)
: type(DYNAMIC_VALUE_INT)
{
    this->value_int = value_int;
}
dynamic_value::dynamic_value(float value_float)
: type(DYNAMIC_VALUE_FLOAT)
{
    this->value_float = value_float;
}
dynamic_value::dynamic_value(const string& value_string)
: type(DYNAMIC_VALUE_STRING)
{
    this->value_string = value_string;
}
dynamic_value::dynamic_value(void* value_ref)
: type(DYNAMIC_VALUE_REF)
{
    this->value_ref = value_ref;
}



dynamic_value_type dynamic_value::get_type() const
{
    return this->type;
}



int dynamic_value::as_int () const
{
    if (this->type != DYNAMIC_VALUE_INT)
        throw dynamic_value_exception(DYNAMIC_VALUE_INT, this->type);

    return this->value_int;
}
int dynamic_value::as_float () const
{
    if (this->type != DYNAMIC_VALUE_FLOAT)
        throw dynamic_value_exception(DYNAMIC_VALUE_FLOAT, this->type);

    return this->value_float;
}
string dynamic_value::as_string () const
{
    if (this->type != DYNAMIC_VALUE_STRING)
        throw dynamic_value_exception(DYNAMIC_VALUE_STRING, this->type);

    return this->value_string;
}
void* dynamic_value::as_ref () const
{
    if (this->type != DYNAMIC_VALUE_REF)
        throw dynamic_value_exception(DYNAMIC_VALUE_REF, this->type);

    return this->value_ref;
}




}



