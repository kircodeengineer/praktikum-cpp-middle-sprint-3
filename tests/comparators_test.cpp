#include <book.hpp>
#include <comparators.hpp>
#include <gtest/gtest.h>
#include <vector>

using namespace bookdb;

class ComparatorsTest : public ::testing::Test {
    void SetUp() override {
        db.emplace_back("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
        db.emplace_back("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
        db.emplace_back("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
        db.emplace_back("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
        db.emplace_back("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
        db.emplace_back("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
        db.emplace_back("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
        db.emplace_back("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
        db.emplace_back("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
        db.emplace_back("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);

        etalon_less_by_year.emplace_back("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
        etalon_less_by_year.emplace_back("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
        etalon_less_by_year.emplace_back("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
        etalon_less_by_year.emplace_back("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
        etalon_less_by_year.emplace_back("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
        etalon_less_by_year.emplace_back("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
        etalon_less_by_year.emplace_back("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
        etalon_less_by_year.emplace_back("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
        etalon_less_by_year.emplace_back("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
        etalon_less_by_year.emplace_back("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);

        etalon_less_by_genre.emplace_back("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
        etalon_less_by_genre.emplace_back("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
        etalon_less_by_genre.emplace_back("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
        etalon_less_by_genre.emplace_back("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
        etalon_less_by_genre.emplace_back("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
        etalon_less_by_genre.emplace_back("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
        etalon_less_by_genre.emplace_back("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
        etalon_less_by_genre.emplace_back("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
        etalon_less_by_genre.emplace_back("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
        etalon_less_by_genre.emplace_back("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);

        etalon_less_by_author.emplace_back("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
        etalon_less_by_author.emplace_back("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
        etalon_less_by_author.emplace_back("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
        etalon_less_by_author.emplace_back("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
        etalon_less_by_author.emplace_back("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
        etalon_less_by_author.emplace_back("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
        etalon_less_by_author.emplace_back("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
        etalon_less_by_author.emplace_back("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
        etalon_less_by_author.emplace_back("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
        etalon_less_by_author.emplace_back("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);

        etalon_less_by_rating.emplace_back("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
        etalon_less_by_rating.emplace_back("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
        etalon_less_by_rating.emplace_back("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
        etalon_less_by_rating.emplace_back("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
        etalon_less_by_rating.emplace_back("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
        etalon_less_by_rating.emplace_back("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
        etalon_less_by_rating.emplace_back("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
        etalon_less_by_rating.emplace_back("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
        etalon_less_by_rating.emplace_back("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
        etalon_less_by_rating.emplace_back("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);

        etalon_less_by_read_count.emplace_back("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
        etalon_less_by_read_count.emplace_back("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
        etalon_less_by_read_count.emplace_back("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
        etalon_less_by_read_count.emplace_back("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3,
                                               112);
        etalon_less_by_read_count.emplace_back("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5,
                                               120);
        etalon_less_by_read_count.emplace_back("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
        etalon_less_by_read_count.emplace_back("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
        etalon_less_by_read_count.emplace_back("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
        etalon_less_by_read_count.emplace_back("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
        etalon_less_by_read_count.emplace_back("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);

        etalon_less_by_title.emplace_back("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
        etalon_less_by_title.emplace_back("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
        etalon_less_by_title.emplace_back("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
        etalon_less_by_title.emplace_back("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
        etalon_less_by_title.emplace_back("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
        etalon_less_by_title.emplace_back("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
        etalon_less_by_title.emplace_back("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
        etalon_less_by_title.emplace_back("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
        etalon_less_by_title.emplace_back("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
        etalon_less_by_title.emplace_back("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
    }

protected:
    std::vector<Book> db;
    std::vector<Book> etalon_less_by_year;
    std::vector<Book> etalon_less_by_genre;
    std::vector<Book> etalon_less_by_author;
    std::vector<Book> etalon_less_by_rating;
    std::vector<Book> etalon_less_by_read_count;
    std::vector<Book> etalon_less_by_title;
};

TEST_F(ComparatorsTest, LessByYear) {
    std::sort(db.begin(), db.end(), comp::LessByYear{});
    EXPECT_EQ(db, etalon_less_by_year);
}

TEST_F(ComparatorsTest, LessByGenre) {
    std::sort(db.begin(), db.end(), comp::LessByGenre{});
    EXPECT_EQ(db, etalon_less_by_genre);
}

TEST_F(ComparatorsTest, LessByAuthor) {
    std::sort(db.begin(), db.end(), comp::LessByAuthor{});
    EXPECT_EQ(db, etalon_less_by_author);
}

TEST_F(ComparatorsTest, LessByReadCount) {
    std::sort(db.begin(), db.end(), comp::LessByReadCount{});
    EXPECT_EQ(db, etalon_less_by_read_count);
}

TEST_F(ComparatorsTest, LessByTitle) {
    std::sort(db.begin(), db.end(), comp::LessByTitle{});
    EXPECT_EQ(db, etalon_less_by_title);
}