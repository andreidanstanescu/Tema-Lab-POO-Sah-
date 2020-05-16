#include "table.h"
#include "My_Exception.h"

table::table()
{
    nr_moves=0;
    sp.reserve(32);
    for(int i=0;i<32;i++)
    {
        if(i==0 || i==7 || i==24 || i==31)
        {
            Piece* x=Piece::Create(Turn);
            sp.emplace_back(std::shared_ptr<Piece>(x));
        }
        else if(i==1 || i==6 || i==30 || i==25)
        {
            Piece* x=Piece::Create(Cal);
            //std::unique_ptr<Piece> ptr2x(x);
            //nu stiu daca face conversie implicita
            sp.emplace_back(std::shared_ptr<Piece>(x));
        }
        else if(i==3 || i==27)
        {
            Piece* x=Piece::Create(Regina);
            sp.emplace_back(std::shared_ptr<Piece>(x));
        }
        else if(i==4 || i==28)
        {
            Piece* x=Piece::Create(Rege);
            sp.emplace_back(std::shared_ptr<Piece>(x));
        }
        else if(i==2 || i==5 || i==29 || i==26)
        {
            Piece *x=Piece::Create(Nebun);
            sp.emplace_back(std::shared_ptr<Piece>(x));
        }
        else{
       Piece* x=Piece::Create(Pion);
       sp.emplace_back(std::shared_ptr<Piece>(x));
        }
    }

    p.reserve(32);
    for(int i=0;i<32;i++)
    {
        if(i==0 || i==7 || i==24 || i==31)
        {
            Piece* x=Piece::Create(Turn);
            p.emplace_back(x);
        }
        else if(i==1 || i==6 || i==30 || i==25)
        {
            Piece* x=Piece::Create(Cal);
            p.emplace_back(x);
        }
        else if(i==3 || i==27)
        {
            Piece* x=Piece::Create(Regina);
            p.emplace_back(x);
        }
        else if(i==4 || i==28)
        {
            Piece* x=Piece::Create(Rege);
            p.emplace_back(x);
        }
        else if(i==2 || i==5 || i==29 || i==26)
        {
            Piece *x=Piece::Create(Nebun);
            p.emplace_back(x);
        }
        else{
       Piece* x=Piece::Create(Pion);
       p.emplace_back(x);
        }
    }


}


bool is_in_chess(const table& T,bool white){

bool is_chess=0;
    if(!white)
    {
        for(int i=0;i<16;++i)
        {
            int x=T.p[i]->get_x();
            int y=T.p[i]->get_y();
            //cout<<i<<' '<<x<<' '<<y<<endl;
            for(auto& it: ( (T.p[i])->get_moves(x,y,const_cast<table*>(&T)->tabla,const_cast<table*>(&T)->p,white)) )
            {
                //cout<<it.first<<' '<<it.second<<' '<<viz[it.first][it.second]<<'\n';
                if(T.tabla[it.first][it.second]==6)
                {
                    //cout<<"Sah la "<<(!white ? "Alb" : "Negru")<<'\n';
                    is_chess=1;
                    break;
                }
            }
        }
    }
    else
    {
        for(int i=16;i<32;++i)
        {
            int x=T.p[i]->get_x();
            int y=T.p[i]->get_y();
            //cout<<i<<' '<<x<<' '<<y<<endl;
            for(auto& it: (T.p[i]->get_moves(x,y,const_cast<table*>(&T)->tabla,const_cast<table*>(&T)->p,white)) )
            {
                //cout<<it.first<<' '<<it.second<<' '<<viz[it.first][it.second]<<'\n';
                if(T.tabla[it.first][it.second]==-6)
                {
                    //cout<<"Sah la "<<(!white ? "Alb" : "Negru")<<'\n';
                    is_chess=1;
                    break;
                }
            }
        }

    }
    return is_chess;

}

void table::notify() {
  // 5. Publisher broadcasts
  for (int i = 0; i < static_cast<int>(views.size()); i++)
    views[i]->update();
}

std::string& table::operator[](int index)
{
    try{
    if (index >= nr_moves) {
            throw My_Exception("Array index out of bound, exiting");
    }
    }
    catch(My_Exception& e){
        std::cout<<e.what();
    }
    return move_count[index];
    int i=0;
    std::vector< std::string >::iterator it=move_count.begin();
    while(i<index)
        ++it;
    return *it;
}

table& table::operator=(const table& rhs)
{
    if(this!=&rhs)
    {
        for(int i=0;i<8;++i)
            for(int j=0;j<8;++j)
            this->tabla[i][j]=rhs.tabla[i][j];

        this->p.reserve(rhs.p.size());
        for(std::size_t i=0;i<rhs.p.size();++i)
        {
            Piece* x;
            this->p[i]=rhs.p[i]->Clone();
            //sau, clasic, cum e mai jos comentat (merg ambele variante)
            /*if(dynamic_cast<Pawn*>(rhs.p[i]))
                this->p[i]=Piece::Create(Pion);
            else
                if(dynamic_cast<Knight*>(rhs.p[i]))
                    this->p[i]=Piece::Create(Cal);
            else
                if(dynamic_cast<kinig*>(rhs.p[i]))
                    this->p[i]=Piece::Create(Rege);
            else
                if(dynamic_cast<Rook*>(rhs.p[i]))
                    this->p[i]=Piece::Create(Turn);
            else
                if(dynamic_cast<Queen*>(rhs.p[i]))
                    this->p[i]=Piece::Create(Regina);
            else
                if(dynamic_cast<Bishop*>(rhs.p[i]))
                    this->p[i]=Piece::Create(Nebun);
            */
        }

        this->turn=rhs.turn;
        this->nr_moves=rhs.nr_moves;
        this->move_count.reserve(rhs.nr_moves);
        for(const auto& it:rhs.move_count)
            this->move_count.emplace_back(it);

        this->views.reserve(rhs.views.size());
        for(const auto& it:rhs.views)
            this->views.emplace_back(it);

        //this->sp.reserve(rhs.sp.size());
        //for(const auto& it:rhs.sp)
         //   this->sp.emplace_back(std::shared_ptr<Piece>(new Piece(*it)));
    }
    return *this;
}


table::~table()
{
   //for(int i=0;i<8;i++)
      //  delete[] tabla[i];
}

Observer::~Observer()
{

}
