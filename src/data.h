//
// Created by Yo Eight on 2024-10-18.
//

#ifndef DATA_H
#define DATA_H

#include <variant>

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



#endif //DATA_H
