#include "Vehicle.h"
#include "SimConst.h"
#include "AlphaVehicle.h"
#include "BravoVehicle.h"
#include "CharlieVehicle.h"
#include "DeltaVehicle.h"
#include "EchoVehicle.h"
#include <stdlib.h>

Vehicle::Vehicle(int cs, int bc, double ttc, double eac, int pc, double fph) :
        _cruiseSpeed_mph(cs), _energyAtCruise_kwhpmi(eac), _passengerCount(pc),
        _faultPerHour_rat(fph), _numFaults(0), _remainingTime_ms(0)
{
    _battery = new Battery(bc, ttc);
}

Vehicle::~Vehicle()
{
    delete _battery;
}

void Vehicle::updateInAir(DWORD deltaTime_ms)
{
    // Vehicle may have leftover time from transition, account for that here
    DWORD simTime_ms = deltaTime_ms + _remainingTime_ms;
    double battChargeUsed_kwh = (double)_cruiseSpeed_mph * MS_TO_HR(simTime_ms) * _energyAtCruise_kwhpmi;
    double chargeLeft_kwh = 0.0;

    chargeLeft_kwh = _battery->drain(battChargeUsed_kwh);
        
    // Back-calculate remaining time from leftover charge
    _remainingTime_ms = (DWORD)HR_TO_MS(chargeLeft_kwh / (double)_cruiseSpeed_mph / _energyAtCruise_kwhpmi);
    
    // if battery is empty, don't update time in air (instantaneous transition)
    if (_battery && !_battery->needToCharge())
    {
        updateTimeInFlight(simTime_ms - _remainingTime_ms);
        updatePassengerTravel(simTime_ms - _remainingTime_ms);
    }

    checkForFault(MS_TO_HR(simTime_ms - _remainingTime_ms));
}

void Vehicle::updateCharge(DWORD deltaTime_ms)
{
    // Vehicle may have leftover time from transition, account for that here
    DWORD simTime_ms = deltaTime_ms + _remainingTime_ms;
    double timeLeft_hr = 0.0;
    
    timeLeft_hr = _battery->charge(MS_TO_HR((double)simTime_ms));
    _remainingTime_ms = (DWORD)HR_TO_MS(timeLeft_hr);
    
    // if battery is full, don't update time in charger (instantaneous transition)
    if (!_battery->isFullyCharged())
        updateTimeInCharge(simTime_ms - _remainingTime_ms);

    checkForFault(MS_TO_HR(simTime_ms - _remainingTime_ms));
}

void Vehicle::updateWaitForCharge(DWORD deltaTime_ms)
{
    // Vehicle may have leftover time from transition, account for that here
    DWORD simTime_ms = deltaTime_ms + _remainingTime_ms;
    
    _remainingTime_ms = 0;
    
    // Running totals update
    updateTimeInWait(simTime_ms);
    checkForFault(MS_TO_HR(simTime_ms));
}

bool Vehicle::readyToCharge()
{
    return _battery ? _battery->needToCharge() : false;
}

bool Vehicle::doneCharging()
{
    return _battery ? _battery->isFullyCharged() : false;
}

/**
 *  rough approximation of event probability. 
 *  Updates this vehicle's numFaults as well as the max for the vehicle type
 */
void Vehicle::checkForFault(double time_hr)
{
    const int factor = 100000;   // arbitrary factor to normalize to
    double r = (rand() / (double)RAND_MAX) * (double)factor;    // get random # from 0 -> factor-1)
    double p = (_faultPerHour_rat * time_hr * (double)factor);  // prob for given time slice, normalized to factor
    
    // event happens if r falls below p
    if (r < p)
        ++_numFaults;

    updateMaxFaults(_numFaults);
}

void Vehicle::printData()
{
    AlphaVehicle::printData();
    BravoVehicle::printData();
    CharlieVehicle::printData();
    DeltaVehicle::printData();
    EchoVehicle::printData();
}

Battery* Vehicle::getBattery()
{
    return _battery;
}