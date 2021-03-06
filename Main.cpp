
#include "Simulator.h"
#include "Plotter.h"

#include "VoltageSource.h"
#include "Diode.h"
#include "Resistor.h"
#include "Resistor2.h"
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
	const double h = 1e-1;
	const double tmax = 300.0;
	const double SOCi = 0.9;
	const double wh = 8.1;

	//const double Va = 10;
	//const double f = 1000;
	//const double R = 10;
	//const double C = 1e-3;

	Plotter plotter("Project", 1000, 600);
	plotter.SetLabels("vBatt (V)", "iBatt (A)", "SOC");

	Simulator simulator(3, 0);

	//VoltageSource V1(1, 0, 0, Va, f);
	//Diode D1(1, 2);
	//Resistor Rin(1, 2, Rin);
	Battery Batt(1, 0, 0.9);
	Resistor Rt1(1, 2, 0.3208, -29.14, 0.04669);
	Capacitor Ct1(1, 2, -752.9, -13.51, 703.6);
	Resistor Rt2(2, 3, 6.603, -155.2, 0.04984);
	Capacitor Ct2(2, 3, -6056.0, -27.12, 4475.0);

	Resistor2 Rload(3, 0, 10.0);

	

	//simulator.AddDevice(V1);
	//simulator.AddDevice(Rin);
	simulator.AddDevice(Rt1);
	simulator.AddDevice(Ct1);
	simulator.AddDevice(Rt2);
	simulator.AddDevice(Ct2);
	simulator.AddDevice(Batt);

	simulator.AddDevice(Rload);

	simulator.Init(h, tmax);

	while (simulator.IsRunning())
	{
		Rt1.soc = Batt.soc;
		Ct1.soc = Batt.soc;
		Rt2.soc = Batt.soc;
		Ct2.soc = Batt.soc;

		plotter.AddRow(simulator.GetTime(), Batt.GetVoltage(),
			-1 * Batt.GetCurrent(), Batt.GetSOC());

		
		simulator.Step();
	}

	plotter.Plot();

	return 0;
}