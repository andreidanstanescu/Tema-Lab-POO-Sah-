#include "Bishop.h"

Bishop::Bishop()
{
    type="bishop";
}

Bishop::~Bishop()
{
    //dtor
}

std::set< std::pair< int,int > > Bishop::get_moves(int p,int q,int mat[8][8],std::vector<Piece*>&f,bool turn)
{
    std::set< std::pair< int,int > > h;

            auto add_if=[&](int a,int b)
            {
                if(a>=0 && a<8 && b>=0 && b<8 && (!mat[a][b] || mat[a][b]*mat[p][q]<0))
                    h.insert(std::make_pair(a,b));
            };

            int i=p-1,j=q+1;
            while(i>=0 && j<8 && !mat[i][j])
            {
                add_if(i,j);
                --i;
                ++j;
            }
            add_if(i,j);

            i=p+1;
            j=q-1;
            while(i<8 && j>=0 && !mat[i][j])
            {
                add_if(i,j);
                ++i;
                --j;
            }
            add_if(i,j);

            i=p-1;
            j=q-1;
            while(i>=0 && j>=0 && !mat[i][j])
            {
                add_if(i,j);
                --i;
                --j;
            }
            add_if(i,j);

            i=p+1;
            j=q+1;
            while(i<8 && j<8 && !mat[i][j])
            {
                add_if(i,j);
                ++i;
                ++j;
            }
            add_if(i,j);

            return h;
}
