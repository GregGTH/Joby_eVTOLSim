#include "Battery.h"
#include <Windows.h>

#ifndef VEHICLEH
#define VEHICLEH

class Vehicle {
private:
    Battery* _battery;
    int _numFaults;
    DWORD _remainingTime_ms;    // Remaining time from last update (i.e. transition before using all of delta time)

    void checkForFault(double time_hr);

protected:
    // Performance constants
    const int _cruiseSpeed_mph;
    const double _energyAtCruise_kwhpmi;
    const int _passengerCount;
    const double _faultPerHour_rat;

    // Update running totals
    virtual void updateTimeInCharge(DWORD time_ms) = 0;
    virtual void updateTimeInWait(DWORD time_ms) = 0;
    virtual void updateTimeInFlight(DWORD time_ms) = 0;
    virtual void updatePassengerTravel(DWORD time_ms) = 0;
    virtual void updateMaxFaults(int faults) = 0;
    
    Vehicle(int cs, int bc, double ttc, double eac, int pc, double fph);
    
public:    
    virtual ~Vehicle();

    // Generic vehicle updates
    void updateInAir(DWORD time_ms);
    void updateCharge(DWORD time_ms);
    void updateWaitForCharge(DWORD time_ms);

    // State transition flag getters
    bool readyToCharge();
    bool doneCharging();

    static void printData();

    // testing
    Battery* getBattery();
};

#endif