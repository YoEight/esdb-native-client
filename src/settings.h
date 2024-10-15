//
// Created by Yo Eight on 2024-10-14.
//

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <vector>
#include <cstdint>
#include "endpoint.h"

enum NodePreference {
    Leader,
    Follower,
    ReadOnlyReplica,
    Random,
};

class Credentials {
public:
    std::string username;
    // TODO - Do not store password in clear
    std::string password;

    Credentials(std::string&& username, std::string&& password);
};

class Settings {
public:
    std::vector<Endpoint> endpoints;
    u_int64_t max_discovery_attempts = 10;
    u_int64_t discovery_interval_in_ms = 500;
    u_int64_t gossip_timeout_in_ms = 3000;
    int64_t keep_alive_interval_in_ms = 10000;
    int64_t keep_alive_timeout_in_ms = 10000;
    std::optional<int64_t> default_deadline_in_ms;
    bool discover_dns = false;
    bool secure = true;
    bool certificate_verification = true;
    std::optional<Credentials> credentials;
    NodePreference node_preference = Leader;
    std::optional<std::string> connection_name;
};



#endif //SETTINGS_H
