//
// Created by Yo Eight on 2024-10-14.
//

#include "settings.h"

Credentials::Credentials(std::string &&username, std::string &&password)
    : username(std::move(username)), password(std::move(password)) {
}

bool Credentials::operator==(const Credentials &other) const {
    return username == other.username && password == other.password;
}

bool Settings::operator==(const Settings &other) const {
    return endpoints == other.endpoints
           && max_discovery_attempts == other.max_discovery_attempts
           && discovery_interval_in_ms == other.discovery_interval_in_ms
           && gossip_timeout_in_ms == other.gossip_timeout_in_ms
           && keep_alive_interval_in_ms == other.keep_alive_interval_in_ms
           && keep_alive_timeout_in_ms == other.keep_alive_timeout_in_ms
           && default_deadline_in_ms == other.default_deadline_in_ms
           && discover_dns == other.discover_dns
           && secure == other.secure
           && certificate_verification == other.certificate_verification
           && credentials == other.credentials
           && node_preference == other.node_preference
           && connection_name == other.connection_name;
};
