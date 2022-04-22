
//#include "Simulator.h"

//Trouble in GetSOC function when mulitplying by step h

//class Battery : public Device
/*
{
    public:

    // Constructor (Va, f, and phi are optional arguments):

    Battery(int positive, int negative, double wh, double soc0);

    // Device interface:
    
    void Init();
    //void DC();
    void Step(double t, double dt);

    // Viewable functions:
    
    //double GetVoltage();
    //double GetCurrent();
    //double GetPower();
    double GetSOC();
    double GetTerminalVoltage();
    double GetTerminalCurrent();

    // Member Variables:
    
    int positive;
    int negative;
    int nodep;
    double wh;
    double soc0;
    //double f;
    //double phi;

};


Battery::Battery(int positive, int negative, double wh, double soc0)
{
    this->positive = positive;
    this->negative = negative;
    this->nodep = nodep;
    this->wh = wh;
    this->soc0 = soc0;
    //this->f = f;
    //this->phi = phi;
}

void Battery::Init()
{
    // this is where and how to setup internal nodes:
    this->nodep = GetNextNode();
}

void Battery::Step(double t, double dt)
{
    //double v = Vdc + Va * sin(2.0 * M_PI * f * t + phi);
    double v = GetTerminalVoltage();
    
    AddJacobian(positive, nodep, 1.0);
    AddJacobian(negative, nodep, -1.0);
    AddJacobian(nodep, positive, 1.0);
    AddJacobian(nodep, negative, -1.0);
    AddBEquivalent(nodep, v);
}

//void VoltageSource::DC()
//{
//    AddJacobian(positive, nodep, -1.0);
//    AddJacobian(negative, nodep, 1.0);
//    AddJacobian(nodep, positive, -1.0);
//    AddJacobian(nodep, negative, 1.0);
//    AddBEquivalent(nodep, Vdc);
//}

double Battery::GetTerminalVoltage()
{
    return GetStateDifference(positive, negative);
}

double Battery::GetTerminalCurrent()
{
    return -GetState(nodep);
}

double Battery::GetSOC()
{
    return GetSOC() + ((GetTerminalVoltage() * GetTerminalCurrent() * dt) / (wh * 3600));
}

//double Battery::GetTerminalVoltage()
//{
//    return GetVoltage() * GetCurrent();
//}

//double Battery::GetTerminalCurrent()
//{
//    return;
//}
*/
// Simplified Battery Model
//#ifndef Battery_Header
//#define Battery_Header

#include "Simulator.h"
#include <cmath>

class Battery : public Device
{
    /*
               (int1)   R(soc)
             vi  o------VVV---o (nodepos)  vpos
                +|
       Vin(soc) ( ) o (int2) ibatt
                 |
                 '------------o (nodeneg)  vneg
        output vector:

           quantities:   indices:
            [ vpos  ]    nodepos
        x = [ vneg  ]    nodeneg
            [ vi    ]    int1
            [ ibatt ]    int2
    */

public:
    // Constructor (external nodes and params):
    Battery(int nodepos, int nodeneg, double soci);

    // Device interface:
    void Step(double t, double dt);
    void Init();

    // Viewable functions:
    double GetVoltage();
    double GetCurrent();
    double GetSOC();

    // f(soc) functions:
    double GetVin(double soc);
    double GetR(double soc);

    /*double GetRt1(double soc);
    double GetRt2(double soc);
    double GetCt1(double soc);
    double GetCt2(double soc);*/

    // Member variables:
    int nodepos;
    int nodeneg;
    int int1;
    int int2;
    /*int int3;
    int int4;*/
    double soci;

public : double soc;  // state of charge
};
Battery::Battery(int nodepos, int nodeneg, double soci)
{
    this->nodepos = nodepos;
    this->nodeneg = nodeneg;
    this->soci = soci;
}

void Battery::Init()
{
    int1 = GetNextNode();
    int2 = GetNextNode();
    //int2 = 5; internal Vin node 5 or 2
    //int3 = GetNextNode();   //node 3
    //int4 = GetNextNode();   //node 4
    soc = soci;
}
void Battery::Step(double t, double h)
{
    double Vin = GetVin(soc);
    double g = 1 / GetR(soc);
   /* double gR1 = 1 / GetRt1(soc);
    double gR2 = 1 / GetRt2(soc);
    double gC1 = GetCt1(soc) / h;
    double gC2 = GetCt2(soc) / h;*/
    double wh = 8.1;
    // R:
    AddJacobian(nodepos, nodepos, g);
    AddJacobian(nodepos, int1, -g);
    AddJacobian(int1, nodeneg, -g);
    AddJacobian(int1, int1, g);
    //Vin
    AddJacobian(int1, int2, 1);
    AddJacobian(nodeneg, int2, -1);
    AddJacobian(int2, int1, 1);
    AddJacobian(int2, nodeneg, -1);

    AddBEquivalent(int2, Vin);
    ////R
    //AddJacobian(int3, int3, g);
    //AddJacobian(int3, int1, -g);
    //AddJacobian(int1, nodeneg, -g);
    //AddJacobian(int1, int1, g);
    ////Vin
    //AddJacobian(int1, int2, 1);
    //AddJacobian(nodeneg, int2, -1);
    //AddJacobian(int2, int1, 1);
    //AddJacobian(int2, nodeneg, -1);
    //AddBEquivalent(int2, Vin);
    ////Rt1
    //AddJacobian(int3, int3, gR1);
    //AddJacobian(int3, int4, -gR1);
    //AddJacobian(int4, int3, gR1);
    //AddJacobian(int4, int4, -gR1);
    ////Rt2
    //AddJacobian(int4, int4, gR2);
    //AddJacobian(int4, nodepos, -gR2);
    //AddJacobian(nodepos, int4, gR2);
    //AddJacobian(nodepos, nodepos, -gR2);
    ////Ct1
    //AddJacobian(int3, int3, gC1);
    //AddJacobian(int3, int4, -gC1);
    //AddJacobian(int4, int3, gC1);
    //AddJacobian(int4, int4, -gC1);
    ////Ct2 
    //AddJacobian(int4, int4, gC2);
    //AddJacobian(int4, nodepos, -gC2);
    //AddJacobian(nodepos, int4, gC2);
    //AddJacobian(nodepos, nodepos, -gC2);
    // update soc:
    soc = soc + GetVoltage() * GetCurrent() * h / (wh * 3600);
}
double Battery::GetVoltage()
{
    return GetStateDifference(nodepos, nodeneg);
}
double Battery::GetCurrent()
{
    return GetState(int2);
}
double Battery::GetSOC()
{
    return soc;
}
double Battery::GetVin(double soc)
{
    //return 3.8 * soc;  // simple linear model
    return (-1.031) * exp(-35.0 * soc) + 3.685 + 0.2156 * soc +
    -0.1178 * pow(soc, 2) + 0.3201 * pow(soc, 3);
}
double Battery::GetR(double soc)
{
    //return 0.1 + (1 - soc) * 0.01;  // simple linear model
    return 0.1562 * exp(-24.37 * soc) + 0.07446;
}

//#endif;

//double Battery::GetRt1(double soc)
//{
//    return 0.3208 * exp(-24.37 * soc) + 0.04669;
//}
//double Battery::GetRt2(double soc)
//{
//    return 6.603 * exp(-155.2 * soc) + 0.04984;
//}
//double Battery::GetCt1(double soc)
//{
//    return -7522.9 * exp(-13.51 * soc) + 703.6;
//}
//double Battery::GetCt2(double soc)
//{
//    return -6056.0 * exp(-27.12 * soc) + 4475;
//}
