#include "FeeCalculator.h"
#include <cmath>
using namespace std;

// Hourly rates in rupees
const double RATE_BIKE = 2.0;
const double RATE_CAR  = 5.0;
const double RATE_EV   = 3.0;

// ---------------------------------------------------------------------------
// calculateFee
// Converts durationMinutes to hours, rounds UP to the nearest whole hour,
// then multiplies by the vehicle's hourly rate.
//
// ceil() from <cmath> handles the rounding:
//   61 min -> ceil(1.016) = 2 hours billed
//   60 min -> ceil(1.0)   = 1 hour  billed
//   10 min -> ceil(0.166) = 1 hour  billed  (minimum charge)
// ---------------------------------------------------------------------------
double FeeCalculator::calculateFee(const string& vehicleType, int durationMinutes)
{
    // Convert minutes to fractional hours, then round up
    double hours = ceil(durationMinutes / 60.0);

    // Minimum charge of 1 hour even for very short stays
    if (hours < 1.0)
        hours = 1.0;

    double rate = 0.0;

    if (vehicleType == "Bike")
        rate = RATE_BIKE;
    else if (vehicleType == "Car")
        rate = RATE_CAR;
    else if (vehicleType == "EV")
        rate = RATE_EV;

    return hours * rate;
}