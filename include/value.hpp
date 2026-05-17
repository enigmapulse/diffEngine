#pragma once
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <set>

namespace diffengine {

    class Value;
    using ValueRef = std::shared_ptr<Value>;

    class Value {
    public:
        float data;
        float grad;
        std::vector<ValueRef> prev;
        std::string op;

        std::function<void()> _backward = [](){};

        // Constructor
        explicit Value(float val, std::vector<ValueRef> children = {}, std::string op_type = "") 
            : data(val), grad(0.0f), prev(children), op(op_type) {}
    };

    inline ValueRef make_value(float val) {
        return std::make_shared<Value>(val);
    }

    inline ValueRef operator+(const ValueRef& a, const ValueRef& b) {
        auto out = std::make_shared<Value>(
            a->data + b->data,
            std::vector<ValueRef>{a, b},
            "+"
        );

        Value* out_ptr = out.get();

        out->_backward = [a, b, out_ptr] () {
            a -> grad += 1.0f * out_ptr -> grad;
            b -> grad += 1.0f * out_ptr -> grad;
        };

        return out;
    }

    inline ValueRef operator*(const ValueRef& a, const ValueRef& b) {
        auto out =  std::make_shared<Value>(
            a->data * b->data,
            std::vector<ValueRef>{a, b},
            "*"
        );

        Value* out_ptr = out.get();

        out->_backward = [a, b, out_ptr] () {
            a -> grad += b -> data * out_ptr -> grad;
            b -> grad += a -> data * out_ptr -> grad;
        };

        return out;
    }

    inline ValueRef relu(const ValueRef& a) {
        auto out = std::make_shared<Value>(
            std::max(a->data, 0.0f),
            std::vector<ValueRef>{a},
            "ReLU"
        );

        Value* out_ptr = out.get();

        out->_backward = [a, out_ptr] () {
            a -> grad += (out_ptr -> data > 0.0f ? 1.0f : 0.0f) * out_ptr -> grad;
        };

        return out;
    }

    inline std::vector<ValueRef> topo_sort(const ValueRef& root) {
        std::vector<ValueRef> topo;
        std::set<Value*> visited;

        std::function<void(const ValueRef&)> build_topo = [&](const ValueRef& v) {
            if(visited.find(v.get()) == visited.end()) {
                visited.insert(v.get());

                for(const auto& child : v->prev) {
                    build_topo(child);
                }
                topo.push_back(v);
            }
        };

        build_topo(root);
        return topo;
    }

    inline void backward(const ValueRef& root) {
        auto topo = topo_sort(root);

        root->grad = 1.0f;

        for(auto it = topo.rbegin(); it != topo.rend(); ++it) {
            (*it)->_backward();
        }
    }
}