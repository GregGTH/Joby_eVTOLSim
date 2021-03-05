#include "Vehicle.h"

#ifndef VEHICLEFACTORYH
#define VEHICLEFACTORYH

class VehicleFactory {
    enum VehicleType
    {
        ALPHA_VEH = 0,
        BRAVO_VEH,
        CHARLIE_VEH,
        DELTA_VEH,
        ECHO_VEH,
        NUM_VEH_TYPES
    };

public:
    Vehicle* createRandomVehicle();
};

#endif