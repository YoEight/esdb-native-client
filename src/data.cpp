//
// Created by Yo Eight on 2024-10-18.
//

#include "data.h"

ExpectedRevision::ExpectedRevision(u_int64_t value) : inner(value) {
}

ExpectedRevision::ExpectedRevision(ExpectedAny any) : inner(any) {
}

ExpectedRevision::ExpectedRevision(ExpectedExists exists) : inner(exists) {
}

ExpectedRevision::ExpectedRevision(ExpectedNoStream no_stream) : inner(no_stream) {
}

const ExpectedRevision ExpectedRevision::ANY = ExpectedRevision(ExpectedAny{});
const ExpectedRevision ExpectedRevision::STREAM_EXISTS = ExpectedRevision(ExpectedExists{});
const ExpectedRevision ExpectedRevision::NO_STREAM = ExpectedRevision(ExpectedNoStream{});

bool ExpectedRevision::operator==(const ExpectedRevision &other) const {
    if (inner.index() != other.inner.index())
        return false;

    if (!std::holds_alternative<u_int64_t>(inner))
        return true;

    return std::get<u_int64_t>(inner) == std::get<u_int64_t>(other.inner);
}

StreamPosition::StreamPosition(u_int64_t value) : inner(value) {
}

StreamPosition::StreamPosition(StreamStart value) : inner(value) {
}

StreamPosition::StreamPosition(StreamEnd value) : inner(value) {
}

bool StreamPosition::operator==(const StreamPosition &other) const {
    if (inner.index() != other.inner.index())
        return false;

    if (!std::holds_alternative<u_int64_t>(inner))
        return true;

    return std::get<u_int64_t>(inner) == std::get<u_int64_t>(other.inner);
}

const std::string &RecordedEvent::get_stream_id() const {
    return stream_id;
}

const uuid &RecordedEvent::get_id() const {
    return id;
}

const std::string &RecordedEvent::get_data() const {
    return data;
}

const std::unordered_map<std::string, std::string> &RecordedEvent::get_metadata() const {
    return metadata;
}

const std::string &RecordedEvent::get_type() const {
    return type;
}

u_int64_t RecordedEvent::get_position() const {
    return position;
}

u_int64_t RecordedEvent::get_revision() const {
    return revision;
}

long RecordedEvent::get_timestamp() const {
    return timestamp;
}

bool RecordedEvent::is_json() const {
    return json;
}

const std::optional<RecordedEvent> &ResolvedEvent::get_event() const {
    return event;
}

const std::optional<RecordedEvent> &ResolvedEvent::get_link() const {
    return link;
}

const std::optional<u_int64_t> &ResolvedEvent::get_position() const {
    return position;
}

const RecordedEvent &ResolvedEvent::get_original_event() const {
    return link.has_value() ? link.value() : event.value();
}
