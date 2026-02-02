#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb { namespace filters {
[[nodiscard]] inline auto YearBetween(int min_year, int max_year) -> BookPredicate auto {
    return [min_year, max_year](const Book &book) { return book.year >= min_year && book.year <= max_year; };
}

[[nodiscard]] inline auto RatingAbove(double min_rating) -> BookPredicate auto {
    return [min_rating](const Book &book) { return book.rating >= min_rating; };
}

[[nodiscard]] inline auto GenreIs(Genre target_genre) -> BookPredicate auto {
    return [target_genre](const Book &book) { return book.genre == target_genre; };
}

template <BookPredicate... Preds>
[[nodiscard]] auto all_of(Preds &&...preds) -> BookPredicate auto {
    return [... preds = std::forward<Preds>(preds)](const Book &book) { return (preds(book) && ...); };
}

template <BookPredicate... Preds>
[[nodiscard]] auto any_of(Preds &&...preds) -> BookPredicate auto {
    return [... preds = std::forward<Preds>(preds)](const Book &book) { return (preds(book) || ...); };
}

template <BookIterator Iter, BookSentinel<Iter> Sentinel>
[[nodiscard]] std::vector<std::reference_wrapper<const Book>> filterBooks(Iter first, Sentinel last,
                                                                          BookPredicate auto &&predicate) {
    std::vector<std::reference_wrapper<const Book>> result;

    std::copy_if(first, last, std::back_inserter(result), [&predicate](const auto &book) { return predicate(book); });

    return result;
}
}}  // namespace bookdb::filters