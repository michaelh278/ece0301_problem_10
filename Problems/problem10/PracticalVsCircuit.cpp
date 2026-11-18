#include <stdexcept>
#include <vector>
#include <cmath>

#include "PracticalVsCircuit.hpp"

PracticalVsCircuit::PracticalVsCircuit(double vs, double rs, double rl) : VoltageDivider(vs, std::vector<double>{rs, rl}.data(), 2) {} // vector thingy is very hacky but it's ok

double PracticalVsCircuit::get_source_voltage() const {
    // Practical source voltage = voltage across RL
    // RL is at index 1
    return get_resistor_voltage(1);
}

void PracticalVsCircuit::set_load(double rl) {
    // Use VoltageDivider method
    set_resistance(1, rl);
}

double PracticalVsCircuit::get_load_power() const {
    // PL = VL^2 / RL
    return std::pow(get_source_voltage(), 2) / get_resistance(1);
}

bool PracticalVsCircuit::maximum_power_transfer(double rl) {
    // Set load to argument
    // Because it calls the voltage divider method,
    // this should automatically rebalance the circuit
    set_load(rl);
    
    // Check whether new load power = max power
    double load_power = get_load_power();
    // Pmax = VS^2 / 4RS
    // VS cannot be gotten through get_source_voltage
    // anymore, so I'll get it with node voltage instead
    double max_power = std::pow(get_node_voltage(1), 2) / (4 * get_resistance(0));
    
    // Check whether values are roughly equal
    return std::abs(load_power - max_power) < 1e-9;
}
