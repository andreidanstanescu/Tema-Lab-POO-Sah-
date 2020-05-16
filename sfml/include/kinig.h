#ifndef KINIG_H
#define KINIG_H
#include "Piece.h"

class kinig:public Piece
{
    public:
        kinig();
        Piece* Clone()
        {
            return new kinig(*this);
        }
        std::set< std::pair< int,int > > get_moves(int p,int q,int mat[8][8],std::vector<Piece*>& f,bool turn){
            std::set< std::pair< int,int > > h ;
            std::vector< std::pair< int,int > > v {{p,q-1},{p,q+1},{p-1,q-1},{p+1,q-1},{p+1,q},{p-1,q},{p+1,q+1},{p-1,q+1}};
            auto pushinto = [&] (const int &a,const int &b)
            {
                if(a>=0 && a<8 && b>=0 && b<8 && (!mat[a][b] || mat[a][b]*mat[p][q]<0))
                    h.insert(std::make_pair(a,b));
            };

            for(auto& x:v)
            {
                pushinto(x.first,x.second);
            }

            return h;
        }
        ~kinig();

};

#endif // KINIG_H
