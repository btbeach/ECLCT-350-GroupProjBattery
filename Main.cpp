
#include "Simulator.h"
#include "Plotter.h"

#include "VoltageSource.h"
#include "Diode.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Battery.h"

/*

  Example circuit (rectifier):

     (1)   D1      (2)
       .--->|---.-------.
      +|        |       |     +
  vin (~)    R1 <   C1 ===   vout
       |        |       |     -
       '--------+-------'
               -+- (0)
                '

  time step = 1e-5 s
  simulation time = 5 ms
  
  vin = 10 * sin(2*pi*1000*t)
  R1 = 10 Ohm
  C1 = 1.0 mF

*/

int main()
{
	const double h = 1e-6;
	const double tmax = 5e-3;
	const double SOCi = 0.9;
	const double wh = 8.1;

	//const double Va = 10;
	//const double f = 1000;
	//const double R = 10;
	//const double C = 1e-3;

	Plotter plotter("Project", 1000, 600);
	plotter.SetLabels("vBatt (V)", "iBatt (A)", "SOC");

	Simulator simulator(6, 0);

	//VoltageSource V1(1, 0, 0, Va, f);
	//Diode D1(1, 2);
	//Resistor Rin(1, 2, Rin);
	//Resistor Rt1(2, 3, R);
	//Capacitor Ct1(2, 0, C);
	//Resistor Rt2(3, 4, R);
	//Capacitor Ct2(3, 4, C);
	Battery Batt(4, 0, 0.9);

	//simulator.AddDevice(V1);
	//simulator.AddDevice(Rin);
	//simulator.AddDevice(Rt1);
	//simulator.AddDevice(Ct1);
	//simulator.AddDevice(Rt2);
	//simulator.AddDevice(Ct2);
	simulator.AddDevice(Batt);

	simulator.Init(h, tmax);

	while (simulator.IsRunning())
	{
		plotter.AddRow(simulator.GetTime(), Batt.GetVoltage(),
			Batt.GetCurrent(), Batt.GetSOC()); 
		//, C1.GetVoltage());
		
		simulator.Step();
	}

	plotter.Plot();

	return 0;
}