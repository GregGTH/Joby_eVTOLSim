#include "Charger.h"

/**
 *  Attempt to add a vehicle to this charger.
 *  If in use, veh is unchanged.
 */
bool Charger::tryPutVehicle(Vehicle* veh)
{
    // Charger in use or invalid vehicle given
    if (_vehicle || !veh)
        return false;
    
    _vehicle = veh;
    return true;
}

/**
 *  Attempt to get and remove the vehicle currently occupying this charger.
 *  If charger is not in use/vehicle is not ready, veh is unchanged.
 */
bool Charger::tryGetVehicle(Vehicle** veh)
{
    // charger in use and vehicle is done, update
    if (_vehicle && _vehicle->doneCharging())
    {
        // Update vehicle pointer and free this charger for use
        *veh = _vehicle;
        _vehicle = nullptr;
        return true;
    }
    
    return false;
}

void Charger::update(DWORD deltaTime_ms)
{
    if (_vehicle)
        _vehicle->updateCharge(deltaTime_ms);
}

Vehicle* Charger::getVehicle()
{
    return _vehicle;
}