
#pragma once



#include <exception>
using std::exception;

#include <string>
using std::string;




namespace diesel
{




class generic_exception : public exception
{
protected:
    string reason;
public:
    generic_exception(const string& reason);

    const char* what() const noexcept;
};




}


