#pragma once

#include <print>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;
    using reverse_iterator = typename BookContainer::reverse_iterator;
    using const_reverse_iterator = typename BookContainer::const_reverse_iterator;

    using size_type = typename BookContainer::size_type;
    using difference_type = typename BookContainer::difference_type;

    using reference = typename BookContainer::reference;
    using const_reference = typename BookContainer::const_reference;

    using AuthorContainer = std::unordered_set<std::string>;
    using AuthorIterator = typename AuthorContainer::iterator;
    using AuthorConstIterator = typename AuthorContainer::const_iterator;

    BookDatabase() = default;

    explicit BookDatabase(std::initializer_list<Book> books) {
        for (const auto &book : books) {
            PushBack(book);
        }
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    void PushBack(const Book &book) {
        auto author_it{GetOrInsertAuthor(book.author)};
        books_.push_back(Book{book.title, *author_it, book.year, book.genre, book.rating, book.read_count});
    }

    template <typename... Args>
    void EmplaceBack(Args &&...args) {
        Book temp_book(std::forward<Args>(args)...);
        auto author_it{GetOrInsertAuthor(temp_book.author)};

        books_.emplace_back(temp_book.title, *author_it, temp_book.year, temp_book.genre, temp_book.rating,
                            temp_book.read_count);
    }

    const std::vector<Book> &GetBooks() const { return books_; }

    const std::unordered_set<std::string> &GetAuthors() const { return authors_; }

    iterator begin() { return books_.begin(); }
    iterator end() { return books_.end(); }
    const_iterator begin() const { return books_.begin(); }
    const_iterator end() const { return books_.end(); }
    const_iterator cbegin() const { return books_.cbegin(); }
    const_iterator cend() const { return books_.cend(); }
    reverse_iterator rbegin() { return books_.rbegin(); }
    reverse_iterator rend() { return books_.rend(); }
    const_reverse_iterator rbegin() const { return books_.rbegin(); }
    const_reverse_iterator rend() const { return books_.rend(); }
    const_reverse_iterator crbegin() const { return books_.crbegin(); }
    const_reverse_iterator crend() const { return books_.crend(); }

    AuthorIterator begin_authors() { return authors_.begin(); }
    AuthorIterator end_authors() { return authors_.end(); }
    AuthorConstIterator begin_authors() const { return authors_.begin(); }
    AuthorConstIterator end_authors() const { return authors_.end(); }

    size_type size() const { return books_.size(); }
    bool empty() const { return books_.empty(); }
    void resize(size_type new_size) {
        if (new_size >= books_.size()) {
            books_.resize(new_size);
            return;
        }

        std::unordered_set<std::string> removed_authors;
        for (size_type i = new_size; i < books_.size(); ++i)
            removed_authors.insert(books_[i].author);

        books_.resize(new_size);

        std::unordered_set<std::string> remaining_authors;
        for (const auto &book : books_)
            remaining_authors.insert(book.author);

        for (const auto &author : removed_authors)
            if (remaining_authors.find(author) == remaining_authors.end())
                authors_.erase(author);
    }
    void reserve(size_type new_capacity) { books_.reserve(new_capacity); }

    reference operator[](size_type pos) { return books_[pos]; }
    const_reference operator[](size_type pos) const { return books_[pos]; }
    reference at(size_type pos) { return books_.at(pos); }
    const_reference at(size_type pos) const { return books_.at(pos); }

private:
    BookContainer books_;
    AuthorContainer authors_;

private:
    AuthorIterator GetOrInsertAuthor(std::string_view author_name) {
        auto it{authors_.find(std::string(author_name))};
        if (it == authors_.end()) {
            it = authors_.insert(std::string(author_name)).first;
        }
        return it;
    }
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        // Раскомментируйте, когда bookdb::BookDatabase поддержит интерфейсы, доступные стандартным контейнерам
        // (size/begin/...)

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
