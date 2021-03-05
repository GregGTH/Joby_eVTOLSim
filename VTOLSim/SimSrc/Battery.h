#include <Windows.h>

#ifndef BATTERYH
#define BATTERYH

class Battery {
private:
    const int _capacity_kwh;
    const double _timeToCharge_hr;
    double _curCharge_kwh;
    double _curTimeToFullCharge_hr;

public:
    Battery(int capacity_kwh, double timeToCharge_hr);

    // Modify battery state
    double drain(double chargeUsed_kwh);
    double charge(double deltaTime_hr);

    bool isFullyCharged();
    bool needToCharge();

    // Testing funcs
    void reset(double curCharge_kwh, double timeToFull_hr);
    double getCurCharge();
    double getCurTimeToCharge();
};

#endif