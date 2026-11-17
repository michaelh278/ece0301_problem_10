#include "CurrentSource.hpp"

double CurrentSource::get_current() const
{
  // Return component value (current) directly
  // instead of calculating current
  return get_value();
}