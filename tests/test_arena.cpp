#include <gtest/gtest.h>
#include "value.hpp"

TEST(ArenaTest, BumpAllocatorFunctionality) {
    diffengine::Value::reset_arena();
    EXPECT_EQ(diffengine::Value::arena_offset, 0);

    auto a = diffengine::make_value(2.0f);
    auto b = diffengine::make_value(3.0f);
    auto c = a + b;

    EXPECT_EQ(diffengine::Value::arena_offset, 3);

    diffengine::Value::reset_arena();
    EXPECT_EQ(diffengine::Value::arena_offset, 0);

    auto d = diffengine::make_value(10.0f);
    EXPECT_FLOAT_EQ(d->data, 10.0f);
    EXPECT_EQ(diffengine::Value::arena_offset, 1);
}