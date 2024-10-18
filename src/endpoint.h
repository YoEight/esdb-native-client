//
// Created by Yo Eight on 2024-10-14.
//

#ifndef ENDPOINT_H
#define ENDPOINT_H


#include <string>

class Endpoint {
public:
    std::string host;
    int port;

    Endpoint(std::string&& host, int port);

    bool operator==(const Endpoint& other) const;
};



#endif //ENDPOINT_H
