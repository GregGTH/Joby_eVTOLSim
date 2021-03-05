#include "DowntimeVehicleController.h"
#include "UptimeVehicleController.h"
#include "Vehicle.h"
#include "VehicleFactory.h"
#include "SimConst.h"
#include <vector>
#include <time.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main()
{
    // Random seed
    srand(time(NULL));

    vector<HANDLE> threadHandles;
    vector<VehicleController::vcParams> threadParams;
    vector<VehicleController*> vehControllers;
    vector<Vehicle*> vehicles;
    VehicleFactory factory;

    for (int i = 0; i < NUM_SIM_VEHICLES; ++i)
    {
        if (Vehicle* veh = factory.createRandomVehicle())
            vehicles.push_back(veh);
            
    }

    // Create controller threads
    vehControllers.push_back(new UptimeVehicleController());
    vehControllers.push_back(new DowntimeVehicleController(3)); // create w/ 3 chargers available

    // Create thread parameters
    for (VehicleController* vc : vehControllers)
    {
        threadParams.push_back(VehicleController::vcParams(*vc, vehicles, TOTAL_SIM_TIME_MS, SIM_TIME_FACTOR));
    }

    // Start sim
    for (int i = 0; i < threadParams.size(); ++i)
    {
        DWORD threadId;

        // Create thread, store handle (don't need ID)
        HANDLE thread = CreateThread(NULL, 0, VehicleController::start, &threadParams[i], 0, &threadId);

        // Error creating thread, exit process (implicitly cleans up threads/memory)
        if (thread == NULL)
            ExitProcess(1);

        threadHandles.push_back(thread);
    }

    // Wait for threads to finish
    WaitForMultipleObjects(threadHandles.size(), &threadHandles[0], TRUE, INFINITE);

    // Cleanup
    for (VehicleController* vc : vehControllers)
        delete vc;

    for (Vehicle* veh : vehicles)
        delete veh;

    for (HANDLE h : threadHandles)
        CloseHandle(h);

    // Print vehicle sim data
    Vehicle::printData();
    system("pause");

    return 0;
}