#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include<windows.h>
#include<memory>
#include<thread>
#include "table.h"
#include "Piece.h"
#include "Knight.h"
#include "Pawn.h"
#include "Bishop.h"
#include "Rook.h"
#include "King.h"
#include "Queen.h"
#include "My_Exception.h"

using namespace std;


bool is_in_chess(int viz[8][8],vector<Piece*>& p,bool white)
{
    bool is_chess=0;
    if(!white)
    {
        for(int i=0;i<16;++i)
        {
            int x=p[i]->get_x();
            int y=p[i]->get_y();
            //cout<<i<<' '<<x<<' '<<y<<endl;
            for(auto& it: (p[i]->get_moves(x,y,viz,p,white)) )
            {
                //cout<<it.first<<' '<<it.second<<' '<<viz[it.first][it.second]<<'\n';
                if(viz[it.first][it.second]==6)
                {
                    cout<<"Sah la "<<(!white ? "Alb" : "Negru")<<'\n';
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
            int x=p[i]->get_x();
            int y=p[i]->get_y();
            //cout<<i<<' '<<x<<' '<<y<<endl;
            for(auto& it: (p[i]->get_moves(x,y,viz,p,white)) )
            {
                //cout<<it.first<<' '<<it.second<<' '<<viz[it.first][it.second]<<'\n';
                if(viz[it.first][it.second]==-6)
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


void is_chess_Mate(int viz[8][8],vector<Piece*>& p,bool white)
{
    //creez o copie pentru tabla de joc
    int viz2[8][8];
    for(int i=0;i<8;++i)
        for(int j=0;j<8;++j)
            viz2[i][j]=viz[i][j];

    bool is_chess=0;
    bool is_mat=1;
    //cout<<white;
    if(!white)
    {
        for(int i=0;i<16;++i)
        {
            int x=p[i]->get_x();
            int y=p[i]->get_y();
            //cout<<i<<endl;
            //cout<<i<<' '<<x<<' '<<y<<endl;
            for(auto& it: (p[i]->get_moves(x,y,viz,p,white)) )
            {
                //cout<<it.first<<' '<<it.second<<' '<<viz[it.first][it.second]<<'\n';
                if(viz[it.first][it.second]==6)
                {
                    //cout<<"Sah la "<<(!white ? "Alb" : "Negru")<<'\n';
                    is_chess=1;
                    break;
                }
            }
        }
        is_mat=is_chess;
        if(is_chess)
        {
            for(int i=16;i<32;++i)
            {
                int x=p[i]->get_x();
                int y=p[i]->get_y();
                int tip=viz[x][y];
                //cout<<i<<' '<<x<<' '<<y<<endl;
                //cout<<i<<' '<<x<<' '<<y<<endl;
                for(auto& it: (p[i]->get_moves(x,y,viz,p,!white)) )
                {
                    //cout<<it.first<<' '<<it.second<<' '<<viz[it.first][it.second]<<'\n';
                    if(!viz[it.first][it.second])
                    {
                        //spot liber si incerc mutarea
                        viz[it.first][it.second]=viz[x][y];
                        viz[x][y]=0;
                        p[i]->set_pos(it.first,it.second);
                        if(!is_in_chess(viz,p,white))
                        {
                            //cout<<"Nu e sah\n";
                            is_mat=0;
                            viz[it.first][it.second]=0;
                            viz[x][y]=tip;
                            p[i]->set_pos(x,y);
                            break;
                        }
                        viz[it.first][it.second]=0;
                        viz[x][y]=tip;
                        p[i]->set_pos(x,y);
                    }
                    else
                    if(viz[it.first][it.second]<0)
                    {
                        int before=viz[it.first][it.second];
                        viz[it.first][it.second]=viz[x][y];
                        viz[x][y]=0;
                        p[i]->set_pos(it.first,it.second);
                        int j;
                        for(int idx=16;idx<32;++idx)
                            if(p[idx]->get_x()==it.first && p[idx]->get_y()==it.second)
                        {
                            j=idx;
                            break;
                        }
                        p[j]->set_pos(-100,-100);
                        if(!is_in_chess(viz,p,white))
                        {
                            is_mat=0;
                            viz[it.first][it.second]=before;
                            viz[x][y]=tip;
                            p[i]->set_pos(x,y);
                            p[j]->set_pos(it.first,it.second);
                            break;
                        }
                        viz[it.first][it.second]=before;
                        viz[x][y]=tip;
                        p[i]->set_pos(x,y);
                        p[j]->set_pos(it.first,it.second);

                    }
                }
            }

        }

    }
    if(white)
    {
        bool atac=0;
        for(int i=16;i<32 && !atac;++i)
        {
            int x=p[i]->get_x();
            int y=p[i]->get_y();
            //cout<<i<<' '<<x<<' '<<y<<endl;
            for(auto& it: (p[i]->get_moves(x,y,viz,p,white)))
            {
                //cout<<it.first<<' '<<it.second<<' '<<viz[it.first][it.second]<<'\n';
                if(viz[it.first][it.second]==-6)
                {
                    //cout<<"Sah la "<<(!white ? "Alb" : "Negru")<<'\n';
                    is_chess=1;
                    atac=1;
                    break;
                }
            }
        }
        is_mat=is_chess;
        //cout<<"Albul da sah:"<<is_chess<<endl;
        if(is_chess)
        {
            for(int i=0;i<16;++i)
            {
                int x=p[i]->get_x();
                int y=p[i]->get_y();
                int tip=viz[x][y];
                //cout<<i<<' '<<x<<' '<<y<<endl;
                for(auto& it: (p[i]->get_moves(x,y,viz,p,white)) )
                {
                    //cout<<it.first<<' '<<it.second<<' '<<viz[it.first][it.second]<<'\n';
                    if(!viz[it.first][it.second])
                    {
                        //spot liber si incerc mutarea
                        viz[it.first][it.second]=viz[x][y];
                        viz[x][y]=0;
                        p[i]->set_pos(it.first,it.second);
                        if(!is_in_chess(viz,p,white))
                        {
                            is_mat=0;
                            viz[it.first][it.second]=0;
                            viz[x][y]=tip;
                            p[i]->set_pos(x,y);
                            break;
                        }
                        viz[it.first][it.second]=0;
                        viz[x][y]=tip;
                        p[i]->set_pos(x,y);
                    }
                    else
                    if(viz[it.first][it.second]>0)
                    {
                        int before=viz[it.first][it.second];
                        viz[it.first][it.second]=viz[x][y];
                        viz[x][y]=0;
                        p[i]->set_pos(it.first,it.second);
                        int j;
                        for(int idx=0;idx<16;++idx)
                            if(p[idx]->get_x()==it.first && p[idx]->get_y()==it.second)
                        {
                            j=idx;
                            break;
                        }
                        p[j]->set_pos(-100,-100);
                        if(!is_in_chess(viz,p,white))
                        {
                            is_mat=0;
                            viz[it.first][it.second]=before;
                            viz[x][y]=tip;
                            p[i]->set_pos(x,y);
                            p[j]->set_pos(it.first,it.second);
                            break;
                        }
                        viz[it.first][it.second]=before;
                        viz[x][y]=tip;
                        p[i]->set_pos(x,y);
                        p[j]->set_pos(it.first,it.second);

                    }
                }
            }

        }


    }

    cout<<"Este sah mat:"<<is_mat<<endl;
    //return is_mat;

}



void afisare(int viz[8][8])
{
    for(int i=0;i<8;i++)
        {for(int j=0;j<8;j++)
        cout<<viz[i][j]<<' ';
        cout<<'\n';
        }

}

void afisare_logo()
{
    std::cout<< "||=====          // \\         |      |    | \n" <<
                "||              //   \\        |      |    |   \n" <<
                "||=====        // === \\       |======|    |        \n" <<
                "      ||      //       \\      |      |    |        \n" <<
                " =====||     //         \\     |      |    .           \n\n\n";
}

void loading()
{
    std::cout << "<";
    for (int i = 50; i > 0; i--)
    {
        std::cout << "=";
        std::this_thread::sleep_for(std::chrono::nanoseconds(50000000));
    }
    std::cout << "> \n\n";

    std::cout << "Sistem Initializat!\n\n\n";
}

constexpr int tablecoords_x(int a)
{
    return a/75;
}

constexpr int tablecoords_y(int a)
{
    return a/68;
}

constexpr int sfmlcoords_x(int a)
{
    return 75*a;
}

constexpr int sfmlcoords_y(int a)
{
    return 68*a;
}

constexpr double poscoords_x(int a)
{
    return 75*tablecoords_x(a);
}

constexpr double poscoords_y(int a)
{
    return 68*tablecoords_y(a);
}

//Clientul din Factory o sa fie clasa Game, care va fi construita pe un Singleton

int main()
{
    //afisare_logo();
    //loading();
    int viz[8][8]={
     {-4,-2,-3,-5,-6,-3,-2,-4},
     {-1,-1,-1,-1,-1,-1,-1,-1},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0},
      {1, 1, 1, 1, 1, 1, 1, 1},
      {4, 2, 3, 5, 6, 3, 2, 4}
    };

    bool white=1;
    bool is_chess=0;
    bool pas=0;
    /*vector< Piece* >p; //piesele, le voi pune intr-o clasa dupa
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
    }*/

    table T;

    //std::vector< std::unique_ptr< Piece > >p=T.get_p();

    std::vector< Piece* > p=T.get_p();
    //initializing_pieces(p);
    sf::Sprite loc;
    loc.setPosition(0,0);
    sf::Texture piese;
    piese.loadFromFile("figures.png");
    for(int i=0;i<32;i++)
    {
       p[i]->get_s().setTexture(piese);
    }
    int index=0;
    p[index]->get_s().setTextureRect(sf::IntRect(0,0,56,56));
    p[index]->get_s().setPosition(0,0);
    p[index]->set_pos(0,0);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(56,0,56,56));
    p[index]->get_s().setPosition(75,0);
    p[index]->set_pos(0,1);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(56*2,0,56,56));
    p[index]->get_s().setPosition(75*2,0);
    p[index]->set_pos(0,2);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(56*3,0,56,56));
    p[index]->get_s().setPosition(75*3,0);
    p[index]->set_pos(0,3);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(56*4,0,56,56));
    p[index]->get_s().setPosition(75*4,0);
    p[index]->set_pos(0,4);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(56*2,0,56,56));
    p[index]->get_s().setPosition(75*5,0);
    p[index]->set_pos(0,5);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(56*1,0,56,56));
    p[index]->get_s().setPosition(75*6,0);
    p[index]->set_pos(0,6);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(0,0,56,56));
    p[index]->get_s().setPosition(75*7,0);
    p[index]->set_pos(0,7);
    index++;


    index=8;
    for(int i=0;i<8;i++)
    {
       p[index]->get_s().setTextureRect(sf::IntRect(56*5,0,56,56));
       p[index]->get_s().setPosition(75*i,70);
       p[index]->set_pos(1,i);
       index++;
    }

    index=16;
    for(int i=0;i<8;i++)
    {
       p[index]->get_s().setTextureRect(sf::IntRect(56*5,56,56,56));
       p[index]->get_s().setPosition(75*i,68*6);
       p[index]->set_pos(6,i);
       index++;
    }

    p[index]->get_s().setTextureRect(sf::IntRect(0,56,56,56));
    p[index]->get_s().setPosition(0,68*7);
    p[index]->set_pos(7,0);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(56,56,56,56));
    p[index]->get_s().setPosition(75,68*7);
    p[index]->set_pos(7,1);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(56*2,56,56,56));
    p[index]->get_s().setPosition(75*2,68*7);
    p[index]->set_pos(7,2);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(56*3,56,56,56));
    p[index]->get_s().setPosition(75*3,68*7);
    p[index]->set_pos(7,3);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(56*4,56,56,56));
    p[index]->get_s().setPosition(75*4,68*7);
    p[index]->set_pos(7,4);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(56*2,56,56,56));
    p[index]->get_s().setPosition(75*5,68*7);
    p[index]->set_pos(7,5);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(56*1,56,56,56));
    p[index]->get_s().setPosition(75*6,68*7);
    p[index]->set_pos(7,6);
    index++;
    p[index]->get_s().setTextureRect(sf::IntRect(0,56,56,56));
    p[index]->get_s().setPosition(75*7,68*7);
    p[index]->set_pos(7,7);
    index++;


    sf::RenderWindow window(sf::VideoMode(800, 600), "Daniel joaca sah");

    //initialize(p);
    while (window.isOpen())
    {
        sf::Event event;
        sf::Shader shader;
        shader.loadFromFile("billboard.vert",sf::Shader::Vertex);

        sf::Image table;

        try{
        if(!table.loadFromFile("tabla.png"))
            throw My_Exception("Nu avem table!\n");
        }
        catch(My_Exception& e)
        {
            cout<<e.what();
        }

        sf::Texture texture;
        texture.loadFromImage(table);  //Load Texture from image

        sf::Sprite sprite;
        sprite.setTexture(texture);

        sprite.setScale(1.3,1.2);

        sf::Vector2i curr=sf::Mouse::getPosition(window)-sf::Vector2i(loc.getPosition());
        int i,ret;

        while (window.pollEvent(event))
        {
            //locatia de unde incepe fereastra de joc de sah
            int predx,predy;

            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type==sf::Event::KeyPressed)
                window.close();
            //incerc sa mut o piesa
            if (event.type == sf::Event::MouseButtonPressed)
            //if(event.type==sf::Event::MouseLeft)
            if(!pas)
                {
                    //cout<<white<<endl;
                    //aflu pe ce piesa pun mana (o mut)
                    //din coordonatele click-ului
                    for(i=0;i<32;i++)
                        if(p[i]->get_s().getGlobalBounds().contains(curr.x,curr.y))
                            {
                                ret=i;
                                //cout<<curr.x/56<<' '<<curr.y/56<<endl;
                                predy=p[i]->get_x();
                                predx=p[i]->get_y();
                                break;
                            }
                    //de verificare
                    //cout<<curr.x<<' '<<curr.y<<' '<<p[23].getPosition().x<<' '<<p[23].getPosition().y<<' '<<i<<' '<<endl;
                    //cout<<curr.x<<' '<<curr.y<<endl;
                    //cout<<i<<' ';
                    //p[i].setPosition(curr.x,curr.y);
                    pas=1;
                    /*if(is_chess)
                        {
                            if(dynamic_cast<King*>(p[ret]))
                            {
                                //
                                cout<<"Daniel joaca sah corect!\n";
                                is_chess=0;
                            }

                        }*/
                    continue;
                }
            //if (event.type == sf::Event::MouseButtonReleased)
                //if(event.key.code==sf::Event::MouseMoved)
            else
                {
                    T.new_move(p[ret]->get_type());
                    //curr=sf::Mouse::getPosition(window);
                    pas=0;
                    //curr=sf::Mouse::getPosition(window);
                    int j;
                        //cout<<curr.x/56-1<<' '<<curr.y/56-1<<endl;
                    bool sah_dinainte=is_chess;
                    for(j=0;j<32;j++)
                        if(p[j]->get_s().getGlobalBounds().contains(curr.x,curr.y))
                            {
                                //p[j].get_s().setPosition(600,500);
                                //cout<<curr.x<<' '<<curr.y<<' '<<p[j].getPosition().x<<' '<<p[j].getPosition().y<<endl;
                                break;
                            }
                    if(j<32)
                        {
                        //cout<<p[j]->get_x()<<' '<<p[j]->get_y()<<endl;
                        //cout<<predy<<' '<<predx<<endl;
                        //if(!(p[ret]->get_moves(predy,predx,viz,p,white)).count({(int)(curr.y/68),(int)(curr.x/75)}))
                        //    cout<<"Daniel NU joaca sah corect!\n";
                        if((p[ret]->get_moves(predy,predx,viz,p,white)).count({tablecoords_y(curr.y),tablecoords_x(curr.x)})==0)
                            cout<<"Daniel NU joaca sah corect!\n";
                        //cout<<predy<<' '<<predx<<' '<<white<<endl;
                        //cout<<p[ret]->get_x()<<' '<<p[ret]->get_y()<<'\n';
                        viz[p[j]->get_x()][p[j]->get_y()]=viz[p[ret]->get_x()][p[ret]->get_y()];
                        viz[p[ret]->get_x()][p[ret]->get_y()]=0;


                        p[ret]->get_s().setPosition(sfmlcoords_x(p[j]->get_y()),sfmlcoords_y(p[j]->get_x()));
                        p[ret]->get_s().setPosition(p[j]->get_s().getPosition());
                        p[ret]->set_pos(p[j]->get_x(),p[j]->get_y());
                        //cout<<p[j]->get_x()<<' '<<p[j]->get_y()<<endl;
                        //for(auto& it: (p[ret]->get_moves(predy,predx,viz,p,white)) )
                        //cout<<it.first<<' '<<it.second<<'\n';

                        //duc piesa in afara tablei
                        p[j]->get_s().setPosition(600,500);
                        p[j]->set_pos(-100,-100);
                        //afisare(viz);

                        }
                    else
                    {
                        //cout<<(int)(curr.y/68)<<' '<<(int)(curr.x/75)<<endl;
                        //cout<<predy<<' '<<predx<<endl;

                        //Am si init statement for if, insa e comentat, deoarece nu merge pe Code Blocks decat la ultima versiune
                        //(in caz ca rulati si da eroare de compilare
                        //if(std::set< std::pair< int,int > >s=(p[ret]->get_moves(predy,predx,viz,p,white)) ; s.count({tablecoords_y(curr.y),tablecoords_x(curr.x)})==0)
                        //    cout<<"Daniel NU joaca sah corect!\n";
                        if((p[ret]->get_moves(predy,predx,viz,p,white)).count({tablecoords_y(curr.y),tablecoords_x(curr.x)})==0)
                            cout<<"Daniel NU joaca sah corect!\n";

                        //cout<<(int)(curr.x/75)<<' '<<(int)(curr.y/68)<<endl;
                        viz[tablecoords_y(curr.y)][tablecoords_x(curr.x)]=viz[p[ret]->get_x()][p[ret]->get_y()];
                        viz[p[ret]->get_x()][p[ret]->get_y()]=0;
                        p[ret]->get_s().setPosition(poscoords_x(curr.x),poscoords_y(curr.y));
                        p[ret]->set_pos(tablecoords_y(curr.y),tablecoords_x(curr.x));

                        //cout<<it.first<<' '<<it.second<<'\n';
                        //p[ret].get_s().setPosition(curr.x,curr.y);
                        //cout<<(int)(curr.y/68)<<' '<<(int)(curr.x/75)<<endl;
                        //afisare(viz);

                    }

                    is_chess=is_in_chess(viz,p,white);

                    //if(sah_dinainte)
                    if(is_in_chess(viz,p,!white))
                    {
                        cout<<"Daniel NU joaca sah corect!\n";
                    }

                    if(is_chess)
                        cout<<"Sah!\n";

                    /*if(is_chess_Mate(viz,p,white))
                    {
                        cout<<"Sah mat!"<<endl;
                    }*/
                    is_chess_Mate(viz,p,white);

                    /*for(auto& it: (p[ret]->get_moves((int)(curr.y/68),(int)(curr.x/75),viz,p,white)) )
                    {
                        //cout<<it.first<<' '<<it.second<<' '<<viz[it.first][it.second]<<'\n';
                        if(fabs(viz[it.first][it.second])==6)
                        {
                            cout<<"Sah la "<<(!white ? "Alb" : "Negru")<<'\n';
                            is_chess=1;
                            break;
                        }
                    }*/
                    //cout<<"Este in sah :"<<(is_in_chess(viz,p,white) ? "Da" : "Nu")<<'\n';
                    Sleep(100);
                    //if(valid)----->de adaugat (pt fiecare tip de piesa verific daca este o mutare valida)
                    //cout<<curr.x<<' '<<curr.y<<' '<<p[i].getPosition().x<<' '<<p[i].getPosition().y<<endl;
                    white=1-white;
                    //cout<<white<<endl;
                    continue;

                }
        }

        /*if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            window.clear();
            window.draw(sprite);
            for(int i=8;i<24;i++)
                window.draw(p[i]->get_s());
            window.display();
        }*/

        //ca sa vad cum merge cu afisarea pieselor
        window.clear();
        window.draw(sprite);
        for(int i=0;i<32;i++)
            window.draw(T.get_p()[i]->get_s());
        window.display();
        window.clear();

        //window.display();

    }

    //ce piesa a fost mutata la a idx-a mutare
    int idx;
    cin>>idx;
    //cout<<T[idx]<<'\n';

    table T1=T;
    cout<<T1[idx];

    for(auto& x:p)
    {
        delete x;
    }
    p.clear();

    return 0;
}
