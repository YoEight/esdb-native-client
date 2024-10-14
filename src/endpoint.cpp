//
// Created by Yo Eight on 2024-10-14.
//

#include <string>
#include "endpoint.h"

Endpoint::Endpoint(const std::string& host, int port) : host(host), port(port) {}

std::string& Endpoint::getHost() { return host; }

int Endpoint::getPort() const { return port; }