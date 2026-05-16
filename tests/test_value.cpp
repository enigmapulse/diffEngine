// tests/test_value.cpp
#include <gtest/gtest.h>
#include "value.hpp"

TEST(ValueTest, Initialization) {
    diffengine::Value v(5.5f);
    EXPECT_DOUBLE_EQ(v.data, 5.5f);
    EXPECT_DOUBLE_EQ(v.grad, 0.0f);
}

TEST(ValueTest, Addition) {
    diffengine::Value a(2.0f);
    diffengine::Value b(3.5f);
    diffengine::Value c = a + b;
    
    EXPECT_DOUBLE_EQ(c.data, 5.5f);
}