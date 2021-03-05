#include "DowntimeVehicleController.h"

using namespace std;

DowntimeVehicleController::DowntimeVehicleController(int numChargers)
{
    // Create/store chargers
    for (int i = 0; i < numChargers; ++i)
    {
        _chargers.push_back(new Charger());
    }
}

DowntimeVehicleController::~DowntimeVehicleController()
{
    // Cleanup
    for (Charger* chgr : _chargers)
    {
        delete chgr;
    }
}

void DowntimeVehicleController::updateController(DWORD deltaTime_ms)
{
    vector<Vehicle*> tmpList;

    // Get all vehicles transitioning from flight
    getVehUpToDown(tmpList);

    // Instantaneous transition, add vehicles to charging queue
    _chargeQueue.insert(_chargeQueue.end(), tmpList.begin(), tmpList.end());

    // Update the chargers
    tmpList.clear();
    for (Charger *chgr : _chargers)
    {
        if (!_chargeQueue.empty() && chgr->tryPutVehicle(_chargeQueue.front()))
            _chargeQueue.pop_front();

        // implicitly updates vehicle
        chgr->update(deltaTime_ms); // Thread pool for updating vehicles instead?

        // If vehicle is done, transition it
        // NOTE: nullptr check in tryGetVehicle()
        Vehicle* veh;
        if (chgr->tryGetVehicle(&veh))
            tmpList.push_back(veh);
    }

    // Update any vehicles left in the charging queue
    for (Vehicle* veh : _chargeQueue)
    {
        veh->updateWaitForCharge(deltaTime_ms);
    }

    // Instantaneous transition
    if (tmpList.size() > 0)
        addVehDownToUp(tmpList);
}

/**
 *  Take the sync-down lock to wait until the last 'uptime' update is finished.
 */
void DowntimeVehicleController::takeSyncSection()
{
    EnterCriticalSection(&_syncDownSection);
}

/**
 *  Release the sync-up lock to signal completion of charge update.
 */
void DowntimeVehicleController::releaseSyncSection()
{
    LeaveCriticalSection(&_syncUpSection);
}