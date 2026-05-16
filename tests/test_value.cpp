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