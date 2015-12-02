#ifndef _NN
#define _NN

#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include "common.h"

#include "../utils/ltensor/LTensor.h"

using namespace std;

class CNN
{
    private:

        int m_layers;
        vector<Marray<float,3>* > m_convLayer;
        Marray<float,2>* m_temp = NULL;
        Marray<float,1>* m_output = NULL;

        int m_size;
        int m_channels;

    public:
        CNN(vector<vector<int>>& shape, int size, int channels);
        ~CNN();
        void forward(Marray<float,1>* input, Marray<float,1>* output);
        void expandConv(Marray<float,1>* input, Marray<float,2>* output);
        void ptTanh(Marray<float,1>* t);

    private:
        void initialize();
        float getKernelInput(Marray<float,1>* input, int shift, int i0, int j0, int size, int k, int kernelSize);

};

#endif
