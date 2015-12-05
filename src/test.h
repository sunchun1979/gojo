#include <cstdio>
#include <iostream>
#include <ctime>

#include "common.h"
#include "player.h"
#include "gameEngine.h"
#include "evolEngine.h"

class Test
{
    public:
        Test(){}
        ~Test(){}

        void Run()
        {
            //TestSpaceContinuity();
            //TestPlayerIO();
            //TestInteractive();
            //TestTensorAvg();
            //TestNN();
            TestPlayer();
        }

        void TestSpaceContinuity()
        {
            int N = 11;
            //srand(0);
            int inConsistent = 0;

            GameEngine ge1(9);
            for (int g = 0; g<100; g++)
            {
                Player p1, p2, p3;
                int bg12 = 0;
                cout << g << endl << " [1 vs 2] ";
                {
                    for (int i=0;i<N;++i)
                    {
                        cout << i << " ";
                        if (ge1.game(&p1,&p2)) bg12++;
                    }
                }
                cout << endl << " [1 vs 3] ";
                int bg13 = 0;
                {
                    for (int i=0;i<N;++i)
                    {
                        cout << i << " ";
                        if (ge1.game(&p1,&p3)) bg13++;
                    }
                }
                cout << endl << " [2 vs 3] ";
                int bg23 = 0;
                {
                    for (int i=0;i<N;++i)
                    {
                        cout << i << " ";
                        if (ge1.game(&p2,&p3)) bg23++;
                    }
                }
                cout << endl;
                cout << bg12 << " " << bg13 << " " << bg23 << endl ;//<< ge1.execute("showboard") << endl;
                if ( (bg12 >= 16) && (bg23 >= 16) && (bg13 < 16) )
                {
                    inConsistent ++;
                }
                if ( (bg12 < 16) && (bg23 < 16) && (bg13 >= 16) )
                {
                    inConsistent ++;
                }
            }
            cout << "Total Inconsistency: " << inConsistent << endl;
            cout.flush();
        }

        void TestPlayerIO()
        {
            Player a;
            a.saveTo("saved_nn.bin");
            a.loadFrom("saved_nn.bin");
        }

        void TestInteractive(string fname)
        {
            Player a;
            a.loadFrom(fname);
            GameEngine ge1(9);
            ge1.game_human(&a, BLACK);
        }

        void TestTensorAvg()
        {
            Marray<float,2> A(2,2);
            A(0,0) = 1; A(0,1) = 2; A(1,0) = 3; A(1,1) = 4;
            Marray<float,2> B(2,2);
            B(0,0) = 10; B(0,1) = 20; B(1,0) = 30; B(1,1) = 40;
            Marray<float,2> C(2,2);
            Index<'i'> i;
            Index<'j'> j;
            C(i,j) = A(i,j) * 0.5 + B(i,j) * 0.5;
            cout << A << endl << B << endl << C << endl;
        }

        void TestNN()
        {
            vector<vector<int>> shape(4);
            // shape (input, output, kernel)
            shape[0] = {1210,1210,9};
            shape[1] = {1210,1210,9};
            shape[2] = {1210,1210,9};
            shape[3] = {1210,121,9};
            vector<CNN*> vn(3);
            vn[0] = new CNN(shape, 11, 10);
            vn[1] = new CNN(shape, 11, 10);
            vn[2] = new CNN(shape, 11, 10);
            vector<float> weights = { 0.5, 0.2, 0.2, 0.1 };
            CNN* nn = new CNN(vn, weights);
            delete nn;
            delete vn[0];
            delete vn[1];
            delete vn[2];
        }

        void TestPlayer()
        {
            vector<Player*> candidates(6);
            vector<float> weights = { 0.5, 0.1, 0.1, 0.1, 0.1, 0.1 };
            for(int i=0;i<candidates.size();++i)
            {
                candidates[i] = new Player();
            }
            Player* newPlayer = new Player(candidates, weights);

            for(int i=0;i<candidates.size();++i)
            {
                delete candidates[i];
            }
            delete newPlayer;
        }
};
