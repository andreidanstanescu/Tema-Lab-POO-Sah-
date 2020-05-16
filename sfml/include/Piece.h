#ifndef PIECE_H
#define PIECE_H
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include<iostream>
#include<vector>
#include<windows.h>
#include<string>

//Factory Pattern

enum tip_piesa{
    Pion,Cal,Nebun,Turn,Regina,Rege
};

class Piece{
protected:
    sf::Sprite s;
    int x, y;
    std::string type,color;
public:
    Piece();

    virtual Piece* Clone()=0;

    sf::Sprite& get_s()
    {
        return s;
    }
    int get_x()
    {
        return x;
    }
    int get_y()
    {
        return y;
    }
    std::string get_color()
    {
        return color;
    }
    std::string get_type()
    {
        return type;
    }
    void set_pos(int _x,int _y)
    {
        x=_x;
        y=_y;
    }

    virtual std::set< std::pair< int,int > > get_moves(int p,int q,int mat[8][8],std::vector<Piece*>& f,bool turn)=0;
    //virtual std::set< std::pair< int,int > > get_moves(int p,int q,int mat[8][8],std::vector< std::unique_ptr< Piece > >& f,bool turn)=0;


    //pentru Factory
    static Piece* Create(tip_piesa );


    virtual ~Piece();

};


#endif // PIECE_H
