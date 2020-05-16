#ifndef KING_H
#define KING_H
#include "Piece.h"

class King:public Piece
{
    public:
        King();
        std::set< std::pair< int,int > > get_moves(int p,int q,int mat[8][8],std::vector<Piece*>& f,bool turn){
          std::set< std::pair< int,int > > h;
          h.insert({1,2});
          return h;

        }
        ~King();

};

#endif // KING_H
