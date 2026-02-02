#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T container) {
    requires std::same_as<typename T::value_type, Book>;
    { container.begin() } -> std::input_iterator;
    { container.end() } -> std::sentinel_for<decltype(container.begin())>;
    { container.size() } -> std::convertible_to<std::size_t>;
    requires std::ranges::range<T>;
};

template <typename T>
concept TitleAuthorContainerLike = requires(T container) {
    requires std::same_as<typename T::value_type, std::string>;
    { container.begin() } -> std::input_iterator;
    { container.end() } -> std::sentinel_for<decltype(container.begin())>;
    { container.find(std::declval<const std::string &>()) } -> std::same_as<typename T::iterator>;
    {
        container.insert(std::declval<const std::string &>())
    } -> std::convertible_to<std::pair<typename T::iterator, bool>>;
    requires std::ranges::range<T>;
};

template <typename T>
concept BookIterator = std::input_iterator<T> && std::same_as<std::iter_value_t<T>, Book>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I> && std::same_as<std::iter_value_t<I>, Book> && BookIterator<I>;

template <typename P>
concept BookPredicate = std::predicate<P, const Book &>;

template <typename C>
concept BookComparator = std::strict_weak_order<C, Book, Book>;
}  // namespace bookdb