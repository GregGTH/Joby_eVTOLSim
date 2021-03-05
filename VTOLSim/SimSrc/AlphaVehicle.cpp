#include "AlphaVehicle.h"
#include "SimConst.h"
#include <iostream>

// Init class variables
double AlphaVehicle::_totalTimeInFlight_hr = 0.0;
double AlphaVehicle::_totalTimeInCharge_hr = 0.0;
double AlphaVehicle::_totalTimeInWait_hr = 0.0;
double AlphaVehicle::_totalPassengerTravel_mi = 0.0;
int AlphaVehicle::_totalVehicles = 0;
int AlphaVehicle::_maxFaults = 0;

AlphaVehicle::AlphaVehicle() : Vehicle(120, 320, 0.6, 1.6, 4, 0.25)
{
    ++_totalVehicles;
}

void AlphaVehicle::updateTimeInCharge(DWORD time_ms)
{
    _totalTimeInCharge_hr += MS_TO_HR(time_ms);
}

void AlphaVehicle::updateTimeInWait(DWORD time_ms)
{
    _totalTimeInWait_hr += MS_TO_HR(time_ms);
}

void AlphaVehicle::updateTimeInFlight(DWORD time_ms)
{
    _totalTimeInFlight_hr += MS_TO_HR(time_ms);
}

void AlphaVehicle::updatePassengerTravel(DWORD time_ms)
{
    _totalPassengerTravel_mi += ((double)_cruiseSpeed_mph * MS_TO_HR(time_ms) * (double)_passengerCount);
}

void AlphaVehicle::updateMaxFaults(int faults)
{
    if (faults > _maxFaults)
        _maxFaults = faults;
}

void AlphaVehicle::printData()
{
    // Average calculations
    double avgTimeInFlight_hrpv = (_totalVehicles > 0) ? (_totalTimeInFlight_hr / (double)_totalVehicles) : 0;
    double avgTimeInCharge_hrpv = (_totalVehicles > 0) ? (_totalTimeInCharge_hr / (double)_totalVehicles) : 0;
    double avgTimeInWait_hrpv = (_totalVehicles > 0) ? (_totalTimeInWait_hr / (double)_totalVehicles) : 0;

    // NOTE: total miles and max faults are updated during vehicle update
    std::cout << "--Alpha Vehicle (" << _totalVehicles << ") --\
                    \n\tAvg Flight Time (hr/veh): " << avgTimeInFlight_hrpv <<
                    "\n\tAvg Charge Time (hr/veh): " << avgTimeInCharge_hrpv <<
                    "\n\tAvg Wait Time (hr/veh): " << avgTimeInWait_hrpv <<
                    "\n\tTotal Passenger Distance (mi): " << _totalPassengerTravel_mi <<
                    "\n\tMax Faults: " << _maxFaults << "\n\n";
}

/**
 *  Sets vehicle's values
 */
void setValues(int cs, int bc, double ttc, double eac, int pc, double fph)
{

}