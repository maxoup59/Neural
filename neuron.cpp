#include "neuron.h"
#include <Qdebug>
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
        total = (float)(input[i]*coeff[i]);
        //qDebug() << QString::number(input[i]);
    }
    output = 1/(1+qExp(-total));
    output -= 0.5;
    output *= 2;
    //qDebug() << QString::number(output);
    return output;
}

