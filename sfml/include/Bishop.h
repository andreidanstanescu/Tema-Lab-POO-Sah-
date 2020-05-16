#ifndef BISHOP_H
#define BISHOP_H
#include "Piece.h"

//Lambda function

class Bishop:virtual public Piece
{
    public:
        Bishop();
        Piece* Clone()
        {
            return new Bishop(*this);
        }
        std::set< std::pair< int,int > > get_moves(int ,int ,int mat[8][8],std::vector<Piece*>&,bool );
        ~Bishop();

};

#endif // BISHOP_H
