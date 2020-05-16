#include "Piece.h"
#include "Knight.h"
#include "Pawn.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "kinig.h"
#include "My_Exception.h"
#include<stdexcept>


Piece::Piece()
{
    //ctor
}

Piece::~Piece()
{
    //dtor
}

Piece* Piece::Create(tip_piesa t)
{
    try{
    if(t==Pion)
        return new Pawn();
    if(t==Cal)
        return new Knight();
    if(t==Nebun)
        return new Bishop();
    if(t==Turn)
        return new Rook();
    if(t==Regina)
        return new Queen();
    if(t==Rege)
        return new kinig();
    throw My_Exception("Piesa nu e valida!");
    }
    catch(My_Exception& e)
    {
        std::cout<<"Eroare capturata: "<<e.what()<<'\n';
    }

}
