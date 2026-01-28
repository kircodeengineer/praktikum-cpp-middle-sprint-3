#pragma once

#include <algorithm>
#include <flat_map>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book_database.hpp"

#include <print>

namespace bookdb {
template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<std::string_view, size_t, Comparator> histogram(comp);
    for (const auto &book : cont) {
        auto [it, inserted] = histogram.try_emplace(book.author, 0);
        ++it->second;
    }
    return histogram;
}

}  // namespace bookdb
