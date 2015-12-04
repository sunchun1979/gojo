#include <cstdio>
#include <iostream>
#include <ctime>

#include "common.h"
#include "gameEngine.h"
#include "player.h"

using namespace std;

int main(int argc, char* argv[])
{
    srand(time(0));
    int N = 31;
    //srand(0);
    int inConsistent = 0;
    for (int g = 0; g<100; g++)
    {
        Player p1, p2, p3;
        int bg12 = 0;
        cout << g << endl << " [1 vs 2] ";
        {
            GameEngine ge1(9);
            for (int i=0;i<N;++i)
            {
                cout << i << " ";
                if (ge1.game(&p1,&p2)) bg12++;
            }
        }
        cout << endl << " [1 vs 3] ";
        int bg13 = 0;
        {
            GameEngine ge1(9);
            for (int i=0;i<N;++i)
            {
                cout << i << " ";
                if (ge1.game(&p1,&p3)) bg13++;
            }
        }
        cout << endl << " [2 vs 3] ";
        int bg23 = 0;
        {
            GameEngine ge1(9);
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

