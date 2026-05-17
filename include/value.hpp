#pragma once
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <set>

namespace diffengine {

    class Value;
    struct ValueRef {
        Value* ptr;
        ValueRef(Value* p = nullptr) : ptr(p) {}
        
        Value* operator->() const { return ptr; }
        Value& operator*() const { return *ptr; }
        
        bool operator==(const ValueRef& other) const { return ptr == other.ptr; }
    };

    class Value {
    public:
        float data;
        float grad;
        std::vector<ValueRef> prev;
        std::string op;

        std::function<void()> _backward = [](){};

        /* -- Arena allocator -- */
        static constexpr size_t ARENA_CAPACITY = 1000000;
        static uint8_t arena_memory[];
        static size_t arena_offset;

        void* operator new(size_t size) {
            if (arena_offset * sizeof(Value) + size > ARENA_CAPACITY * sizeof(Value)) {
                throw std::bad_alloc(); // out of memory
            }
            void* ptr = &arena_memory[arena_offset * sizeof(Value)];
            arena_offset++;
            return ptr;
        }

        void operator delete(void* p) {}

        static void reset_arena() {
            arena_offset = 0;
        }

        explicit Value(float val, std::vector<ValueRef> children = {}, std::string op_type = "") 
            : data(val), grad(0.0f), prev(children), op(op_type) {}
    };

    inline uint8_t Value::arena_memory[ARENA_CAPACITY * sizeof(Value)];
    inline size_t Value::arena_offset = 0;

    inline ValueRef make_value(float val) {
        return new Value(val);
    }

    inline ValueRef operator+(const ValueRef& a, const ValueRef& b) {
        auto out = new Value(a->data + b->data, std::vector<ValueRef>{a, b}, "+");
        out->_backward = [a, b, out] () {
            a -> grad += 1.0f * out -> grad;
            b -> grad += 1.0f * out -> grad;
        };
        return out;
    }

    inline ValueRef operator*(const ValueRef& a, const ValueRef& b) {
        auto out = new Value(a->data * b->data, std::vector<ValueRef>{a, b}, "*");
        out->_backward = [a, b, out] () {
            a->grad += b->data * out->grad;
            b->grad += a->data * out->grad;
        };
        return out;
    }

    inline ValueRef relu(const ValueRef& a) {
        auto out = new Value(std::max(a->data, 0.0f), std::vector<ValueRef>{a}, "ReLU");
        out->_backward = [a, out] () {
            a->grad += (out->data > 0.0f ? 1.0f : 0.0f) * out->grad;
        };
        return out;
    }

    inline std::vector<ValueRef> topo_sort(const ValueRef& root) {
        std::vector<ValueRef> topo;
        std::set<Value*> visited;

        std::function<void(const ValueRef&)> build_topo = [&](const ValueRef& v) {
            if(visited.find(v.ptr)== visited.end()) {
                visited.insert(v.ptr);

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

    inline std::ostream& operator<<(std::ostream& os, const ValueRef& v) {
        os << "Value(data=" << v->data << ", grad=" << v->grad;
        if (!v->op.empty()) os << ", op='" << v->op << "'";
        os << ")";
        return os;
    }
}