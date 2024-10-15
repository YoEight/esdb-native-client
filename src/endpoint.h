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
};



#endif //ENDPOINT_H
