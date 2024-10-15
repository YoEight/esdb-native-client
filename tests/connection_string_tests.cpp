//
// Created by Yo Eight on 2024-10-14.
//

#include <string>
#include <gtest/gtest.h>
#include "connection_string.h"

TEST(ConnectionStringTest, ParseConnectionString) {
    Settings settings;
    auto parsed = tryParseSettings("esdb://admin:changeit@node1:1111,node2:2222,node3:3333?tls=true&maxdiscoverattempts=524", &settings);
    EXPECT_TRUE(parsed);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}