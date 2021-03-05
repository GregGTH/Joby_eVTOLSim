#include "Vehicle.h"

class Charger
{
private:
    Vehicle* _vehicle;

public:
    Charger() : _vehicle(nullptr) {}
    
    void update(DWORD deltaTime_ms);

    // Modify charger state
    bool tryPutVehicle(Vehicle* veh);
    bool tryGetVehicle(Vehicle** veh);

    // testing
    Vehicle* getVehicle();
};