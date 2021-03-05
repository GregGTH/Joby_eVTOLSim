#include "Battery.h"
#include <math.h>

Battery::Battery(int capacity_kwh, double timeToCharge_hr) 
    : _capacity_kwh(capacity_kwh), _timeToCharge_hr(timeToCharge_hr)
{
    // Start fully charged
    _curCharge_kwh = _capacity_kwh;
    _curTimeToFullCharge_hr = 0.0;
}

/**
 *  Drain this battery given the amount of charge used.
 *
 *  @return The amount of charge (kwh) leftover if battery is drained completely.
 */
double Battery::drain(double chargeUsed_kwh)
{
    double chargeLeft_kwh = 0.0;

    _curCharge_kwh -= chargeUsed_kwh;

    // If more charge than available, return leftover
    if (_curCharge_kwh < 0.0)
    {
        chargeLeft_kwh = abs(_curCharge_kwh);
        _curCharge_kwh = 0.0;
    }
    
    // Reset time-to-full-charge if battery is empty
    if (needToCharge())
        _curTimeToFullCharge_hr = _timeToCharge_hr;

    return chargeLeft_kwh;
}

/**
 *  Charge this battery given a delta time.
 *
 *  @return The amount of time (hr) leftover if battery is fully charged.
 */
double Battery::charge(double deltaTime_hr)
{
    double timeLeft_hr = 0.0;

    _curTimeToFullCharge_hr -= deltaTime_hr;

    // If more time than available, return leftover
    if (_curTimeToFullCharge_hr < 0.0)
    {
        timeLeft_hr = abs(_curTimeToFullCharge_hr);
        _curTimeToFullCharge_hr = 0.0;
    }
    
    // Reset current charge if battery is full
    if (isFullyCharged())
        _curCharge_kwh = _capacity_kwh;

    return timeLeft_hr;
}

/**
 *  Check battery charge is full. 
 *  True when time-to-charge is 0.
 */
bool Battery::isFullyCharged()
{
    return (_curTimeToFullCharge_hr <= 0.0);
}

/**
 *  Check battery needs to charge 
 *  True when charge is drained.
 */
bool Battery::needToCharge()
{
    return (_curCharge_kwh <= 0.0);
}

/**
 *  Reset this battery to the given values
 */
void Battery::reset(double curCharge_kwh, double timeToFull_hr)
{
    _curCharge_kwh = curCharge_kwh;
    _curTimeToFullCharge_hr = timeToFull_hr;
}

double Battery::getCurCharge()
{
    return _curCharge_kwh;
}

double Battery::getCurTimeToCharge()
{
    return _curTimeToFullCharge_hr;
}