#include <book.hpp>
#include <book_database.hpp>
#include <gtest/gtest.h>
#include <statistics.hpp>

using namespace bookdb;

TEST(CalculateAverageRatingTest, SingleBook) {
    Book book{"Title", "Author", 2020, Genre::Fiction, 4.5, 10};
    std::span<const Book> span{&book, 1};

    double result{calculateAverageRating(span)};

    EXPECT_DOUBLE_EQ(result, 4.5);
}

TEST(CalculateAverageRatingTest, MultipleBooks) {
    Book books[] = {Book{"A", "Author1", 2020, Genre::Fiction, 5.0, 10},
                    Book{"B", "Author2", 2021, Genre::SciFi, 3.0, 5},
                    Book{"C", "Author3", 2022, Genre::Mystery, 4.0, 8}};
    std::span<const Book> span{books};

    double result{calculateAverageRating(span)};

    // (5.0 + 3.0 + 4.0) / 3 = 4.0
    EXPECT_DOUBLE_EQ(result, 4.0);
}

TEST(CalculateAverageRatingTest, EmptySpan) {
    std::vector<Book> empty_books;
    std::span<const Book> span{empty_books};

    double result{calculateAverageRating(span)};

    EXPECT_DOUBLE_EQ(result, 0.0);
}

TEST(CalculateAverageRatingTest, LargeDataset) {
    const size_t count = 10'000;
    std::vector<Book> books(count, Book{"", "", 0, Genre::Unknown, 3.14, 0});
    std::span<const Book> span{books};

    double result{calculateAverageRating(span)};

    EXPECT_NEAR(result, 3.14, 1e-10);
}

TEST(BuildAuthorHistogramFlatTest, SingleAuthor) {
    BookDatabase<std::vector<Book>> db{{Book{"A", "Author1", 2020, Genre::Fiction, 4.0, 10}}};
    auto hist{buildAuthorHistogramFlat(db)};
    EXPECT_EQ(hist.size(), 1);
    EXPECT_EQ(hist["Author1"sv], 1);
}

TEST(BuildAuthorHistogramFlatTest, MultipleAuthors) {
    BookDatabase<std::vector<Book>> db{{Book{"A", "Author1", 2020, Genre::Fiction, 4.0, 10},
                                        Book{"B", "Author2", 2021, Genre::SciFi, 3.0, 5},
                                        Book{"C", "Author1", 2022, Genre::Mystery, 5.0, 8}}};
    auto hist = buildAuthorHistogramFlat(db);
    EXPECT_EQ(hist.size(), 2);
    EXPECT_EQ(hist["Author1"sv], 2);
    EXPECT_EQ(hist["Author2"sv], 1);
}

TEST(BuildAuthorHistogramFlatTest, EmptyDatabase) {
    BookDatabase<std::vector<Book>> db;
    auto hist{buildAuthorHistogramFlat(db)};
    EXPECT_TRUE(hist.empty());
}

TEST(BuildAuthorHistogramTest, SameAuthorDifferentCase) {
    BookDatabase<std::vector<Book>> db{
        {Book{"A", "author", 2020, Genre::Fiction, 4.0, 10}, Book{"B", "Author", 2021, Genre::SciFi, 3.0, 5}}};
    auto hist{buildAuthorHistogramFlat(db)};
    EXPECT_EQ(hist.size(), 2);
}

TEST(CalculateGenreRatingsTest, SingleGenre) {
    std::vector<Book> books{Book{"A", "Auth", 2020, Genre::Fiction, 4.0, 10},
                            Book{"B", "Auth", 2021, Genre::Fiction, 5.0, 5}};
    auto ratings{calculateGenreRatings(books)};
    EXPECT_EQ(ratings.size(), 1);
    EXPECT_NEAR(ratings[Genre::Fiction], 4.5, 1e-10);  // (4+5)/2
}

TEST(CalculateGenreRatingsTest, MultipleGenres) {
    std::vector<Book> books{
        Book{"A", "Auth", 2020, Genre::Fiction, 4.0, 10}, Book{"B", "Auth", 2021, Genre::SciFi, 3.0, 5},
        Book{"C", "Auth", 2022, Genre::Mystery, 5.0, 8}, Book{"D", "Auth", 2023, Genre::SciFi, 5.0, 3}};
    auto ratings{calculateGenreRatings(books)};
    EXPECT_EQ(ratings.size(), 3);
    EXPECT_NEAR(ratings[Genre::Fiction], 4.0, 1e-10);
    EXPECT_NEAR(ratings[Genre::SciFi], 4.0, 1e-10);  // (3+5)/2
    EXPECT_NEAR(ratings[Genre::Mystery], 5.0, 1e-10);
}

TEST(CalculateGenreRatingsTest, EmptyBooks) {
    std::vector<Book> empty;
    auto ratings{bookdb::calculateGenreRatings(empty)};
    EXPECT_TRUE(ratings.empty());
}

TEST(CalculateGenreRatingsTest, ZeroBooksPerGenre) {
    std::vector<Book> books;
    auto ratings{calculateGenreRatings(books)};
    EXPECT_TRUE(ratings.empty());
}
