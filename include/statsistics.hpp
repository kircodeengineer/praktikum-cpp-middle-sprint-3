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

template <typename Iterator>
auto calculateGenreRatings(Iterator first, Iterator last) {
    // Тип элемента выводится из итератора
    using BookType = typename std::iterator_traits<Iterator>::value_type;

    std::flat_map<Genre, std::pair<double, size_t>> genre_stats;
    std::for_each(first, last, [&](const BookType &book) {
        auto [it, inserted] = genre_stats.try_emplace(book.genre, std::pair{0, 0});
        auto &sum{it->second.first};
        auto &count{it->second.second};
        sum += book.rating;
        ++count;
    });

    std::flat_map<Genre, double> average_ratings;
    for (const auto &[genre, stats] : genre_stats) {
        auto sum{stats.first};
        auto count{stats.second};
        if (count > 0)
            average_ratings.try_emplace(genre, stats.first / static_cast<double>(stats.second));
    }
    return average_ratings;
}

}  // namespace bookdb
