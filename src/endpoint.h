//
// Created by Yo Eight on 2024-10-14.
//

#ifndef ENDPOINT_H
#define ENDPOINT_H


#include <string>

class Endpoint {
private:
    std::string host;
    int port;

public:
    Endpoint(const std::string& host, int port);

    std::string& getHost();
    int getPort() const;
};



#endif //ENDPOINT_H
