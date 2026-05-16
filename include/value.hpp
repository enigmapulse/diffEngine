#pragma once

namespace diffengine {
    class Value {
    public:
        double data;
        double grad;

        // Constructor
        explicit Value(double val) : data(val), grad(0.0) {}

        // Basic addition (forward pass only for now)
        Value operator+(const Value& other) const {
            return Value(this->data + other.data);
        }
    };
}