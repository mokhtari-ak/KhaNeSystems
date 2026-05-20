#include <gtest/gtest.h>
#include "khane_systems/navigation/ParameterServer.hpp"

TEST(ParameterServerTest, ValidationAndGetSetWorks) {
    khane::navigation::ParameterServer server;
    server.register_param("p1", {0.0f, 10.0f, 5.0f});

    // Test valid set
    EXPECT_TRUE(server.set_param("p1", 8.0f));
    EXPECT_FLOAT_EQ(std::get<float>(server.get_param("p1")), 8.0f);

    // Test invalid set
    EXPECT_FALSE(server.set_param("p1", 15.0f));
    EXPECT_FLOAT_EQ(std::get<float>(server.get_param("p1")), 8.0f);
}
