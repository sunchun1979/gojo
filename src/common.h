#ifndef _COMMON
#define _COMMON

#include <string>
#include <iomanip>
#include <iostream>
#include "../utils/ltensor/LTensor.h"

static const std::string COLOR[] = {"black", "white"};

#define N_FEATURES 8

enum Features{
    BLACK = 0,
    WHITE,
    LEGAL_BLACK,
    LEGAL_WHITE,
    GROUPSIZE,
    LIBERTY,
    D_EDGE,
    D_CORNER
};

struct XY { int x; int y; };
const XY KERNEL9[] = { {-1,-1}, {0,-1}, {1,-1}, {-1,0}, {0,0}, {1,0}, {-1,1}, {0,1}, {1,1}};

#define STR2POS(a,b) make_pair(a>'I'?a-'A'-1:a-'A',b-'1')
#define POS2STR(a) std::string({char(a.first>7?a.first+'B':a.first+'A'),char(a.second+'1'),'\0'})

static const float KOMI9 = 6.5;

using namespace std;

static void printVec(Marray<float,1>* vec, int bsize, int fsize, string label="")
{
    std::setprecision(4);
    for (int f=0; f<fsize; ++f)
    {
        cout << "[" << label << "] Feature " << f << endl;

        for (int oj=0; oj<bsize; ++oj)
        {
            cout << std::fixed << std::setw(8) << oj;
        }
        cout << endl;

        for (int oi=bsize-1; oi>=0; --oi)
        {
            cout << std::fixed << std::setw(3) << oi;
            for (int oj=0; oj<bsize; ++oj)
            {
                cout << std::fixed << std::setw(8) << std::setprecision(3) << (*vec)(f*bsize*bsize + oj*bsize + oi);
            }
            cout << endl;
        }
        cout << endl;
    }

} 

#endif


