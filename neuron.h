#ifndef NEURON_H
#define NEURON_H
#include <QVector>
#include <QtMath>
class Neuron
{
public:
    Neuron();
    ~Neuron();
    void setInput(QVector<float> pInput){input = pInput;};
    int getOutput() {return output;};
    void setCoeff(QVector<float> pCoeff){coeff = pCoeff;};
    QVector<float> getCoeff() {return coeff;};
    float think();
private:
    QVector<float> input;
    QVector<float> coeff;
    int output;
};

#endif // NEURON_H
