#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.author < rhs.author; }
};

struct LessByTitle {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.title < rhs.title; }
};

struct LessByYear {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.year < rhs.year; }
};

struct LessByRating {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.rating < rhs.rating; }
};

struct LessByReadCount {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.read_count < rhs.read_count; }
};

struct LessByGenre {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.genre < rhs.genre; }
};
}  // namespace bookdb::comp