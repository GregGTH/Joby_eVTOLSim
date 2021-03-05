#include "VehicleController.h"
#include "Vehicle.h"
#include <vector>

#ifndef UPTIMEVEHICLECONTROLLERH
#define UPTIMEVEHICLECONTROLLERH

class UptimeVehicleController : public VehicleController {
private:
    std::vector<Vehicle*> operatingVehicles;

protected:
    // Override, update this instance
    void updateController(DWORD time_ms);
    void takeSyncSection();
    void releaseSyncSection();

    // Override, setup this instance
    bool setup(vcParams& params);
};

#endif