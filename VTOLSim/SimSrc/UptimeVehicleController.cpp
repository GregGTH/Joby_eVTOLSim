#include "UptimeVehicleController.h"
#include "VehicleFactory.h"
#include "Vehicle.h"
#include "SimConst.h"

using namespace std;

bool UptimeVehicleController::setup(vcParams& params)
{
    VehicleController::setup(params);
    
    // Vehicles start in flight
    for (Vehicle* veh : params.vehicleList)
    {
        operatingVehicles.push_back(veh);
    }

    return true;
}

void UptimeVehicleController::updateController(DWORD deltaTime_ms)
{
    vector<Vehicle*> transitionList;

    // Vehicle transitioning from charging
    getVehDownToUp(transitionList);

    // Instantaneous transition, add vehicles update list
    operatingVehicles.insert(operatingVehicles.end(), transitionList.begin(), transitionList.end());

    // Update vehicles
    transitionList.clear();
    vector<Vehicle*> tmpList;
    for (Vehicle* veh : operatingVehicles)
    {
        veh->updateInAir(deltaTime_ms); // Thread pool for updating vehicles instead?

        // Add vehicle to transition list or keep list
        if (veh->readyToCharge())
            transitionList.push_back(veh);
        else
            tmpList.push_back(veh);
    }

    // Rewrite operating list w/ non-transitioning vehicles
    operatingVehicles = move(tmpList);

    // (Instantaneous transition) add ready vehicles
    if (transitionList.size() > 0)
        addVehUpToDown(transitionList);
}

/**
 *  Take the sync-up lock to wait until the last 'downtime' update is finished.
 */
void UptimeVehicleController::takeSyncSection()
{
    EnterCriticalSection(&_syncUpSection);
}

/**
 *  Release the sync-down lock to signal completion of flight update.
 */
void UptimeVehicleController::releaseSyncSection()
{
    LeaveCriticalSection(&_syncDownSection);
}