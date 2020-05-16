#ifndef PAWN_H
#define PAWN_H
#include "Piece.h"

class Pawn:public Piece
{
    public:
        Pawn();

        Piece* Clone()
        {
            return new Pawn(*this);
        }

        std::set< std::pair< int,int > > get_moves(int p,int q,int mat[8][8],std::vector<Piece*>& f,bool turn);

        ~Pawn();

};

#endif // PAWN_H
