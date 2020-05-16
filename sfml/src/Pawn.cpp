#include "Pawn.h"


Pawn::Pawn()
{
    type="pion";
}

std::set< std::pair< int,int > >  Pawn::get_moves(int p,int q,int mat[8][8],std::vector<Piece*>& f,bool turn)
{
    std::set< std::pair< int,int > > h;
            if(!turn)
            {
                //std::cout<<p<<' '<<q+1<<' '<<mat[p][q-1]<<'\n';
                if(!mat[p+1][q])
                    h.insert(std::make_pair(p+1,q));
                if(p+1<8 && q+1<8 && mat[p+1][q+1]*mat[p][q]<0)
                    h.insert(std::make_pair(p+1,q+1));
                if(p+1<8 && q-1>=0 && mat[p+1][q-1]*mat[p][q]<0)
                    h.insert(std::make_pair(p+1,q-1));

            }
            else
            {
                //std::cout<<p<<' '<<q-1<<' '<<mat[p][q-1]<<'\n';
                if(!mat[p-1][q])
                    h.insert(std::make_pair(p-1,q));
                if(p-1>=0 && q-1>=0 && mat[p-1][q-1]*mat[p][q]<0)
                    h.insert(std::make_pair(p-1,q-1));
                if(p-1>=0 && q+1<8 && mat[p-1][q+1]*mat[p][q]<0)
                    h.insert(std::make_pair(p-1,q+1));


            }
            //std::cout<<"Miscarile posibile sunt:\n";
            //for(auto it:h)
             //   std::cout<<it.first<<' '<<it.second<<std::endl;

            return h;
}

Pawn::~Pawn()
{

}
