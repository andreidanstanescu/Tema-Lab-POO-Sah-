#ifndef QUEEN_H
#define QUEEN_H
#include "Bishop.h"
#include "Rook.h"

//mostenire in diamant

class Queen:public Bishop, public Rook
{
    public:
        Queen():Bishop(),Rook(){
            type="regina";
        }
        Piece* Clone()
        {
            return new Queen(*this);
        }
        std::set< std::pair< int,int > > get_moves(int ,int ,int mat[8][8],std::vector<Piece*>& ,bool );
        ~Queen();

};

#endif // QUEEN_H
