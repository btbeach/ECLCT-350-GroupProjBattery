
#include "Simulator.h"
#include "Battery.h"

class Resistor : public Device
{
    public:

    // Constructor:
    
    Resistor(int nodei, int nodej,double A, double k, double a0);

    // Device interface (don't need Init() or DC() functions for resistor):
    
    void Step(double t, double dt);

    // Viewable functions:
    
    double GetVoltage();
    double GetCurrent();
    double GetPower();
    double GetR(double soc, double A, double k, double a0);

    // Member variables:

    int nodei;
    int nodej;
    double R;
    double g;
    double A;
    double soc;
    double k;
    double a0;
};

Resistor::Resistor(int nodei, int nodej,double A, double k, double a0)
{
    this->nodei = nodei;
    this->nodej = nodej;
    //this->R = R;
    this->A = A;
    this->soc = soc;
    this->k = k;
    this->a0 = a0;

    g = 1.0 / GetR(soc, A, k, a0);
}

void Resistor::Step(double t, double dt)
{
    AddJacobian(nodei, nodei, g);
    AddJacobian(nodei, nodej, -g);
    AddJacobian(nodej, nodei, -g);
    AddJacobian(nodej, nodej, g);
}

double Resistor::GetVoltage()
{
    return GetStateDifference(nodei, nodej);
}

double Resistor::GetCurrent()
{
    return GetVoltage() * g;
}

double Resistor::GetPower()
{
    return GetVoltage() * GetCurrent();
}

double Resistor::GetR(double soc, double A, double k, double a0)
{
    return A * exp(k * soc) + a0;
}
