
#pragma once

#include <map>
using std::map;

#include <string>
using std::string;

#include <exception>
using std::exception;



namespace diesel
{


enum dynamic_value_type
{
    DYNAMIC_VALUE_NONE,

    DYNAMIC_VALUE_INT,
    DYNAMIC_VALUE_FLOAT,
    DYNAMIC_VALUE_STRING,
    DYNAMIC_VALUE_REF,

    DYNAMIC_VALUE_OBJECT,
};


class dynamic_value_exception : public exception
{
public:
    dynamic_value_exception(dynamic_value_type expected, dynamic_value_type received);
    ~dynamic_value_exception();

    const char* dynamic_type_to_string(dynamic_value_type type);
    const char* what() const noexcept;

    const dynamic_value_type expected, received;

private:
    const char* msg;
};



class dynamic_value
{
private:
    dynamic_value_type type;
    int value_int;
    float value_float;
    string value_string;
    void* value_ref;

    map<string, dynamic_value> value_object;

public:
    dynamic_value();
    dynamic_value(int value_int);
    dynamic_value(float value_float);
    dynamic_value(const string& value_string);
    dynamic_value(void* value_ref);

    dynamic_value_type get_type() const;

    int as_int () const;
    int as_float () const;
    string as_string () const;
    void* as_ref () const;

    template <typename T>
    T at (const string& key) const;
    void set (const string& key, const dynamic_value& val);

};





}


