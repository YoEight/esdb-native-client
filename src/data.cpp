//
// Created by Yo Eight on 2024-10-18.
//

#include "data.h"

ExpectedRevision::ExpectedRevision(u_int64_t value) : inner(value) {}
ExpectedRevision::ExpectedRevision(ExpectedAny any) : inner(any) {}
ExpectedRevision::ExpectedRevision(ExpectedExists exists) : inner(exists){}
ExpectedRevision::ExpectedRevision(ExpectedNoStream no_stream) : inner(no_stream) {}

const ExpectedRevision ExpectedRevision::ANY = ExpectedRevision(ExpectedAny{});
const ExpectedRevision ExpectedRevision::STREAM_EXISTS = ExpectedRevision(ExpectedExists{});
const ExpectedRevision ExpectedRevision::NO_STREAM = ExpectedRevision(ExpectedNoStream {});

bool ExpectedRevision::operator==(const ExpectedRevision & other) const {
    return inner.index() == other.inner.index();
}


