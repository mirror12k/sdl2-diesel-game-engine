


#include "dynamic_values.hpp"


namespace diesel
{






dynamic_value_exception::dynamic_value_exception(dynamic_value_type expected, dynamic_value_type received)
: generic_exception("incorrect type when accessing dynamic value: '"+this->dynamic_type_to_string(expected)+
    "' expected, '"+this->dynamic_type_to_string(received)+"' received"),
expected(expected), received(received)
{}

string dynamic_value_exception::dynamic_type_to_string(dynamic_value_type type)
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











dynamic_value::dynamic_value()
: type(DYNAMIC_VALUE_OBJECT)
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



template <>
int dynamic_value::at<int> (const string& key) const
{
    if (this->type != DYNAMIC_VALUE_OBJECT)
        throw dynamic_value_exception(DYNAMIC_VALUE_OBJECT, this->type);

    return this->value_object.at(key).as_int();
}

template <>
float dynamic_value::at<float> (const string& key) const
{
    if (this->type != DYNAMIC_VALUE_OBJECT)
        throw dynamic_value_exception(DYNAMIC_VALUE_OBJECT, this->type);

    return this->value_object.at(key).as_float();
}

template <>
string dynamic_value::at<string> (const string& key) const
{
    if (this->type != DYNAMIC_VALUE_OBJECT)
        throw dynamic_value_exception(DYNAMIC_VALUE_OBJECT, this->type);

    return this->value_object.at(key).as_string();
}

template <>
void* dynamic_value::at<void*> (const string& key) const
{
    if (this->type != DYNAMIC_VALUE_OBJECT)
        throw dynamic_value_exception(DYNAMIC_VALUE_OBJECT, this->type);

    return this->value_object.at(key).as_ref();
}


void dynamic_value::set (const string& key, const dynamic_value& val)
{
    if (this->type != DYNAMIC_VALUE_OBJECT)
        throw dynamic_value_exception(DYNAMIC_VALUE_OBJECT, this->type);

    this->value_object[key] = val;
}



}



