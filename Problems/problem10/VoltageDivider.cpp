#include <stdexcept>

#include "VoltageDivider.hpp"

VoltageDivider::VoltageDivider(double v, double res[], int num_res) : Network(num_res + 1, num_res + 1) {
    // Reject circuits with < 1 resistor
    if(num_res <= 0) {
        throw std::invalid_argument("Must have at least one resistor");
    }
    
    // Voltage source
    VoltageSource *vs = new VoltageSource();
    set_component(0, vs);
    set_component_value(0, v);
    // Connect to GND (node 0) and node 1
    connect_component_terminal_a(0, 0);
    connect_component_terminal_b(0, 1);
    
    // Resistors
    for(int i = 0; i < num_res; i++) {
        Resistor *ri = new Resistor();
        set_component(i + 1, ri);
        set_component_value(i + 1, res[i]);
        // Connect to consecutive nodes (backward)
        connect_component_terminal_b(i + 1, i + 1);
        // If this is the last resistor, connect back to GND
        connect_component_terminal_a(i + 1, (i == num_res - 1)? 0 : i + 2);
    }
    
    balance_voltages();
}

void VoltageDivider::balance_voltages() {
    // do a ton of math
    
    // Node 0: GND
    set_node_voltage(0, 0.0);
    
    // Node 1: connected to voltage source
    double source_voltage = get_component_value(0);
    set_node_voltage(1, source_voltage);
    
    // Other nodes: voltage divider rule
    
    // Sum of all resistances for denominator
    double total_resistance = 0.0;
    for(int i = 0; i < get_num_nodes() - 1; i++) {
        total_resistance += get_component_value(i + 1);
    }
    
    // Voltage of each node depends on the
    // voltage of the node before it
    for(int i = 1; i < get_num_nodes() - 1; i++) {
        double resistance_i = get_resistance(i - 1);
        double prev_node_voltage = get_node_voltage(i);
        double voltage_drop = get_source_voltage() * resistance_i / total_resistance;
        double new_node_voltage = prev_node_voltage - voltage_drop;
        set_node_voltage(i + 1, new_node_voltage);
    }
}

int VoltageDivider::get_num_resistors() const {
    // One component is a voltage source,
    // the rest are resistors
    return get_num_components() - 1;
}

double VoltageDivider::get_source_voltage() const {
    return get_component_value(0);
}

double VoltageDivider::get_resistance(int idx) const {
    // Reject invalid indices
    if(idx < 0 || idx >= get_num_components() - 1) {
        throw std::out_of_range("Resistor index out of range");
    }
    
    return get_component_value(idx + 1);
}

double VoltageDivider::get_resistor_voltage(int idx) const {
    // Reject invalid indices
    if(idx < 0 || idx >= get_num_components() - 1) {
        throw std::out_of_range("Resistor index out of range");
    }
    
    return get_component_voltage(idx + 1);
}

double VoltageDivider::get_loop_current() const {
    // Current can be automatically calculated from
    // node voltages and resistance from any resistor
    // Voltage divider will always have at least 1
    // resistor, and the first resistor is at index 1
    return get_component_current(1);
}

void VoltageDivider::set_source_voltage(double v) {
    set_component_value(0, v);
    
    // Need to rebalance circuit after changing values
    balance_voltages();
}

void VoltageDivider::set_resistance(int idx, double r) {
    // Reject invalid indices
    if(idx < 0 || idx >= get_num_components() - 1) {
        throw std::out_of_range("Resistor index out of range");
    }
    
    set_component_value(idx + 1, r);
    
    // Need to rebalance circuit after changing values
    balance_voltages();
}