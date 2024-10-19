//
// Created by Yo Eight on 2024-10-14.
//

#include <uriparser/Uri.h>
#include "connection_string.h"

bool try_parse_integer(const std::string& str, int* port) {
    try {
        *port = std::stoi(str);
        return true;
    } catch (const std::invalid_argument& _) {
        return false;
    } catch (const std::out_of_range& _) {
        return false;
    }
}

bool try_parse_u_int64_t(const std::string& str, u_int64_t* port) {
    try {
        *port = std::stoll(str);
        return true;
    } catch (const std::invalid_argument& _) {
        return false;
    } catch (const std::out_of_range& _) {
        return false;
    }
}

bool try_parse_int64_t(const std::string& str, int64_t* port) {
    try {
        *port = std::stoll(str);
        return true;
    } catch (const std::invalid_argument& _) {
        return false;
    } catch (const std::out_of_range& _) {
        return false;
    }
}

bool try_parse_bool(std::string& str, bool* value) {
    if (str == "true") {
        *value = true;
        return true;
    }

    if (str == "false") {
        *value = false;
        return true;
    }

    return false;
}

bool try_handle_param(std::string& name, std::string& value, Settings* settings) {
    if (name == "tls") {
        if (!try_parse_bool(value, &settings->secure))
            return false;
    } else if (name == "tlsverifycert") {
        if (!try_parse_bool(value, &settings->certificate_verification))
            return false;
    } else if (name == "maxdiscoverattempts") {
        if (!try_parse_u_int64_t(value, &settings->max_discover_attempts))
            return false;
    } else if (name == "discoveryinterval") {
        if (!try_parse_u_int64_t(value, &settings->discovery_interval_in_ms))
            return false;
    } else if (name == "gossiptimeout") {
        if (!try_parse_u_int64_t(value, &settings->gossip_timeout_in_ms))
            return false;
    } else if (name == "keepaliveinterval") {
        if (!try_parse_int64_t(value, &settings->keep_alive_interval_in_ms))
            return false;

        if (settings->keep_alive_interval_in_ms == -1)
            settings->keep_alive_interval_in_ms = INT64_MAX;
        else if (settings->keep_alive_interval_in_ms < -1)
            return false;
    } else if (name == "keepalivetimeout") {
        if (!try_parse_int64_t(value, &settings->keep_alive_timeout_in_ms))
            return false;

        if (settings->keep_alive_timeout_in_ms == -1)
            settings->keep_alive_timeout_in_ms = INT64_MAX;
        else if (settings->keep_alive_timeout_in_ms < -1)
            return false;
    } else if (name == "defaultdeadline") {
        int64_t deadline;
        if (!try_parse_int64_t(value, &deadline))
            return false;

        if (deadline == -1)
            settings->default_deadline_in_ms = INT64_MAX;
        else if (deadline < -1)
            return false;
        else
            settings-> default_deadline_in_ms = deadline;
    } else if (name == "nodepreference") {
        if (value == "leader") {
            settings->node_preference = Leader;
        } else if (value == "follower") {
            settings->node_preference = Follower;
        } else if (value == "readonlyreplica") {
            settings->node_preference = ReadOnlyReplica;
        } else if (value == "random") {
            settings->node_preference = Random;
        }
    } else if (name == "connectionname") {
        settings->connection_name = value;
    }

    return true;
}

bool try_parse_url(const UriUriA& url, bool gossip_seed, Settings* settings) {
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
        if (!try_parse_integer(port_str, &port))
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

            if (!port_str.empty() && !try_parse_integer(port_str, &node.port))
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

        uriUnescapeInPlaceA(username.data());
        uriUnescapeInPlaceA(password.data());
        username = std::string(username.data());
        password = std::string(password.data());
        settings->credentials = Credentials(std::move(username), std::move(password));
    }

    if (url.query.first != nullptr) {
        std::string name;
        std::string value;
        bool parse_name = true;

        for (auto p = url.query.first; p < url.query.afterLast; p++) {
            if (*p == '=') {
                parse_name = false;
                continue;
            }

            if (*p == '&') {
                if (!try_handle_param(name, value, settings))
                    return false;

                parse_name = true;
                name.clear();
                value.clear();
                continue;
            }

            parse_name ? name.push_back(std::tolower(*p)) : value.push_back(std::tolower(*p));
        }

        if (!name.empty()) {
            if (!try_handle_param(name, value, settings))
                return false;
        }
    }

    return true;
}

bool try_parse_settings(std::string connection_string, Settings *settings) {
    UriUriA url;
    const char * error_pos;
    bool gossip_seed = false;

    if (connection_string.find(',') != std::string::npos) {
        std::ranges::replace(connection_string, ',', '/');
        gossip_seed = true;
    }

    if (uriParseSingleUriA(&url, connection_string.c_str(), &error_pos) != URI_SUCCESS)
        return true;

    const auto result = try_parse_url(url, gossip_seed, settings);

    uriFreeUriMembersA(&url);

    return result;
}

