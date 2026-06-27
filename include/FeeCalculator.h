#ifndef FEECALCULATOR_H
#define FEECALCULATOR_H

#include <string>
using namespace std;

// ---------------------------------------------------------------------------
// FeeCalculator
// Single responsibility: calculate the parking fee for a completed session.
// Called by ParkingManager on vehicle exit.
//
// Rates (per hour, minimum 1 hour charged):
//   Bike ->  Rs. 2 / hr
//   Car  ->  Rs. 5 / hr
//   EV   ->  Rs. 3 / hr
// ---------------------------------------------------------------------------
class FeeCalculator
{
public:

    // -----------------------------------------------------------------------
    // calculateFee
    // Returns the total fee in rupees.
    // durationMinutes : how long the vehicle was parked (entry to exit)
    // vehicleType     : "Car", "Bike", or "EV"
    // Minimum charge  : 1 full hour (so 10 min costs the same as 60 min)
    // -----------------------------------------------------------------------
    double calculateFee(const string& vehicleType, int durationMinutes);
};

#endif // FEECALCULATOR_H