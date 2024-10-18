//
// Created by Yo Eight on 2024-10-14.
//

#include <string>
#include "endpoint.h"

Endpoint::Endpoint(std::string&& host, int port) : host(std::move(host)), port(port) {}

bool Endpoint::operator==(const Endpoint& other) const {
    return host == other.host && port == other.port;
}
