#pragma once

namespace diffengine {
    class Value {
    public:
        float data;
        float grad;

        // Constructor
        explicit Value(float val) : data(val), grad(0.0f) {}

        // Basic addition (forward pass only for now)
        Value operator+(const Value& other) const {
            return Value(this->data + other.data);
        }
    };
}