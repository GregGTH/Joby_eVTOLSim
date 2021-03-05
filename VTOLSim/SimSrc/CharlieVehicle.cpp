#include "CharlieVehicle.h"
#include "SimConst.h"
#include <iostream>

// Init class variables
double CharlieVehicle::_totalTimeInFlight_hr = 0.0;
double CharlieVehicle::_totalTimeInCharge_hr = 0.0;
double CharlieVehicle::_totalTimeInWait_hr = 0.0;
double CharlieVehicle::_totalPassengerTravel_mi = 0.0;
int CharlieVehicle::_totalVehicles = 0;
int CharlieVehicle::_maxFaults = 0;

CharlieVehicle::CharlieVehicle() : Vehicle(160, 220, 0.8, 2.2, 3, 0.05)
{
    ++_totalVehicles;
}

void CharlieVehicle::updateTimeInCharge(DWORD time_ms)
{
    _totalTimeInCharge_hr += MS_TO_HR(time_ms);
}

void CharlieVehicle::updateTimeInWait(DWORD time_ms)
{
    _totalTimeInWait_hr += MS_TO_HR(time_ms);
}

void CharlieVehicle::updateTimeInFlight(DWORD time_ms)
{
    _totalTimeInFlight_hr += MS_TO_HR(time_ms);
}

void CharlieVehicle::updatePassengerTravel(DWORD time_ms)
{
    _totalPassengerTravel_mi += ((double)_cruiseSpeed_mph * MS_TO_HR(time_ms) * (double)_passengerCount);
}

void CharlieVehicle::updateMaxFaults(int faults)
{
    if (faults > _maxFaults)
        _maxFaults = faults;
}

void CharlieVehicle::printData()
{
    // Calculate averages
    double avgTimeInFlight_hrpv = (_totalTimeInFlight_hr / (double)_totalVehicles);
    double avgTimeInCharge_hrpv = (_totalTimeInCharge_hr / (double)_totalVehicles);
    double avgTimeInWait_hrpv = (_totalTimeInWait_hr / (double)_totalVehicles);
    
    // NOTE: total miles and max faults are updated during vehicle update
    std::cout << "--Charlie Vehicle (" << _totalVehicles << ") --\
                    \n\tAvg Flight Time (hr/veh): " << avgTimeInFlight_hrpv << "\
                    \n\tAvg Charge Time (hr/veh): " << avgTimeInCharge_hrpv << "\
                    \n\tAvg Wait Time (hr/veh): " << avgTimeInWait_hrpv << "\
                    \n\tTotal Passenger Distance (mi): " << _totalPassengerTravel_mi << "\
                    \n\tMax Faults: " << _maxFaults << "\n\n";
}