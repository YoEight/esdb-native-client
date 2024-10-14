//
// Created by Yo Eight on 2024-10-14.
//

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include "endpoint.h"

class Settings {
private:
    bool discover_dns = false;
    Endpoint*endpoint= nullptr;
};



#endif //SETTINGS_H
