#include "Vehicle.h"

#ifndef CHARLIEVEHICLEH
#define CHARLIEVEHICLEH

class CharlieVehicle : public Vehicle {
private:
    // Vehicle C running totals
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
    CharlieVehicle();

    static void printData();
};

#endif