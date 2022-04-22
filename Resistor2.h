
#include "Simulator.h"

class Resistor2 : public Device
{
    public:

    // Constructor:
    
    Resistor2(int nodei, int nodej,double R);

    // Device interface (don't need Init() or DC() functions for resistor):
    
    void Step(double t, double dt);

    // Viewable functions:
    
    double GetVoltage();
    double GetCurrent();
    double GetPower();

    // Member variables:

    int nodei;
    int nodej;
    double R;
    double g;
};

Resistor2::Resistor2(int nodei, int nodej, double R)
{
    this->nodei = nodei;
    this->nodej = nodej;
    this->R = R;
    g = 1 / R;
}

void Resistor2::Step(double t, double dt)
{
    AddJacobian(nodei, nodei, g);
    AddJacobian(nodei, nodej, -g);
    AddJacobian(nodej, nodei, -g);
    AddJacobian(nodej, nodej, g);
}

double Resistor2::GetVoltage()
{
    return GetStateDifference(nodei, nodej);
}

double Resistor2::GetCurrent()
{
    return GetVoltage() * g;
}

double Resistor2::GetPower()
{
    return GetVoltage() * GetCurrent();
}
