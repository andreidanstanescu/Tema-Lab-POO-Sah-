#ifndef TABLE_H
#define TABLE_H
#include "kinig.h"
#include "Pawn.h"
#include "Knight.h"
#include "Rook.h"
#include "Queen.h"
#include<iostream>
#include<memory>
#include<vector>

//Observer Pattern la sah mat
class Observer;

class table
{
private:
    int tabla[8][8]={
     {-4,-2,-3,-5,-6,-3,-2,-4},
     {-1,-1,-1,-1,-1,-1,-1,-1},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {4, 2, 3, 5, 6, 3, 2, 4}
    };
    std::vector< std::shared_ptr< Piece > >sp;
    std::vector< Piece* >p; //toate piesele
    std::vector<Observer*> views;
    std::vector< std::string > move_count;
    bool turn;
    int nr_moves;
    public:
        table();

        int spot(int x,int y)
        {
            return tabla[x][y];
        }
        void attach(Observer *obs)
        {
            views.push_back(obs);
        }

        std::vector< std::shared_ptr< Piece > >& get_sp()
        {
            return sp;
        }
        std::vector< Piece* > get_p()
        {
            return p;
        }

        std::string& operator[](int);

        table& operator=(const table&);

        void new_move(const std::string& type)
        {
            ++nr_moves;
            //de implementat aici move assignment
            move_count.push_back(type);
        }

        void notify();

        friend bool is_in_chess(const table&,bool);
        friend void initializing_1pieces(table& t);

        std::vector<Piece*> create_copy(const std::vector<Piece*>& p);

        ~table();
};


class Observer{
    table* subject;
    int denom;
public:
    Observer(table* t,int _denom)
    {
        subject=t;
        denom=_denom;
    }
    virtual void update()=0;
    virtual ~Observer();
protected:
    table *getSubject() {
        return subject;
    }
    int getDivisor() {
        return denom;
    }

};


#endif // TABLE_H
