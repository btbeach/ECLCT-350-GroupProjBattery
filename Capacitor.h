
#include "Simulator.h"


class Capacitor : public Device
{
    public:

    // Constructor:
    
    Capacitor(int nodei, int nodej, double A, double k, double a0);

    // Device interface:
    
    void Step(double t, double dt);
    void DC();

    // Viewable functions:
    
    double GetVoltage();
    double GetCurrent();
    double GetPower();
    double GetC(double soc, double A, double k, double a0);

    // Member variables:
    
    int nodei;
    int nodej;
    //int nodepos;
    double C;
    double soc;
    double A;
    double k;
    double a0;
};

Capacitor::Capacitor(int nodei, int nodej, double A, double k, double a0)
{
    this->nodei = nodei;
    this->nodej = nodej;
    //this->nodepos = nodepos;
    this->C = C;
    this->soc = soc;
    this->A = A;
    this->k = k;
    this->a0 = a0;
}

void Capacitor::Step(double t, double dt)
{
    double g = GetC(soc, A, k, a0) / dt;
    double b = g * GetStateDifference(nodei, nodej);  // g * v(t)

    AddJacobian(nodei, nodei, g);
    AddJacobian(nodei, nodej, -g);
    AddJacobian(nodej, nodei, -g);
    AddJacobian(nodej, nodej, g);

    AddBEquivalent(nodei, b);
    AddBEquivalent(nodej, -b);
}

void Capacitor::DC()
{
    // model open circuit with 0 conductance:
    
    AddJacobian(nodei, nodei, 0.0);
    AddJacobian(nodei, nodej, 0.0);
    AddJacobian(nodej, nodei, 0.0);
    AddJacobian(nodej, nodej, 0.0);
}

double Capacitor::GetVoltage()
{
    return GetStateDifference(nodei, nodej);
}

double Capacitor::GetCurrent()
{
    // current = g * v - b:
    return C / GetTimeStep() * GetVoltage() - GetBEquivalent(nodei);
}

double Capacitor::GetPower()
{
    return GetVoltage() * GetCurrent();
}
double Capacitor::GetC(double soc, double A, double k, double a0)
{
    return A * exp(k * soc) + a0;
}

