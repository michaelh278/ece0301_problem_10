#include <stdexcept>

#include "CurrentDivider.hpp"

CurrentDivider::CurrentDivider(double c, double res[], int num_res) : Network(2, num_res + 1) {
    // Reject circuits with < 1 resistor
    if(num_res <= 0) {
        throw std::invalid_argument("Must have at least one resistor");
    }
    
    // Current source
    CurrentSource *cs = new CurrentSource();
    set_component(0, cs);
    set_component_value(0, c);
    // Connect to GND (node 0) and node 1
    connect_component_terminal_a(0, 0);
    connect_component_terminal_b(0, 1);
    
    // Resistors
    for(int i = 0; i < num_res; i++) {
        Resistor *ri = new Resistor();
        set_component(i + 1, ri);
        set_component_value(i + 1, res[i]);
        // Connect in parallel with current source
        connect_component_terminal_a(i + 1, 0);
        connect_component_terminal_b(i + 1, 1);
    }
    
    balance_voltages();
}

void CurrentDivider::balance_voltages() {
    // Node 0: GND
    set_node_voltage(0, 0.0);
    
    // Use current divider equation to calculate
    // current through the first resistor, then
    // multiply by its resistance to get voltage
    
    // Sum inverse resistances, then invert that
    // to get parallel combination of resistors
    double inv_parallel = 0.0;
    for(int i = 1; i < get_num_resistors(); i++) {
        inv_parallel += 1.0 / get_resistance(i);
    }
    double parallel_res = 1.0 / inv_parallel;
    // Denominator of current divider is parallel
    // combination plus the resistance in question
    double denom = get_resistance(0) + parallel_res;
    // Actual current divider equation
    double first_res_current = get_source_current() * parallel_res / denom;
    // Ohm's law: V = IR
    double top_voltage = first_res_current * get_resistance(0);
    set_node_voltage(1, top_voltage);
}

int CurrentDivider::get_num_resistors() const {
    // Same definition as voltage divider
    return get_num_components() - 1;
}

double CurrentDivider::get_source_current() const {
    return get_component_value(0);
}

double CurrentDivider::get_resistance(int idx) const {
    // Reject invalid indices
    if(idx < 0 || idx >= get_num_components() - 1) {
        throw std::out_of_range("Resistor index out of range");
    }
    
    return get_component_value(idx + 1);
}

double CurrentDivider::get_resistor_voltage() const {
    // Voltage across any resistor = voltage of top node
    return get_node_voltage(1);
}

double CurrentDivider::get_resistor_current(int idx) const {
    // Reject invalid indices
    if(idx < 0 || idx >= get_num_components() - 1) {
        throw std::out_of_range("Resistor index out of range");
    }
    
    // Can calculate using existing method
    return get_component_current(idx + 1);
}

void CurrentDivider::set_source_current(double c) {
    set_component_value(0, c);
    
    // Need to rebalance circuit after changing values
    balance_voltages();
}

void CurrentDivider::set_resistance(int idx, double r) {
    // Reject invalid indices
    if(idx < 0 || idx >= get_num_components() - 1) {
        throw std::out_of_range("Resistor index out of range");
    }
    
    set_component_value(idx + 1, r);
    
    // Need to rebalance circuit after changing values
    balance_voltages();
}
