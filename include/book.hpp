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
// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь
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

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author{};
    std::string title{};

    int year{};
    double rating{};
    int read_count{};
    Genre genre{};

    template <typename T>
    explicit constexpr Book(std::string_view author_input, std::string title_input, int year_input, T &&genre_input,
                            double rating_input, int read_count_input)
        : author(author_input), title(std::move(title_input)), year(year_input), rating(rating_input),
          read_count(read_count_input) {
        if constexpr (std::is_same_v<std::decay_t<T>, Genre>)
            this->genre = genre_input;
        else
            this->genre = GenreFromString(genre_input);
    }
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

// Ваш код для std::formatter<Book> здесь

}  // namespace std
