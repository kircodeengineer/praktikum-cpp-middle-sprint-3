#include <algorithm>

#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statsistics.hpp"

using namespace bookdb;

int main() {
    //
    // Ниже приведён пример работы `BookDatabase`.
    //
    //     - Обратите внимание, что в этой функции реализованы основные возможности, охватывающие как обязательные, так
    //     и опциональные требования,
    //       которые не обязательны к реализации для сдачи работы.
    //     - Не забудьте перед созданием коммита вызвать 'run_clang_format.sh' для форматирования кода
    //
    // 1. Через enum (OK)
    constexpr Book test_book_1{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190};
    // 2. Через string_view (OK)
    constexpr Book test_book_2{"1984", "George Orwell", 1949, "SciFi"sv, 4., 190};
    // 3. Через const char* (OK — преобразуется в string_view)
    constexpr Book test_book_3{"1984", "George Orwell", 1949, "SciFi", 4., 190};
    // 4. Через std::string (OK, если не в constexpr-контексте)
    std::string g = "SciFi";
    Book test_book_4{"1984", "George Orwell", 1949, g, 4., 190};  // Но не constexpr!

    std::println("{}", test_book_1);
    std::println("{}", bookdb::Genre::Biography);
    // 5. Недопустимый тип (ошибка компиляции)
    // Book b5{"A", "T", 2023, 42, 0.0, 0};  // int не удовлетворяет ConvertibleToGenre
    // Create a book database
    BookDatabase<std::vector<Book>> db;

    /*

    Код закомментирован, чтобы не приводить к ошибке компиляции

    // Add some books
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
    db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    db.EmplaceBack("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
    db.EmplaceBack("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
    db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
    std::print("Books: {}\n\n", db);

    // Sorts
    std::sort(db.begin(), db.end(), comp::LessByAuthor{});
    std::print("Books sorted by author: {}\n\n==================\n", db);

    std::sort(db.begin(), db.end(), comp::LessByPopularity{});
    std::print("Books sorted by popularity: {}\n\n==================\n", db);

    // Author histogram
    auto histogram = buildAuthorHistogramFlat(db);
    std::print("Author histogram: {}", histogram);

    // Ratings
    auto genreRatings = calculateGenreRatings(db.begin(), db.end());
    std::print("\n\nAverage ratings by genres: {}\n", genreRatings);

    auto avrRating = calculateAverageRating(db);
    std::print("Average books rating in library: {}\n", avrRating);

    // Filters
    auto filtered = filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
    std::print("\n\nBooks from the 20th century with rating ≥ 4.5:\n");
    std::for_each(filtered.cbegin(), filtered.cend(), [](const auto &v) { std::print("{}\n", v.get()); });

    // Top 3 books
    auto topBooks = getTopNBy(db, 3, comp::LessByRating{});
    std::print("\n\nTop 3 books by rating:\n");
    std::for_each(topBooks.cbegin(), topBooks.cend(), [](const auto &v) { std::print("{}\n", v.get()); });

    auto orwellBookIt = std::find_if(db.begin(), db.end(), [](const auto &v) { return v.author == "George Orwell"; });
    if (orwellBookIt != db.end()) {
        std::print("\n\nTransparent lookup by authors. Found Orwell's book: {}\n", *orwellBookIt);
    }
    */

    return 0;
}