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
    std::string name;
    std::string connection_string;
    Settings expected;

    explicit Expectation(std::string&& name, std::string&& connection_string)
        : name(std::move(name)), connection_string(std::move(connection_string)) {}
};

void PrintTo(const Expectation& expectation, std::ostream* os) {
    *os << expectation.connection_string;
}

Endpoint parse_endpoint(std::string&&  str) {
    const auto pos = str.find(':');

    if (pos == std::string::npos) {
        return { std::move(str), 2113 };
    }

    auto host = str.substr(0, pos);
    const auto port_str = str.substr(pos + 1);

    return { std::move(host), std::stoi(port_str) };
}

std::string print_settings(const ::testing::TestParamInfo<Expectation>& param) {
    return param.param.name;
}

std::vector<Expectation> load_valid_connection_string_tests() {
    std::ifstream file("valid_connection_strings.json");
    std::vector<Expectation> expectations;

    json data = json::parse(file);
    for (auto test: data.get<std::vector<json>>()) {
        auto name = test.at("name").get<std::string>();
        auto connection_string = test.at("connection").get<std::string>();
        auto expectation = Expectation(std::move(name), std::move(connection_string));
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
            } else if (key == "credentials") {
                auto parts = param.value().get<json>();
                auto username = parts.at("username").get<std::string>();
                auto password = parts.at("password").get<std::string>();

                expectation.expected.credentials = Credentials(std::move(username),std::move(password));
            } else if (key == "tlsVerifyCert") {
                expectation.expected.certificate_verification = param.value().get<bool>();
            } else if (key == "tls") {
                expectation.expected.secure = param.value().get<bool>();
            } else if (key == "maxDiscoverAttempts") {
                expectation.expected.max_discover_attempts = param.value().get<int>();
            } else if (key == "discoveryInterval") {
                expectation.expected.discovery_interval_in_ms = param.value().get<int>();
            } else if (key == "gossipTimeout") {
                expectation.expected.gossip_timeout_in_ms = param.value().get<int>();
            } else if (key == "keepAliveTimeout") {
                expectation.expected.keep_alive_timeout_in_ms = param.value().get<int>();
            } else if (key == "keepAliveInterval") {
                expectation.expected.keep_alive_interval_in_ms = param.value().get<int>();
            } else if (key == "tlsCaFile") {
                expectation.expected.tls_ca_file = param.value().get<std::string>();
            } else if (key == "userCertFile") {
                expectation.expected.user_cert_file = param.value().get<std::string>();
            } else if (key == "userKeyFile") {
                expectation.expected.user_key_file = param.value().get<std::string>();
            } else if (key == "nodePreference") {
                auto pref = param.value().get<std::string>();

                if (pref == "leader") {
                    expectation.expected.node_preference = Leader;
                } else if (pref == "follower") {
                    expectation.expected.node_preference = Follower;
                } else if (pref == "readonlyreplica") {
                    expectation.expected.node_preference = ReadOnlyReplica;
                } else if (pref == "random") {
                    expectation.expected.node_preference = Random;
                } else {
                    throw new std::invalid_argument("unknown node preference: " + pref);
                }
            } else {
                throw new std::invalid_argument("unknown param: " + key);
            }
        }

        expectations.push_back(expectation);
    }

    return expectations;
}

class ValidConnectionStringTests : public ::testing::TestWithParam<Expectation> {};

TEST_P(ValidConnectionStringTests, CheckValidConnectionString) {
    Expectation expectation = GetParam();

    Settings actual;
    auto parsed = try_parse_settings(expectation.connection_string, &actual);

    ASSERT_TRUE(parsed);
    ASSERT_EQ(expectation.expected.endpoints.size(), actual.endpoints.size());
    ASSERT_EQ(expectation.expected.endpoints, actual.endpoints);
    ASSERT_EQ(expectation.expected.max_discover_attempts, actual.max_discover_attempts);
    ASSERT_EQ(expectation.expected.discovery_interval_in_ms, actual.discovery_interval_in_ms);
    ASSERT_EQ(expectation.expected.gossip_timeout_in_ms, actual.gossip_timeout_in_ms);
    ASSERT_EQ(expectation.expected.keep_alive_interval_in_ms, actual.keep_alive_interval_in_ms);
    ASSERT_EQ(expectation.expected.keep_alive_timeout_in_ms, actual.keep_alive_timeout_in_ms);
    ASSERT_EQ(expectation.expected.default_deadline_in_ms, actual.default_deadline_in_ms);
    ASSERT_EQ(expectation.expected.discover_dns, actual.discover_dns);
    ASSERT_EQ(expectation.expected.secure, actual.secure);
    ASSERT_EQ(expectation.expected.certificate_verification, actual.certificate_verification);
    ASSERT_EQ(expectation.expected.credentials, actual.credentials);
    ASSERT_EQ(expectation.expected.node_preference, actual.node_preference);
    ASSERT_EQ(expectation.expected.connection_name, actual.connection_name);
    ASSERT_EQ(expectation.expected, actual);
}

INSTANTIATE_TEST_SUITE_P(
    LoadConnectionStringJsonFile,
    ValidConnectionStringTests,
    ::testing::ValuesIn(load_valid_connection_string_tests()),
    print_settings);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}