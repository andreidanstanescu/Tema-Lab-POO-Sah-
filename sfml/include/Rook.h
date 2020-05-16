#ifndef ROOK_H
#define ROOK_H
#include "Piece.h"

class Rook:virtual public Piece
{
    public:
        Rook();
        Piece* Clone()
        {
            return new Rook(*this);
        }
        std::set< std::pair< int,int > > get_moves(int ,int ,int mat[8][8],std::vector<Piece*>&,bool );
        ~Rook();

};

#endif // ROOK_H
