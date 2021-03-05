#include "Vehicle.h"

#ifndef ECHOVEHICLEH
#define ECHOVEHICLEH

class EchoVehicle : public Vehicle {
private:
    // Vehicle E running totals
    static double _totalTimeInFlight_hr;
    static double _totalTimeInCharge_hr;
    static double _totalTimeInWait_hr;
    static double _totalPassengerTravel_mi;
    static int _totalVehicles;
    static int _maxFaults;
    
protected:
    // Overriden functions to update running totals
    void updateTimeInCharge(DWORD time_ms);
    void updateTimeInWait(DWORD time_ms);
    void updateTimeInFlight(DWORD time_ms);
    void updatePassengerTravel(DWORD time_ms);
    void updateMaxFaults(int faults);

public:
    EchoVehicle();

    static void printData();
};

#endif