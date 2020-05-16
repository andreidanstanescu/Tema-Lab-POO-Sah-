#include "My_Exception.h"

My_Exception::My_Exception(const char* s):std::logic_error(s){}


My_Exception::~My_Exception()
{
    //dtor
}
