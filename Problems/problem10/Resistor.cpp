#include <stdexcept>

#include "Resistor.hpp"

void Resistor::set_value(double r)
{
  // Reject negative resistance
  if(r < 0) {
    throw std::invalid_argument("Resistance cannot be negative");
  }
  
  // Use base class version of set_value
  Component::set_value(r);
}

double Resistor::get_current() const
{
  // Error if resistance = 0
  if(get_value() == 0.0) {
    throw std::logic_error("Cannot divide by zero resistance");
  }
  
  // Ohm's law: I = V/R
  return get_voltage() / get_value();
}