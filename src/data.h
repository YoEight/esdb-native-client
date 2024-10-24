//
// Created by Yo Eight on 2024-10-18.
//

#ifndef DATA_H
#define DATA_H

#include <string>
#include <variant>
#include <boost/uuid/uuid.hpp>

using uuid = boost::uuids::uuid;

class ExpectedAny {};
class ExpectedNoStream {};
class ExpectedExists {};

class ExpectedRevision {
    std::variant<ExpectedAny, ExpectedNoStream, ExpectedExists, u_int64_t> inner;
    explicit ExpectedRevision(ExpectedAny);
    explicit ExpectedRevision(ExpectedNoStream);
    explicit ExpectedRevision(ExpectedExists);

public:
    explicit ExpectedRevision(u_int64_t);

    static const ExpectedRevision ANY;
    static const ExpectedRevision NO_STREAM;
    static const ExpectedRevision STREAM_EXISTS;

    bool operator==(const ExpectedRevision &) const;
};

class StreamStart{};
class StreamEnd{};

class StreamPosition {
    std::variant<StreamStart, StreamEnd, u_int64_t> inner;

    explicit StreamPosition(StreamStart);
    explicit StreamPosition(StreamEnd);
public:
    explicit StreamPosition(u_int64_t);

    bool operator==(const StreamPosition &) const;
};

enum Direction {
    Forwards,
    Backwards,
};

class WriteResult {
public:
    u_int64_t next_stream_position;
    u_int64_t position;
};

class RecordedEvent {
    std::string stream_id;
    uuid id;
    u_int64_t revision = 0;
    std::string type;
    std::string data;
    std::unordered_map<std::string, std::string> metadata;
    std::string custom_metadata;
    bool json = false;
    u_int64_t position = 0;
    long timestamp = 0;

public:
    const std::string& get_stream_id() const;
    const uuid& get_id() const;
    u_int64_t get_revision() const;
    const std::string& get_type() const;
    const std::string& get_data() const;
    bool is_json() const;
    const std::unordered_map<std::string, std::string>& get_metadata() const;
    u_int64_t get_position() const;
    long get_timestamp() const;
};

class ResolvedEvent {
    std::optional<RecordedEvent> link;
    std::optional<RecordedEvent> event;
    std::optional<u_int64_t> position;
public:
    const std::optional<RecordedEvent>& get_link() const;
    const std::optional<RecordedEvent>& get_event() const;
    const std::optional<u_int64_t>& get_position() const;
};


#endif //DATA_H
