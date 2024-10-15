//
// Created by Yo Eight on 2024-10-14.
//

#include "settings.h"

Credentials::Credentials(std::string &&username, std::string &&password)
    : username(std::move(username)), password(std::move(password)) {}
