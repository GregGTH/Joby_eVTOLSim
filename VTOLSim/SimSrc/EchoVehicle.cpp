#include "EchoVehicle.h"
#include "SimConst.h"
#include <iostream>

// Class variables for final calculations
double EchoVehicle::_totalTimeInFlight_hr = 0.0;
double EchoVehicle::_totalTimeInCharge_hr = 0.0;
double EchoVehicle::_totalTimeInWait_hr = 0.0;
double EchoVehicle::_totalPassengerTravel_mi = 0.0;
int EchoVehicle::_totalVehicles = 0;
int EchoVehicle::_maxFaults = 0;

EchoVehicle::EchoVehicle() : Vehicle(30, 150, 0.3, 5.8, 2, 0.61)
{
    ++_totalVehicles;
}

void EchoVehicle::updateTimeInCharge(DWORD time_ms)
{
    _totalTimeInCharge_hr += MS_TO_HR(time_ms);
}

void EchoVehicle::updateTimeInWait(DWORD time_ms)
{
    _totalTimeInWait_hr += MS_TO_HR(time_ms);
}

void EchoVehicle::updateTimeInFlight(DWORD time_ms)
{
    _totalTimeInFlight_hr += MS_TO_HR(time_ms);
}

void EchoVehicle::updatePassengerTravel(DWORD time_ms)
{
    _totalPassengerTravel_mi += ((double)_cruiseSpeed_mph * MS_TO_HR(time_ms) * (double)_passengerCount);
}

void EchoVehicle::updateMaxFaults(int faults)
{
    if (faults > _maxFaults)
        _maxFaults = faults;
}

void EchoVehicle::printData()
{
    // Calculate averages
    double avgTimeInFlight_hrpv = (_totalTimeInFlight_hr / (double)_totalVehicles);
    double avgTimeInCharge_hrpv = (_totalTimeInCharge_hr / (double)_totalVehicles);
    double avgTimeInWait_hrpv = (_totalTimeInWait_hr / (double)_totalVehicles);
    
    // NOTE: total miles and max faults are updated during vehicle update
    std::cout << "--Echo Vehicle (" << _totalVehicles << ") --\
                    \n\tAvg Flight Time (hr/veh): " << avgTimeInFlight_hrpv << "\
                    \n\tAvg Charge Time (hr/veh): " << avgTimeInCharge_hrpv << "\
                    \n\tAvg Wait Time (hr/veh): " << avgTimeInWait_hrpv << "\
                    \n\tTotal Passenger Distance (mi): " << _totalPassengerTravel_mi << "\
                    \n\tMax Faults: " << _maxFaults << "\n\n";
}