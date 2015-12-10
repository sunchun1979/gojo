#include <cmath>
#include <random>
#include <algorithm>
#include "nn.h"

using namespace std;

CNN::CNN(vector<vector<int>>& shape, int size, int channels)
{
    m_size = size;
    m_channels = channels;
    m_layers = shape.size();
    m_convLayer.resize(m_layers);
    for(int i=0;i<m_layers;++i)
    {
        m_convLayer[i] = new Marray<float,3>(shape[i][0], shape[i][1], shape[i][2]);
    }
    initialize();
    perturb();
}

CNN::CNN(vector<CNN*>& samples, vector<float>& weights)
{
    m_size = samples[0]->m_size;
    m_channels = samples[0]->m_channels;
    m_layers = samples[0]->m_layers;
    m_convLayer.resize(m_layers);
    for(int i=0;i<m_layers;++i)
    {
        int dim1, dim2, dim3;
        dim1 = samples[0]->m_convLayer[i]->get_dim1();
        dim2 = samples[0]->m_convLayer[i]->get_dim2();
        dim3 = samples[0]->m_convLayer[i]->get_dim3();
        m_convLayer[i] = new Marray<float,3>(dim1, dim2, dim3);
    }
    for(int i=0;i<m_layers;++i)
    {
        for(int s=0;s<samples.size();++s)
        {
            Index<'i'> i1;
            Index<'j'> i2;
            Index<'k'> i3;
            (*m_convLayer[i])(i1,i2,i3) += ((*samples[s]->m_convLayer[i])(i1,i2,i3)*weights[s]);
        }
    }
    perturb();
}

CNN::~CNN()
{
    for(int i=0;i<m_layers;++i) delete m_convLayer[i];
    if (m_temp!=NULL) delete m_temp;
    if (m_output!=NULL) delete m_output;
}

void CNN::forward(Marray<float,1>* input, Marray<float,1>* output)
{
    if (m_output!=NULL) delete m_output;
    m_output = new Marray<float,1>(input->get_dim1());
    (*m_output) = (*input);

    for (int i = 0; i < m_layers; ++i)
    {
        if (m_temp!=NULL) delete m_temp;
        m_temp = new Marray<float,2>(m_convLayer[i]->get_dim1(), m_convLayer[i]->get_dim3());

        //conv
        expandConv(m_output, m_temp);

        if (m_output!=NULL) delete m_output;
        m_output = new Marray<float,1>(m_convLayer[i]->get_dim2());

        //forward
        Index<'i'> Ti;
        Index<'j'> Tj;
        Index<'k'> Tk;
        (*m_output)(Tj) = (*m_temp)(Ti, Tk) * (*m_convLayer[i])(Ti, Tj, Tk);

        //printVec(m_output, m_size, m_channels, "Layer " + to_string(i) + " ");

        //non-linear
        if (i<m_layers-1)
            nonlinear(m_output);

    }

    (*output) = (*m_output);
}

void CNN::expandConv(Marray<float,1>* input, Marray<float,2>* output)
{
    // verify dimension
    if (input->get_dim1() != output->get_dim1()) 
    {
        cout << "Assert Error" << endl;
        exit(-1);
    }
    int kernelSize = output->get_dim2();
    for (int ch=0 ; ch<m_channels; ++ch)
    {
        int shift = m_size*m_size*ch;
        for (int i=0;i<m_size;++i)
            for (int j=0;j<m_size;++j)
            {
                int fdim = shift + i*m_size + j; 
                for (int k=0; k<kernelSize; ++k)
                {
                    float v = getKernelInput(input, shift, i, j, m_size, k, kernelSize);
                    (*output)(fdim,k) = v;
                }
            }
    }
}

void CNN::nonlinear(Marray<float,1>* t)
{
    t->normalize();
    for(int i1=0;i1<t->get_dim1();++i1)
    {
        // simple tanh;
        // (*t)(i1) = tanh((*t)(i1));
        // simple relu
        (*t)(i1) = std::max(0.0F,(*t)(i1));
    }
}

float CNN::getKernelInput(Marray<float,1>* input, int shift, int i0, int j0, int size, int k, int kernelSize)
{
    if (kernelSize==9)
    {
        int i1 = i0 + KERNEL9[k].x;
        int j1 = j0 + KERNEL9[k].y;
        if ( (i1>=0) && (i1<size) && (j1>=0) && (j1<size) )
        {
            return (*input)(shift + i1*size + j1);
        }
        else
        {
            return 0;
        }
    }else
    {
        return 0;
    }
}

void CNN::initialize()
{
    for(int i=0;i<m_layers;++i)
    {
        for(int i1=0;i1<m_convLayer[i]->get_dim1();++i1)
            for(int i2=0;i2<m_convLayer[i]->get_dim2();++i2)
                for(int i3=0;i3<m_convLayer[i]->get_dim3();++i3)
                {
                    float v = 1;
                    if (rand() < RAND_MAX/2) v = -1;
                    (*m_convLayer[i])(i1,i2,i3) = v;
                }
    }
}

void CNN::perturb()
{
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(0,0.5);
    for(int i=0;i<m_layers;++i)
    {
        for(int i1=0;i1<m_convLayer[i]->get_dim1();++i1)
            for(int i2=0;i2<m_convLayer[i]->get_dim2();++i2)
                for(int i3=0;i3<m_convLayer[i]->get_dim3();++i3)
                {
                    float v = distribution(generator);
                    (*m_convLayer[i])(i1,i2,i3) += v;
                }
    }
}

void CNN::saveTo(string fname)
{
    ofstream out;
    out.open(fname,ios::binary);
    out.write(reinterpret_cast<char*>(&m_layers),sizeof(int));
    out.write(reinterpret_cast<char*>(&m_size),sizeof(int));
    out.write(reinterpret_cast<char*>(&m_channels),sizeof(int));
    for(int i=0;i<m_layers;++i)
    {
        int temp;
        temp = m_convLayer[i]->get_dim1(); out.write(reinterpret_cast<char*>(&temp),sizeof(int));
        temp = m_convLayer[i]->get_dim2(); out.write(reinterpret_cast<char*>(&temp),sizeof(int));
        temp = m_convLayer[i]->get_dim3(); out.write(reinterpret_cast<char*>(&temp),sizeof(int));
        float value;
        for(int i1=0;i1<m_convLayer[i]->get_dim1();++i1)
            for(int i2=0;i2<m_convLayer[i]->get_dim2();++i2)
                for(int i3=0;i3<m_convLayer[i]->get_dim3();++i3)
                {
                    value = (*m_convLayer[i])(i1,i2,i3);
                    out.write(reinterpret_cast<char*>(&value),sizeof(float));
                } 
    }
    out.close();
}

void CNN::loadFrom(string fname)
{
    for(int i=0;i<m_layers;++i) delete m_convLayer[i];
    ifstream in;
    in.open(fname,ios::binary);
    in.read(reinterpret_cast<char*>(&m_layers), sizeof(int));
    in.read(reinterpret_cast<char*>(&m_size), sizeof(int));
    in.read(reinterpret_cast<char*>(&m_channels), sizeof(int));
    cout << m_layers << " " << m_size << " " << m_channels << endl;
    m_convLayer.resize(m_layers);

    for(int i=0;i<m_layers;++i)
    {
        int dim[3];
        for(int j=0;j<3;++j)
        {
            in.read(reinterpret_cast<char*>(&dim[j]),sizeof(int));
        }
        cout << "[" << i << "]: " << dim[0] << " " << dim[1] << " " << dim[2] << endl;
        m_convLayer[i] = new Marray<float,3>(dim[0], dim[1], dim[2]);
        float value;
        for(int i1=0;i1<m_convLayer[i]->get_dim1();++i1)
            for(int i2=0;i2<m_convLayer[i]->get_dim2();++i2)
                for(int i3=0;i3<m_convLayer[i]->get_dim3();++i3)
                {
                    in.read(reinterpret_cast<char*>(&value),sizeof(float));
                    (*m_convLayer[i])(i1,i2,i3) = value;
                } 
    }
    in.close();
}

