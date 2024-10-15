//
// Created by Yo Eight on 2024-10-14.
//

#include <uriparser/Uri.h>
#include "connection_string.h"

bool tryParsePortNumber(const std::string& str, int* port) {
    try {
        *port = std::stoi(str);
        return true;
    } catch (const std::invalid_argument& _) {
        return false;
    } catch (const std::out_of_range& _) {
        return false;
    }
}

bool tryParseUrl(const UriUriA& url, Settings* settings) {
    if (url.scheme.first == nullptr && url.scheme.afterLast == nullptr)
        return false;

    std::string scheme;

    for (auto p = url.scheme.first; p < url.scheme.afterLast; p++) {
        scheme.push_back(*p);
    }

    if (scheme != "esdb" && scheme != "esdb+discover")
        return false;

    *settings = Settings();
    settings->discover_dns = scheme == "esdb+discover";

    std::string host;
    for (auto p = url.hostText.first; p < url.hostText.afterLast; p++) {
        host.push_back(*p);
    }

    auto endpoint = Endpoint(std::move(host), 2113);

    if (url.portText.first != nullptr) {
        std::string port_str;
        for (auto p = url.portText.first; p < url.portText.afterLast; p++) {
            port_str.push_back(*p);
        }

        int port;
        if (!tryParsePortNumber(port_str, &port))
            return false;

        endpoint.port = port;
    }

    settings->endpoints.push_back(endpoint);

    return true;
}

bool tryParseSettings(std::string connection_string, Settings *settings) {
    UriUriA url;
    const char * error_pos;

    if (uriParseSingleUriA(&url, connection_string.c_str(), &error_pos) != URI_SUCCESS) {
        if (connection_string.find(',') == std::string::npos)
            return false;

        std::ranges::replace(connection_string, ',', '/');

        if (uriParseSingleUriA(&url, connection_string.c_str(), &error_pos) != URI_SUCCESS)
            return false;
    }

    const auto result = tryParseUrl(url, settings);

    uriFreeUriMembersA(&url);

    return result;
}

