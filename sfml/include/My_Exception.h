#ifndef MY_EXCEPTION_H
#define MY_EXCEPTION_H
#include<stdexcept>

class My_Exception: public std::logic_error
{
    public:
        My_Exception(const char *s);
        ~My_Exception();

};

#endif // MY_EXCEPTION_H
