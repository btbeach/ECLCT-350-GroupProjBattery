
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
	Resistor Rt1(1, 2, 0.3208, -29.14, 0.04669);
	Capacitor Ct1(1, 2, -752.9, -13.51, 703.6);
	Resistor Rt2(3, 4, 6.603, -155.2, 0.04984);
	Capacitor Ct2(3, 4, -6056.0, -27.12, 4475.0);
	Battery Batt(2, 0, 0.9);

	//simulator.AddDevice(V1);
	//simulator.AddDevice(Rin);
	simulator.AddDevice(Rt1);
	simulator.AddDevice(Ct1);
	simulator.AddDevice(Rt2);
	simulator.AddDevice(Ct2);
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