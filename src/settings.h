//
// Created by Yo Eight on 2024-10-14.
//

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <vector>
#include "endpoint.h"

enum NodePreference {
    Leader,
    Follower,
    ReadOnlyReplica,
};

class Credentials {
private:
    std::string username;
    // TODO - Do not store password in clear
    std::string password;
};

class Settings {
public:
    std::vector<Endpoint> endpoints;
    std::size_t max_discovery_attempts = 10;
    std::size_t discovery_interval_in_ms = 500;
    std::size_t keep_alive_interval_in_ms = 10000;
    std::size_t keep_alive_timeout_in_ms = 10000;
    std::size_t gossip_timeout = 3000;
    std::optional<std::size_t> default_deadline_in_ms;
    bool discover_dns = false;
    bool secure = true;
    bool certificate_verification = true;
    std::optional<Credentials> credentials;
    NodePreference node_preference = Leader;
    std::optional<std::string> connection_name;
};



#endif //SETTINGS_H
