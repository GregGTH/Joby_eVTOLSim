#include "stdafx.h"
#include "gtest/gtest.h"
#include "Charger.h"
#include "Vehicle.h"
#include "AlphaVehicle.h"
#include "BravoVehicle.h"
#include "SimConst.h"
#include <list>

using namespace std;

class ChargerTestFixture : public ::testing::Test {
public:
    Charger* chgr;
    list<Vehicle*> vehList;

    ChargerTestFixture() { }

    void SetUp() {
        chgr = new Charger();
        vehList.push_back(new AlphaVehicle());
        vehList.push_back(new BravoVehicle());
    }

    void TearDown() { 
        delete chgr;

        for (Vehicle* veh : vehList)
            delete veh;
    }

   ~ChargerTestFixture() { }
};

/**
  * Testing the Charger class.
  */
TEST_F(ChargerTestFixture, ChargerTest)
{
    Vehicle* vehPtr;
    Vehicle* vehA = vehList.front();
    Vehicle* vehB = vehList.back();

    // Set batteries to empty
    vehA->getBattery()->reset(0.0, 1.0);
    vehB->getBattery()->reset(0.0, 2.0);

    ASSERT_FALSE(chgr->tryGetVehicle(&vehPtr));    // empty at start

    ASSERT_TRUE(chgr->tryPutVehicle(vehA));   // can put 1 vehicle
    ASSERT_FALSE(chgr->tryPutVehicle(vehB));

    // Charge some time, can't change vehicle until fully charged
    chgr->update((DWORD)HR_TO_MS(0.9));     // vehA battery set to 1.0hr to charge
    ASSERT_FALSE(chgr->tryGetVehicle(&vehPtr));
    ASSERT_FALSE(chgr->tryPutVehicle(vehB));

    // charge rest of time, can replace vehicle
    chgr->update((DWORD)HR_TO_MS(0.1));
    ASSERT_TRUE(chgr->tryGetVehicle(&vehPtr));
    ASSERT_TRUE(vehPtr->doneCharging());
    ASSERT_TRUE(chgr->tryPutVehicle(vehB));

    // Type check - ensure correct vehicle is retrieved/charging
    ASSERT_TRUE(dynamic_cast<AlphaVehicle*>(vehPtr));
    ASSERT_TRUE(dynamic_cast<BravoVehicle*>(chgr->getVehicle()));
}