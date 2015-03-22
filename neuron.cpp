#include "neuron.h"

Neuron::Neuron()
{

}

Neuron::~Neuron()
{

}

float Neuron::think()
{
    float total = 0;
    for(int i = 0 ;i < input.length() ; i ++)
    {
        total += input[i]*coeff[i];
    }
    output = 1/(1+qExp(-total));
    return output;
}

