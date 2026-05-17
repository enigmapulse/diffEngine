# diffEngine 

**A high-performance, header-only reverse-mode automatic differentiation engine built in C++17.**

`diffEngine` is a lightweight, scalar autograd engine (similar to PyTorch's core autograd or Karpathy's `micrograd`) designed for speed and memory efficiency. It dynamically builds a computational Directed Acyclic Graph (DAG) during the forward pass and executes gradients via a topological sort in the backward pass.

## Key Features

* **Custom Arena Allocator:** Bypasses `std::shared_ptr` and standard OS heap allocations (`malloc`/`new`). Nodes are allocated in $O(1)$ time via a static bump allocator, completely eliminating cyclical memory leaks and drastically speeding up graph construction
* **Topological Sort:** Uses a zero-overhead Depth-First Search (DFS) to flatten the computational graph and accurately accumulate gradients
* **Header-Only:** Drop `Value.hpp` into any C++ project and compile instantly. No linking required.
* **Modern C++:** Utilizes functional operator overloading (`+`, `*`), lambda closures for chain-rule logic, and the handle pattern for safe raw pointer manipulation.

## Quick Start

```cpp
#include <iostream>
#include "Value.hpp"

using namespace diffengine;

int main() {
    // 1. Initialize variables
    auto a = make_value(2.0f);
    auto b = make_value(-3.0f);
    auto c = make_value(10.0f);

    // 2. Build the computational graph (Forward Pass)
    auto e = a * b;
    auto d = e + c;
    auto f = make_value(-2.0f);
    auto L = d * f;

    // 3. Compute gradients (Backward Pass)
    backward(L);

    // 4. Print results
    std::cout << "Loss: " << L->data << std::endl; // Loss: -8
    std::cout << "Gradient of a: " << a->grad << std::endl; // dL/da: 6
    
    // 5. Clean up memory for the next training loop
    Value::reset_arena();

    return 0;
}
