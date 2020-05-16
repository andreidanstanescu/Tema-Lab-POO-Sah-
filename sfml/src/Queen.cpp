#include "Queen.h"


//Lambda functions

std::set< std::pair< int,int > > Queen::get_moves(int p,int q,int mat[8][8],std::vector<Piece*>& f,bool turn){

    std::set< std::pair< int,int > > h;

    auto add =[&](std::pair<int, int> m)
    {
        h.insert(m);
    };

    for(auto it:Rook::get_moves(p,q,mat,f,turn))
        h.insert(it);
    for(auto it:Bishop::get_moves(p,q,mat,f,turn))
        h.insert(it);
    return h;
}

Queen::~Queen()
{
    //dtor
}
