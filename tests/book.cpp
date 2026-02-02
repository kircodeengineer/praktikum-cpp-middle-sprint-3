#include <book.hpp>
#include <gtest/gtest.h>
#include <sstream>

using namespace bookdb;

TEST(BookTest, ConstructByEnum) { constexpr Book book{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190}; }
TEST(BookTest, ConstructByStrView) { constexpr Book book{"1984", "George Orwell", 1949, "SciFi"sv, 4., 190}; }
TEST(BookTest, ConstructByConstChar) { constexpr Book book{"1984", "George Orwell", 1949, "SciFi", 4., 190}; }
TEST(BookTest, ConstructByString) {
    std::string g = "SciFi";
    Book test_book_4{"1984", "George Orwell", 1949, g, 4., 190};  // Но не constexpr!
}
TEST(BookTest, FormatterGenre) {
    Genre genre{Genre::Biography};
    std::string expected{"Biography"};
    std::string actual{std::format("{}", genre)};
    EXPECT_EQ(actual, expected);
}
TEST(BookTest, FormatterBook) {
    constexpr Book book{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190};
    std::string expected{"Название: 1984, Автор: George Orwell, Год: 1949, Жанр: SciFi, Рейтинг: 4, Прочтений: 190"};
    std::string actual{std::format("{}", book)};
    EXPECT_EQ(actual, expected);
}
