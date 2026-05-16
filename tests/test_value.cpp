// tests/test_value.cpp
#include <gtest/gtest.h>
#include "value.hpp"

TEST(ValueTest, Initialization) {
    diffengine::Value v(5.5);
    EXPECT_DOUBLE_EQ(v.data, 5.5);
    EXPECT_DOUBLE_EQ(v.grad, 0.0);
}

TEST(ValueTest, Addition) {
    diffengine::Value a(2.0);
    diffengine::Value b(3.5);
    diffengine::Value c = a + b;
    
    EXPECT_DOUBLE_EQ(c.data, 5.5);
}