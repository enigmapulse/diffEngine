#!/usr/bin/env bash

set -e

echo "Creating issues for Autograd Engine..."

# --- EPIC 1: Project Setup ---

gh issue create --title "Document initial CMake and Test setup" --label "setup,good-first-issue" --body "
## Description
Set up CMake build system and integrate a testing framework (like Catch2 or GTest).

## Tasks
- [ ] Create base CMakeLists.txt
- [ ] Add tests/ folder and test executable target
- [ ] Verify dummy test passes

## Acceptance Criteria
'cmake --build build' compiles the library and runs tests successfully.
"

gh issue create --title "Create standard project folder layout" --label "setup" --body "
## Description
Organize source and include folders for a C++ library.

## Tasks
- [ ] include/autograd/
- [ ] src/
- [ ] examples/
- [ ] Update CMake to link include directories

## Acceptance Criteria
Headers can be included via '#include <autograd/filename.hpp>'.
"

# --- EPIC 2: Core Data Structures ---

gh issue create --title "Define Value class skeleton" --label "core" --body "
## Description
Create the foundational computational node.

## Tasks
- [ ] float data field
- [ ] float grad field (default 0.0)
- [ ] Constructor taking a float

## Acceptance Criteria
Can instantiate a Value object and read its data/grad.
"

gh issue create --title "Implement graph tracking in Value" --label "core" --body "
## Description
Enable nodes to remember their operands.

## Tasks
- [ ] Add a vector/set of shared pointers to children
- [ ] Add a string/enum for operation type (e.g., '', '+', '*')
- [ ] Update constructor to handle children

## Acceptance Criteria
A node can successfully store pointers to other nodes.
"

# --- EPIC 3: Forward Pass & Operator Overloading ---

gh issue create --title "Implement addition operator overloading" --label "forward-pass" --body "
## Description
Overload operator+ to create a new Value node.

## Tasks
- [ ] Implement operator+(Value a, Value b)
- [ ] Sum the data fields
- [ ] Add a and b as children of the new node
- [ ] Set operation type to '+'

## Acceptance Criteria
c = a + b compiles and c.data is mathematically correct.
"

gh issue create --title "Implement multiplication operator overloading" --label "forward-pass" --body "
## Description
Overload operator* to create a new Value node.

## Tasks
- [ ] Implement operator*(Value a, Value b)
- [ ] Multiply the data fields
- [ ] Add a and b as children
- [ ] Set operation type to '*'

## Acceptance Criteria
c = a * b compiles and c.data is mathematically correct.
"

gh issue create --title "Implement ReLU activation function" --label "forward-pass" --body "
## Description
Add a non-linear activation function.

## Tasks
- [ ] Implement relu() method
- [ ] If data < 0, new data = 0; else new data = data
- [ ] Store original node as child

## Acceptance Criteria
ReLU successfully clips negative values to 0.
"

# --- EPIC 4: The Backward Pass ---

gh issue create --title "Add backward closures to Value class" --label "backward-pass" --body "
## Description
Store the chain rule logic for each operation.

## Tasks
- [ ] Add a std::function<void()> _backward field
- [ ] Initialize to empty lambda
- [ ] Implement addition derivative (local grad = 1.0)
- [ ] Implement multiplication derivative (local grad = other child's data)

## Acceptance Criteria
Calling _backward() on a node correctly updates its children's gradients.
"

gh issue create --title "Implement Topological Sort" --label "backward-pass" --body "
## Description
Sort the graph so parents are processed before children.

## Tasks
- [ ] Write a DFS helper function
- [ ] Keep track of visited nodes
- [ ] Push nodes to a vector after visiting children

## Acceptance Criteria
Produces a correctly ordered list of nodes from output to inputs.
"

gh issue create --title "Implement the backward() entrypoint" --label "backward-pass" --body "
## Description
The main function that triggers automatic differentiation.

## Tasks
- [ ] Set output node grad to 1.0
- [ ] Call topological sort
- [ ] Iterate through sorted list in reverse and execute _backward() closures

## Acceptance Criteria
A complex equation like d = (a * b) + c calculates correct gradients for a, b, and c.
"

# --- EPIC 5: Systems Optimizations ---

gh issue create --title "Replace shared_ptr with custom Arena Allocator" --label "optimization,memory" --body "
## Description
Remove dynamic heap allocations for nodes to speed up graph building and fix cyclical memory leaks.

## Tasks
- [ ] Allocate a large raw memory block via malloc/vector
- [ ] Implement a bump allocator for new Values
- [ ] Update operators to use the arena instead of standard pointers

## Acceptance Criteria
Performance test shows significantly faster graph construction times. No memory leaks via Valgrind.
"

echo "All issues created successfully! Time to write some code."