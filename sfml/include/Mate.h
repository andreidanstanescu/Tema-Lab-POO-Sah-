#ifndef MATE_H
#define MATE_H
#include "table.h"

class Mate: public Observer
{
    public:
        Mate(table* t,int _denom):Observer(t,_denom){}
        void update()
        {


        }
        virtual ~Mate();

};

#endif // MATE_H
