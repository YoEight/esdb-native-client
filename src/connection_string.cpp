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

bool tryParseUrl(const UriUriA& url, bool gossip_seed, Settings* settings) {
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

    if (gossip_seed) {
        auto current_node = url.pathHead;

        do {
            auto text = current_node->text;
            auto parse_host = true;
            std::string host;
            std::string port_str;

            for (auto p = text.first; p < text.afterLast; p++) {
                if (*p == ':') {
                    parse_host = false;
                    continue;
                }

                parse_host ? host.push_back(*p) : port_str.push_back(*p);
            }

            auto node = Endpoint(std::move(host), 2113);

            if (!port_str.empty() && !tryParsePortNumber(port_str, &node.port))
                return false;

            settings->endpoints.push_back(node);
            current_node = current_node->next;
        } while (current_node != nullptr);
    }

    if (url.userInfo.first != nullptr) {
        std::string username;
        std::string password;

        auto parsing_username = true;
        for (auto p = url.userInfo.first; p < url.userInfo.afterLast; p++) {
            if (*p == ':') {
                parsing_username = false;
                continue;
            }

            parsing_username ? username.push_back(*p) : password.push_back(*p);
        }

        settings->credentials = Credentials(std::move(username), std::move(password));
    }

    return true;
}

bool tryParseSettings(std::string connection_string, Settings *settings) {
    UriUriA url;
    const char * error_pos;
    bool gossip_seed = false;

    if (uriParseSingleUriA(&url, connection_string.c_str(), &error_pos) != URI_SUCCESS) {
        if (connection_string.find(',') == std::string::npos)
            return false;

        std::ranges::replace(connection_string, ',', '/');

        if (uriParseSingleUriA(&url, connection_string.c_str(), &error_pos) != URI_SUCCESS)
            return false;

        gossip_seed = true;
    }

    const auto result = tryParseUrl(url, gossip_seed, settings);

    uriFreeUriMembersA(&url);

    return result;
}

