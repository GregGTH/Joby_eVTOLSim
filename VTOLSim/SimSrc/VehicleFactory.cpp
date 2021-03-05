#include "VehicleFactory.h"
#include "AlphaVehicle.h"
#include "BravoVehicle.h"
#include "CharlieVehicle.h"
#include "DeltaVehicle.h"
#include "EchoVehicle.h"

Vehicle* VehicleFactory::createRandomVehicle()
{
    // Decide random vehicle type
    VehicleType type = (VehicleType)(rand() % NUM_VEH_TYPES);

    switch(type)
    {
    case ALPHA_VEH:
        return new AlphaVehicle();
        break;

    case BRAVO_VEH:
        return new BravoVehicle();
        break;

    case CHARLIE_VEH:
        return new CharlieVehicle();
        break;

    case DELTA_VEH:
        return new DeltaVehicle();
        break;

    case ECHO_VEH:
        return new EchoVehicle();
        break;

    default:
        return nullptr;
        break;
    }
}