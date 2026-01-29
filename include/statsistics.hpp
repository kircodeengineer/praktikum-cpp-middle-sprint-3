#pragma once

#include <algorithm>
#include <flat_map>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book_database.hpp"
#include "concepts.hpp"

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

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &cont) {
    if (cont.empty())
        return 0.0;

    double total_rating{
        std::accumulate(cont.begin(), cont.end(), 0.0, [](double sum, const auto &book) { return sum + book.rating; })};

    return total_rating / static_cast<double>(cont.size());
}

template <BookContainerLike T>
std::vector<std::reference_wrapper<const Book>> sampleRandomBooks(const BookDatabase<T> &cont, size_t sample_size) {
    if (sample_size > cont.size()) {
        throw std::out_of_range("Размер выборки превышает количество книг в базе");
    }
    if (sample_size == 0) {
        return {};
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::vector<size_t> indices(cont.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::shuffle(indices.begin(), indices.end(), gen);

    std::vector<std::reference_wrapper<const Book>> result;
    result.reserve(sample_size);

    for (size_t i = 0; i < sample_size; ++i) {
        result.emplace_back(cont[indices[i]]);
    }

    return result;
}

template <BookContainerLike T>
std::vector<std::reference_wrapper<const Book>> getTopNBy(BookDatabase<T> &cont, size_t n, BookComparator auto &&comp) {
    if (n == 0 || cont.empty())
        return {};

    n = std::min(n, cont.size());

    auto middle{cont.begin()};
    std::advance(middle, n);

    std::partial_sort(cont.begin(), middle, cont.end(), comp);

    std::vector<std::reference_wrapper<const Book>> result;
    result.reserve(n);

    for (auto it = cont.begin(); it != middle; ++it) {
        result.emplace_back(*it);
    }

    return result;
}
}  // namespace bookdb
