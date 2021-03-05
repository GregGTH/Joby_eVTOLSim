#include "VehicleController.h"
#include <Windows.h>

using namespace std;

// Class variables
int VehicleController::_critSectionCounter = 0;
CRITICAL_SECTION VehicleController::_chargeQueueLock;
CRITICAL_SECTION VehicleController::_flightLock;
vector<Vehicle*> VehicleController::_transitionToChargeList;
vector<Vehicle*> VehicleController::_transitionToFlightList;
CRITICAL_SECTION VehicleController::_syncUpSection;
CRITICAL_SECTION VehicleController::_syncDownSection;

VehicleController::VehicleController() : _timeRemaining_ms(0), _simTimeFactor(1)
{
    // Init critical sections once
    if (_critSectionCounter == 0)
    {
        InitializeCriticalSection(&_chargeQueueLock);
        InitializeCriticalSection(&_flightLock);
        InitializeCriticalSection(&_syncUpSection);
        InitializeCriticalSection(&_syncDownSection);
    }
    
    ++_critSectionCounter;
}

VehicleController::~VehicleController()
{
    --_critSectionCounter;

    // Release critical sections once
    if (_critSectionCounter == 0)
    {
        DeleteCriticalSection(&_chargeQueueLock);
        DeleteCriticalSection(&_flightLock);
        DeleteCriticalSection(&_syncUpSection);
        DeleteCriticalSection(&_syncDownSection);
    }
}

/**
 *  Thread entry point to start controllers. The vehicle controllers
 *  will update all relevant vehicles, either in-air (uptime) or
 *  charging (downtime).
 */
DWORD WINAPI VehicleController::start(LPVOID param)
{
    vcParams* paramData = (vcParams*)param;
    VehicleController& vcInstance = paramData->controller;
    DWORD prevTime_ms = 0;

    // Setup instance
    if (!paramData || !vcInstance.setup(*paramData))
        return 1;

    // Simulate until time is up
    while (vcInstance.isRunning())
    {
        vcInstance.takeSyncSection();

        DWORD currentTime_ms = GetTickCount();
        DWORD deltaTime_ms = 0;

        // Init previous time
        if (prevTime_ms == 0)
        {
            prevTime_ms = currentTime_ms;
        }
        // Otherwise, update
        else
        {
            deltaTime_ms = currentTime_ms - prevTime_ms;
            prevTime_ms = currentTime_ms;

            // Apply time factor, take min so we don't go over given total time
            DWORD simTime_ms = min((deltaTime_ms * paramData->simTimeFactor), 
                  (vcInstance._timeRemaining_ms * paramData->simTimeFactor));
            
#if 1
            vcInstance.update(deltaTime_ms, simTime_ms);
#elif
            vcInstance.update(10000, 600000);
#endif
        }

        vcInstance.releaseSyncSection();

        // Delay
        Sleep(25);
    }

    return 0;
}

void VehicleController::update(DWORD realTimeElapsed_ms, DWORD simTimeElapsed_ms)
{
    // Update controller instance
    updateController(simTimeElapsed_ms);

    // Update remaining time, take min to prevent rollover
    _timeRemaining_ms -= min(realTimeElapsed_ms, _timeRemaining_ms);
}

bool VehicleController::setup(vcParams& params)
{
    _timeRemaining_ms = params.simTime_ms;

    return true;
}

bool VehicleController::isRunning()
{
    return (_timeRemaining_ms > 0);
}

/**
 *  Transition vehicles in given list from uptime controller to downtime controller.
 */
void VehicleController::addVehUpToDown(const vector<Vehicle*>& vehToAdd)
{
    EnterCriticalSection(&_chargeQueueLock);
    _transitionToChargeList.insert(_transitionToChargeList.end(), vehToAdd.begin(), vehToAdd.end());
    LeaveCriticalSection(&_chargeQueueLock);
}

/**
 *  Transition vehicles in given list from downtime controller to uptime controller.
 */
void VehicleController::addVehDownToUp(const vector<Vehicle*>& vehToAdd)
{
    EnterCriticalSection(&_flightLock);
    _transitionToFlightList.insert(_transitionToFlightList.end(), vehToAdd.begin(), vehToAdd.end());
    LeaveCriticalSection(&_flightLock);
}

/**
 *  Get all vehicles from the uptime-to-downtime tranisition list.
 *  vehList will be updated and the transition list will be cleared
 */
void VehicleController::getVehUpToDown(vector<Vehicle*>& vehList)
{
    EnterCriticalSection(&_chargeQueueLock);
    
    vehList = move(_transitionToChargeList);
    _transitionToChargeList.clear();

    LeaveCriticalSection(&_chargeQueueLock);
}

/**
 *  Get all vehicles from the downtime-to-uptime tranisition list
 *  vehList will be updated and the transition list will be cleared
 */
void VehicleController::getVehDownToUp(vector<Vehicle*>& vehList)
{
    EnterCriticalSection(&_flightLock);
    
    vehList = move(_transitionToFlightList);
    _transitionToFlightList.clear();

    LeaveCriticalSection(&_flightLock);
}