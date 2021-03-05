#include "Vehicle.h"
#include <Windows.h>
#include <vector>
#include <synchapi.h>

#ifndef VEHICLECONTROLLERH
#define VEHICLECONTROLLERH

class VehicleController {
public:
    // Parameters for thread entry func
    typedef struct threadParams {
        DWORD simTime_ms;                   // total real-time to sim
        DWORD simTimeFactor;                // sim-time factor
        VehicleController& controller;      // controller instance to update
        std::vector<Vehicle*>& vehicleList; // vehicles list

        threadParams(VehicleController& ctrl, std::vector<Vehicle*>& veh, DWORD time_ms, DWORD factor) :
            controller(ctrl), vehicleList(veh), simTime_ms(time_ms), simTimeFactor(factor) {}
    } vcParams;

private:
    // Transitions between uptime/downtime controllers
    static std::vector<Vehicle*> _transitionToChargeList;
    static std::vector<Vehicle*> _transitionToFlightList;
    static CRITICAL_SECTION _chargeQueueLock;
    static CRITICAL_SECTION _flightLock;
    static int _critSectionCounter;

    // real-time remaining in sim
    DWORD _timeRemaining_ms;
    DWORD _simTimeFactor;

    // Generic update (will call instance update)
    void update(DWORD realTimeElapsed_ms, DWORD simTimeElapsed_ms);
    bool isRunning();

protected:
    // Sync so uptime/downtime updates are 1:1
    static CRITICAL_SECTION _syncUpSection;
    static CRITICAL_SECTION _syncDownSection;

    // Generic setup
    virtual bool setup(vcParams& params);

    // Instance update
    virtual void updateController(DWORD timeElapsed_ms) = 0;

    // Allow controllers to sync updates 1:1
    virtual void takeSyncSection() = 0;
    virtual void releaseSyncSection() = 0;
    
    // Transition list accessors
    void addVehUpToDown(const std::vector<Vehicle*>& vehToAdd);
    void addVehDownToUp(const std::vector<Vehicle*>& vehToAdd);
    void getVehUpToDown(std::vector<Vehicle*>& vehList);
    void getVehDownToUp(std::vector<Vehicle*>& vehList);

public:
    // thread entry point
    static DWORD WINAPI start(LPVOID param);
    
    VehicleController();
    virtual ~VehicleController();
};

#endif