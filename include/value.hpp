#pragma once

namespace diffengine {

    class Value;
    using ValueRef = std::shared_ptr<Value>;

    class Value {
    public:
        float data;
        float grad;
        std::vector<ValueRef> prev;
        std::string op;

        // Constructor
        explicit Value(float val, std::vector<ValueRef> children = {}, std::string op_type = "") 
            : data(val), grad(0.0f), prev(children), op(op_type) {}
    };

    inline ValueRef make_value(float val) {
        return std::make_shared<Value>(val);
    }

    inline ValueRef operator+(const ValueRef& a, const ValueRef& b) {
        return std::make_shared<Value>(
            a->data + b->data,
            std::vector<ValueRef>{a, b},
            "+"
        );
    }

    inline ValueRef operator*(const ValueRef& a, const ValueRef& b) {
        return std::make_shared<Value>(
            a->data * b->data,
            std::vector<ValueRef>{a, b},
            "*"
        );
    }
}