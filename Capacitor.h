
#include "Simulator.h"
#include "Battery.h"

class Capacitor : public Device
{
    public:

    // Constructor:
    
    Capacitor(int nodei, int nodej, double C);

    // Device interface:
    
    void Step(double t, double dt);
    void DC();

    // Viewable functions:
    
    double GetVoltage();
    double GetCurrent();
    double GetPower();
    double GetC(int nodei, int nodej, double soc);

    // Member variables:
    
    int nodei;
    int nodej;
    int nodepos;
    double C;
    double Ct2;
};

Capacitor::Capacitor(int nodei, int nodej, double C)
{
    this->nodei = nodei;
    this->nodej = nodej;
    this->nodepos = nodepos;
    this->C = C;
    this->Ct2 = Ct2;
}

void Capacitor::Step(double t, double dt)
{
    double g = C / dt;
    double g2 = Ct2 / dt;
    double b = g * GetStateDifference(nodei, nodej);  // g * v(t)
    double b2 = g2 * GetStateDifference(nodei, nodej);  // g * v(t)

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
double Capacitor::GetC(int nodei,int nodej, double soc)
{
    if (nodei = 3 && nodej = 4)
    {
        return -7522.9 * exp(-13.51 * soc) + 703.6;
    }
    if (nodei == 4 && nodej = nodepos)
    {
        return -6056.0 * exp(-27.12 * soc) + 4475;
    }
}
double Capacitor::GetCt2(double soc)
{
    return -6056.0 * exp(-27.12 * soc) + 4475;
}
