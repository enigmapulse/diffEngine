// tests/test_value.cpp
#include <gtest/gtest.h>
#include "value.hpp"

TEST(ValueTest, Initialization) {
    auto v = diffengine::make_value(5.5f);
    
    EXPECT_FLOAT_EQ(v->data, 5.5f);
    EXPECT_FLOAT_EQ(v->grad, 0.0f);
    EXPECT_TRUE(v->prev.empty()); // Leaf nodes have no children
    EXPECT_EQ(v->op, "");         // Leaf nodes have no operation
}

TEST(ValueTest, GraphTrackingAddition) {
    auto a = diffengine::make_value(2.0f);
    auto b = diffengine::make_value(3.5f);
    
    auto c = a + b;
    
    EXPECT_FLOAT_EQ(c->data, 5.5f);
    EXPECT_EQ(c->op, "+");
    
    ASSERT_EQ(c->prev.size(), 2);
    EXPECT_EQ(c->prev[0], a); 
    EXPECT_EQ(c->prev[1], b); 
}


TEST(ValueTest, GraphTrackingMultiplication) {
    auto a = diffengine::make_value(2.0f);
    auto b = diffengine::make_value(3.0f);
    
    auto c = a * b;
    
    EXPECT_FLOAT_EQ(c->data, 6.0f);
    EXPECT_EQ(c->op, "*");
    
    ASSERT_EQ(c->prev.size(), 2);
    EXPECT_EQ(c->prev[0], a);
    EXPECT_EQ(c->prev[1], b);
}

TEST(ValueTest, ReLUPositive) {
    auto a = diffengine::make_value(5.0f);
    auto out = diffengine::relu(a); 
    
    EXPECT_FLOAT_EQ(out->data, 5.0f);
    EXPECT_EQ(out->op, "ReLU");
    ASSERT_EQ(out->prev.size(), 1);
    EXPECT_EQ(out->prev[0], a);
}

TEST(ValueTest, ReLUNegative) {
    auto a = diffengine::make_value(-3.5f);
    auto out = diffengine::relu(a);
    
    EXPECT_FLOAT_EQ(out->data, 0.0f);
    EXPECT_EQ(out->op, "ReLU");
    ASSERT_EQ(out->prev.size(), 1);
    EXPECT_EQ(out->prev[0], a);
}

TEST(ValueTest, BackwardClosures) {
    auto a = diffengine::make_value(2.0f);
    auto b = diffengine::make_value(3.0f);
    
    // c = a * b 
    auto c = a * b;
    
    // d = c + a 
    auto d = c + a;
    
    d->grad = 1.0f; 
    
    d->_backward(); // d = c + a -> c.grad += 1.0, a.grad += 1.0
    c->_backward(); // c = a * b -> a.grad += b.data * c.grad, b.grad += a.data * c.grad
    
    /* Expected Gradients:
     * dd/dd = 1.0
     * dd/dc = 1.0
     * dd/da = (dd/da from addition) + (dd/dc * dc/da) = 1.0 + (1.0 * 3.0) = 4.0
     * dd/db = dd/dc * dc/db = 1.0 * 2.0 = 2.0
     */
    
    EXPECT_FLOAT_EQ(c->grad, 1.0f);
    EXPECT_FLOAT_EQ(a->grad, 4.0f);
    EXPECT_FLOAT_EQ(b->grad, 2.0f);
}

TEST(ValueTest, TopologicalSortDiamondProblem) {
    auto a = diffengine::make_value(2.0f);
    auto b = diffengine::make_value(3.0f);
    
    // c depends on a and b
    auto c = a + b; 
    
    // d depends on c and a 
    auto d = c * a; 
    
    auto topo = diffengine::topo_sort(d);
    
    EXPECT_EQ(topo.back(), d);
    
    auto it_a = std::find(topo.begin(), topo.end(), a);
    auto it_b = std::find(topo.begin(), topo.end(), b);
    auto it_c = std::find(topo.begin(), topo.end(), c);
    
    EXPECT_TRUE(it_a < it_c);
    EXPECT_TRUE(it_b < it_c); 
    EXPECT_TRUE(it_c < topo.end() - 1); 
    EXPECT_TRUE(it_a < topo.end() - 1); 
}