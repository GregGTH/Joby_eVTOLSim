#include "VehicleController.h"
#include "Vehicle.h"
#include "Charger.h"
#include <deque>
#include <vector>

#ifndef DOWNTIMEVEHICLECONTROLLERH
#define DOWNTIMEVEHICLECONTROLLERH

class DowntimeVehicleController : public VehicleController {
private:
    std::deque<Vehicle*> _chargeQueue;
    std::vector<Charger*> _chargers;

protected:
    // Update this instance
    void updateController(DWORD time_ms);
    void takeSyncSection();
    void releaseSyncSection();

public:
    DowntimeVehicleController(int numChargers);
    ~DowntimeVehicleController();
};

#endif