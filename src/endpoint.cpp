//
// Created by Yo Eight on 2024-10-14.
//

#include <string>
#include "endpoint.h"

Endpoint::Endpoint(std::string&& host, int port) : host(std::move(host)), port(port) {}