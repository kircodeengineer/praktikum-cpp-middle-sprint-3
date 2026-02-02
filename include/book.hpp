#pragma once

#include <algorithm>
#include <format>
#include <stdexcept>
#include <string_view>
#include <type_traits>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

struct GenreInfo {
    Genre genre{Genre::Unknown};
    std::string_view name{};
};

using namespace std::literals;

inline constexpr GenreInfo unknownGenreInfo{Genre::Unknown, "Unknown"sv};

inline constexpr std::array<GenreInfo, 6> genres = {{{Genre::Fiction, "Fiction"sv},
                                                     {Genre::NonFiction, "NonFiction"sv},
                                                     {Genre::SciFi, "SciFi"sv},
                                                     {Genre::Biography, "Biography"sv},
                                                     {Genre::Mystery, "Mystery"sv},
                                                     {unknownGenreInfo.genre, unknownGenreInfo.name}}};

constexpr std::string_view GenreToStringView(Genre g) {
    auto it{
        std::find_if(genres.begin(), genres.end(), [g](const GenreInfo &genre_info) { return genre_info.genre == g; })};
    return it != genres.end() ? it->name : unknownGenreInfo.name;
}

constexpr Genre GenreFromString(std::string_view sv) {
    auto it{std::find_if(genres.begin(), genres.end(),
                         [sv](const GenreInfo &genre_info) { return genre_info.name == sv; })};
    return it != genres.end() ? it->genre : unknownGenreInfo.genre;
}

template <typename T>
concept ConvertibleToGenre = std::is_same_v<std::decay_t<T>, Genre> || std::convertible_to<T, std::string_view>;

struct Book {
    // Вынужденное решение сменить тип с std::string на std::string_view, иначе constexpr Book переменные не
    // компилировались
    std::string_view title{};
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author{};

    int year{};
    double rating{};
    int read_count{};
    Genre genre{};

    template <ConvertibleToGenre T>
    explicit constexpr Book(std::string_view title_input, std::string_view author_input, int year_input,
                            T &&genre_input, double rating_input, int read_count_input)
        : title(title_input), author(author_input), year(year_input), rating(rating_input),
          read_count(read_count_input) {
        if constexpr (std::is_same_v<std::decay_t<T>, Genre>)
            this->genre = genre_input;
        else
            this->genre = GenreFromString(genre_input);
    }

    auto operator<=>(const Book &) const = default;
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        return format_to(fc.out(), "{}", bookdb::GenreToStringView(g));
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Book, char> {
    std::string format_str = "Название: {}, Автор: {}, Год: {}, Жанр: {}, Рейтинг: {}, Прочтений: {}";

    constexpr auto parse(format_parse_context &ctx) {
        auto it{ctx.begin()};
        auto end{ctx.end()};

        if (it != end && *it != '}') {
            auto close_brace{std::find(it, end, '}')};
            format_str.assign(it, close_brace);
            it = close_brace;
        }
        return it;
    }

    template <typename FormatContext>
    auto format(const bookdb::Book &book, FormatContext &ctx) const {
        return std::vformat_to(
            ctx.out(), format_str,
            std::make_format_args(book.title, book.author, book.year, book.genre, book.rating, book.read_count));
    }
};

}  // namespace std
