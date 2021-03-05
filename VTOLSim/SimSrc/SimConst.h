#ifndef SIMCONSTH
#define SIMCONSTH

#define NUM_SIM_VEHICLES    20
#define TOTAL_SIM_TIME_MS   (3*60*1000) // total real-time to sim (3 min)
#define SIM_TIME_FACTOR     60          // 1 min (real-time) = 1 hr (sim-time)
#define MS_TO_HR(x)         ((x) / 1000.0 / 60.0 / 60.0)
#define HR_TO_MS(x)         ((x) * 1000.0 * 60.0 * 60.0)

#endif