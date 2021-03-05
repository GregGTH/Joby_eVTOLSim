#include "BravoVehicle.h"
#include "SimConst.h"
#include <iostream>

// Init class variables
double BravoVehicle::_totalTimeInFlight_hr = 0.0;
double BravoVehicle::_totalTimeInCharge_hr = 0.0;
double BravoVehicle::_totalTimeInWait_hr = 0.0;
double BravoVehicle::_totalPassengerTravel_mi = 0.0;
int BravoVehicle::_totalVehicles = 0;
int BravoVehicle::_maxFaults = 0;

BravoVehicle::BravoVehicle() : Vehicle(100, 100, 0.2, 1.5, 5, 0.10)
{
    ++_totalVehicles;
}

void BravoVehicle::updateTimeInCharge(DWORD time_ms)
{
    _totalTimeInCharge_hr += MS_TO_HR(time_ms);
}

void BravoVehicle::updateTimeInWait(DWORD time_ms)
{
    _totalTimeInWait_hr += MS_TO_HR(time_ms);
}

void BravoVehicle::updateTimeInFlight(DWORD time_ms)
{
    _totalTimeInFlight_hr += MS_TO_HR(time_ms);
}

void BravoVehicle::updatePassengerTravel(DWORD time_ms)
{
    _totalPassengerTravel_mi += ((double)_cruiseSpeed_mph * MS_TO_HR(time_ms) * (double)_passengerCount);
}

void BravoVehicle::updateMaxFaults(int faults)
{
    if (faults > _maxFaults)
        _maxFaults = faults;
}

void BravoVehicle::printData()
{
    // Average calculations
    double avgTimeInFlight_hrpv = (_totalTimeInFlight_hr / (double)_totalVehicles);
    double avgTimeInCharge_hrpv = (_totalTimeInCharge_hr / (double)_totalVehicles);
    double avgTimeInWait_hrpv = (_totalTimeInWait_hr / (double)_totalVehicles);

    // NOTE: total miles and max faults are updated during vehicle update
    std::cout << "--Bravo Vehicle (" << _totalVehicles << ") --\
                    \n\tAvg Flight Time (hr/veh): " << avgTimeInFlight_hrpv << "\
                    \n\tAvg Charge Time (hr/veh): " << avgTimeInCharge_hrpv << "\
                    \n\tAvg Wait Time (hr/veh): " << avgTimeInWait_hrpv << "\
                    \n\tTotal Passenger Distance (mi): " << _totalPassengerTravel_mi << "\
                    \n\tMax Faults: " << _maxFaults << "\n\n";
}