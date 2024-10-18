//
// Created by Yo Eight on 2024-10-14.
//

#include <string>
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "connection_string.h"

using json = nlohmann::json;

class Expectation {
public:
    std::string connection_string;
    Settings expected;

    explicit Expectation(std::string&& connection_string) : connection_string(std::move(connection_string)) {}
};

Endpoint parse_endpoint(std::string&&  str) {
    const auto pos = str.find(':');

    if (pos == std::string::npos) {
        return { std::move(str), 2113 };
    }

    auto host = str.substr(0, pos);
    const auto port_str = str.substr(pos + 1);

    return { std::move(host), std::stoi(port_str) };
}

std::vector<Expectation> loadValidConnectionStringTests() {
    std::ifstream file("valid_connection_strings.json");
    std::vector<Expectation> expectations;

    json data = json::parse(file);
    for (auto test: data.get<std::vector<json>>()) {
        auto connection_string = test.at("connection").get<std::string>();
        auto expectation = Expectation(std::move(connection_string));
        auto params = test.at("expectation");

        for (const auto& param : params.items()) {
            auto key = param.key();

            if (key == "endpoints") {
                if (param.value().is_array()) {
                    for (auto uri : param.value().get<std::vector<std::string>>()) {
                        expectation.expected.endpoints.push_back(parse_endpoint(std::move(uri)));
                    }
                } else {
                    expectation.expected.endpoints.push_back(parse_endpoint(std::move(param.value())));
                }
            }
        }

        expectations.push_back(expectation);
    }

    return expectations;
}

class ValidConnectionStringTests : public ::testing::TestWithParam<Expectation> {};

TEST_P(ValidConnectionStringTests, CheckValidConnectionStrings) {
    Expectation expectation = GetParam();
    Settings actual;
    auto parsed = tryParseSettings(expectation.connection_string, &actual);

    EXPECT_TRUE(parsed);
    EXPECT_EQ(expectation.expected, actual);
}

INSTANTIATE_TEST_SUITE_P(
    LoadFromFile,
    ValidConnectionStringTests,
    ::testing::ValuesIn(loadValidConnectionStringTests()));

TEST(ConnectionStringTest, ParseConnectionString) {
    Settings settings;
    auto parsed = tryParseSettings("esdb://admin:changeit@node1:1111,node2:2222,node3:3333?tls=true&maxdiscoverattempts=524", &settings);
    EXPECT_TRUE(parsed);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}