#include <book.hpp>
#include <book_database.hpp>
#include <gtest/gtest.h>

TEST(BookDatabaseTest, DefaultConstructor) {
    bookdb::BookDatabase db;

    EXPECT_EQ(db.size(), 0);
    EXPECT_TRUE(db.empty());
    EXPECT_EQ(std::distance(db.begin(), db.end()), 0);
}

TEST(BookDatabaseTest, PushBackAddsBooks) {
    bookdb::BookDatabase db;
    bookdb::Book book1{"1984", "George Orwell", 1949, bookdb::Genre::SciFi, 4.0, 190};
    bookdb::Book book2{"Animal Farm", "George Orwell", 1945, bookdb::Genre::Fiction, 4.4, 143};

    db.PushBack(book1);
    EXPECT_EQ(db.size(), 1);
    EXPECT_FALSE(db.empty());

    db.PushBack(book2);
    EXPECT_EQ(db.size(), 2);

    auto authors_begin = db.begin_authors();
    auto authors_end = db.end_authors();
    EXPECT_EQ(std::distance(authors_begin, authors_end), 1);
}

TEST(BookDatabaseTest, InitializerListConstructor) {
    bookdb::BookDatabase db{
        bookdb::Book{"1984", "George Orwell", 1949, bookdb::Genre::SciFi, 4.0, 190},
        bookdb::Book{"Animal Farm", "George Orwell", 1945, bookdb::Genre::Fiction, 4.4, 143},
        bookdb::Book{"The Great Gatsby", "F. Scott Fitzgerald", 1925, bookdb::Genre::Fiction, 4.5, 120}};

    EXPECT_EQ(db.size(), 3);

    EXPECT_EQ(std::distance(db.begin_authors(), db.end_authors()), 2);

    auto it{db.begin()};
    EXPECT_EQ(it->title, "1984");
    EXPECT_EQ(it->author, "George Orwell");
    EXPECT_EQ(it->year, 1949);
}
