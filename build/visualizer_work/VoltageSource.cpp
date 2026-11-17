#include "VoltageSource.hpp"

double VoltageSource::get_voltage() const
{
  // Return component value (voltage) directly
  // instead of calculating voltage difference
  return get_value();
}