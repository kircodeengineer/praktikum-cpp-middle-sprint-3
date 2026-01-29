#include <algorithm>
#include <list>

#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statsistics.hpp"
#include <deque>
#include <iostream>
#include <set>
#include <unordered_set>

using namespace bookdb;

void PrintBooks(const std::vector<Book> &books) {
    for (const auto &book : books)
        std::println("{}", book);
};

void test_TransparentStringLess() {
    TransparentStringLess less;

    std::string a = "apple";
    std::string b = "banana";
    std::string_view av = "apple";
    std::string_view bv = "banana";

    // Все возможные комбинации
    std::cout << "a < b: " << less(a, b) << std::endl;      // 1 (true)
    std::cout << "b < a: " << less(b, a) << std::endl;      // 0 (false)
    std::cout << "a < bv: " << less(a, bv) << std::endl;    // 1
    std::cout << "av < b: " << less(av, b) << std::endl;    // 1
    std::cout << "av < bv: " << less(av, bv) << std::endl;  // 1
    std::cout << "a < a: " << less(a, a) << std::endl;      // 0
    std::cout << "av < av: " << less(av, av) << std::endl;  // 0
}

void test_TransparentStringEqual() {
    TransparentStringEqual eq;

    std::string a = "hello";
    std::string b = "world";
    std::string_view av = "hello";
    std::string_view bv = "world";

    std::cout << "a == a: " << eq(a, a) << std::endl;                    // 1
    std::cout << "a == av: " << eq(a, av) << std::endl;                  // 1
    std::cout << "av == a: " << eq(av, a) << std::endl;                  // 1
    std::cout << "a == b: " << eq(a, b) << std::endl;                    // 0
    std::cout << "av == bv: " << eq(av, bv) << std::endl;                // 0
    std::cout << "av == \"hello\": " << eq(av, "hello"sv) << std::endl;  // 1
}

void test_TransparentStringHash() {
    TransparentStringHash hash;

    std::string str = "test";
    std::string_view sv = "test";

    size_t h1 = hash(str);
    size_t h2 = hash(sv);

    std::cout << "Hash of string: " << h1 << std::endl;
    std::cout << "Hash of string_view: " << h2 << std::endl;
    std::cout << "Equal hashes: " << (h1 == h2) << std::endl;

    // Разные строки → разные хеши
    std::string other = "other";
    size_t h3 = hash(other);
    std::cout << "Hash of 'other': " << h3 << std::endl;
    std::cout << "h1 != h3: " << (h1 != h3) << std::endl;
}

void test_set_heterogeneous_lookup() {
    std::set<std::string, TransparentStringLess> my_set;
    my_set.insert("apple");
    my_set.insert("banana");
    my_set.insert("cherry");

    std::string_view key = "banana";

    auto it = my_set.find(key);
    if (it != my_set.end()) {
        std::cout << "Found: " << *it << std::endl;  // "banana"
    } else {
        std::cout << "Not found!" << std::endl;
    }

    key = "grape";
    it = my_set.find(key);
    if (it == my_set.end()) {
        std::cout << "\"grape\" not found." << std::endl;
    }
}

void test_unordered_set_heterogeneous_lookup() {
    std::unordered_set<std::string, TransparentStringHash, TransparentStringEqual> my_set;

    my_set.insert("hello");
    my_set.insert("world");

    std::string_view key = "hello";

    auto it = my_set.find(key);
    if (it != my_set.end()) {
        std::cout << "Found: " << *it << std::endl;  // "hello"
    } else {
        std::cout << "Not found!" << std::endl;
    }

    key = "not_present";
    it = my_set.find(key);
    if (it == my_set.end()) {
        std::cout << "\"" << key << "\" not found." << std::endl;
    }
}

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
    // 5. Недопустимый тип (ошибка компиляции)
    // Book b5{"A", "T", 2023, 42, 0.0, 0};  // int не удовлетворяет ConvertibleToGenre

    // Проверка форматтера
    std::println("{}", test_book_1);
    std::println("{}", bookdb::Genre::Biography);

    std::vector<Book> test_db;
    test_db.emplace_back("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    test_db.emplace_back("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    test_db.emplace_back("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    test_db.emplace_back("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
    test_db.emplace_back("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    test_db.emplace_back("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
    test_db.emplace_back("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
    test_db.emplace_back("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
    test_db.emplace_back("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
    test_db.emplace_back("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);

    std::println();
    std::sort(test_db.begin(), test_db.end(), comp::LessByYear{});
    std::println("Сортировка по году");
    PrintBooks(test_db);
    std::println();

    std::sort(test_db.begin(), test_db.end(), comp::LessByGenre{});
    std::println("Сортировка по жанру");
    PrintBooks(test_db);
    std::println();

    std::sort(test_db.begin(), test_db.end(), comp::LessByAuthor{});
    std::println("Сортировка по автору");
    PrintBooks(test_db);
    std::println();

    std::sort(test_db.begin(), test_db.end(), comp::LessByRating{});
    std::println("Сортировка по рейтингу");
    PrintBooks(test_db);
    std::println();

    std::sort(test_db.begin(), test_db.end(), comp::LessByReadCount{});
    std::println("Сортировка по числу прочтений");
    PrintBooks(test_db);
    std::println();

    std::sort(test_db.begin(), test_db.end(), comp::LessByTitle{});
    std::println("Сортировка по названию");
    PrintBooks(test_db);
    std::println();

    // BookContainerLike
    // позитив
    static_assert(bookdb::BookContainerLike<std::vector<bookdb::Book>>,
                  "vector<Book> должен удовлетворять BookContainerLike");
    // позитив
    static_assert(bookdb::BookContainerLike<std::list<bookdb::Book>>,
                  "list<Book> должен удовлетворять BookContainerLike");
    // негатив
    static_assert(!bookdb::BookContainerLike<std::vector<int>>,
                  "vector<int> не должен удовлетворять BookContainerLike — value_type != Book");
    // негатив
    static_assert(!bookdb::BookContainerLike<int>, "int не контейнер — нет begin(), end(), size()");

    // BookIterator
    // позитив
    static_assert(bookdb::BookIterator<decltype(test_db.begin())>, "итератор vector<Book> должен быть BookIterator");
    // позитив
    static_assert(bookdb::BookIterator<decltype(test_db.cbegin())>,
                  "const_iterator vector<Book> должен быть BookIterator");
    // негатив
    std::vector<int> ints = {1};
    static_assert(!bookdb::BookIterator<decltype(ints.begin())>,
                  "итератор vector<int> не BookIterator — value_type != Book");
    // негатив
    static_assert(!bookdb::BookIterator<int *>, "int* не удовлетворяет BookIterator — не input_iterator для Book");

    // BookSentinel
    auto it{test_db.begin()};
    auto sentinel{test_db.end()};
    // позитив
    static_assert(bookdb::BookSentinel<decltype(sentinel), decltype(it)>,
                  "end() vector<Book> должен быть сентинелом для begin()");
    // негатив
    static_assert(!bookdb::BookSentinel<decltype(ints.end()), decltype(it)>,
                  "сентинел vector<int> не подходит для итератора vector<Book>");
    // негатив
    static_assert(!bookdb::BookSentinel<int, decltype(it)>, "int не может быть сентинелом для BookIterator");

    // BookPredicate
    // позитив
    auto is_recent = [](const bookdb::Book &b) { return b.year >= 2000; };
    static_assert(bookdb::BookPredicate<decltype(is_recent)>, "лямбда (const Book&) -> bool должна быть BookPredicate");

    // позитив
    struct IsFiction {
        bool operator()(const bookdb::Book &b) const { return b.genre == bookdb::Genre::Fiction; }
    };
    static_assert(bookdb::BookPredicate<IsFiction>, "функтор с operator()(const Book&) должен быть BookPredicate");

    // негатив
    auto returns_string = [](const bookdb::Book &b) { return "lol"s; };
    static_assert(!bookdb::BookPredicate<decltype(returns_string)>,
                  "предикат, возвращающий std::string, не удовлетворяет BookPredicate — не bool");

    // негатив
    auto takes_int = [](int x) { return x > 0; };
    static_assert(!bookdb::BookPredicate<decltype(takes_int)>,
                  "предикат, принимающий int, не удовлетворяет BookPredicate");

    // BookComparator
    // позитив
    static_assert(bookdb::BookComparator<decltype(comp::LessByYear())>,
                  "компоратор comp::LessByYear() -> bool должна быть BookComparator");

    // позитив
    static_assert(bookdb::BookComparator<comp::LessByTitle>, "компоратор comp::LessByTitle должен быть BookComparator");

    // негатив
    auto unary = [](const bookdb::Book &a) { return true; };
    static_assert(!bookdb::BookComparator<decltype(unary)>,
                  "унарный предикат не удовлетворяет BookComparator — нужно два аргумента");

    // негатив
    auto returns_void = [](const bookdb::Book &a, const bookdb::Book &b) {};
    static_assert(!bookdb::BookComparator<decltype(returns_void)>,
                  "компаратор, возвращающий void, не удовлетворяет BookComparator");

    // негатив
    auto compares_int = [](int a, int b) { return a < b; };
    static_assert(!bookdb::BookComparator<decltype(compares_int)>,
                  "компаратор для int не удовлетворяет BookComparator для Book");

    std::println();
    test_TransparentStringLess();
    std::println();
    test_TransparentStringEqual();
    std::println();
    test_TransparentStringHash();
    std::println();
    test_set_heterogeneous_lookup();
    std::println();
    test_unordered_set_heterogeneous_lookup();
    std::println();

    BookDatabase<std::vector<Book>> vect_db;
    BookDatabase<std::deque<Book>> deque_db;

    std::sort(vect_db.begin(), vect_db.end(), comp::LessByAuthor{});

    for (const auto &book : vect_db) {
        // Только чтение
    }

    std::println("is vect_db empty {}", vect_db.empty());
    // Create a book database
    BookDatabase<std::vector<Book>> db;

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
    std::println();
    std::println("is vect_db empty {}", db.empty());

    std::println();
    std::println("Books: {}\n\n", db);

    // Sorts
    std::sort(db.begin(), db.end(), comp::LessByAuthor{});
    std::println("Books sorted by author: {}\n\n==================\n", db);

    std::sort(db.begin(), db.end(), comp::LessByRating{});
    std::println("Books sorted by popularity: {}\n\n==================\n", db);

    BookDatabase<std::vector<Book>> init_list_db{
        Book{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
        Book{"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
        Book{"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120}};

    // Author histogram
    auto histogram{buildAuthorHistogramFlat(db)};
    std::println("Гистограмма");
    for (const auto &[author, count] : histogram)
        std::println("Автор: {} Книг: {}", author, count);

    // Ratings
    std::println();
    auto genre_ratings{calculateGenreRatings(db.begin(), db.end())};
    for (const auto &[genre, rating] : genre_ratings)
        std::println("Жанр: {} Рейтинг: {}", genre, rating);

    std::println();
    auto avr_rating{calculateAverageRating(db)};
    std::println("Средний рейтинг: {}\n", avr_rating);

    std::println();
    std::println("Книги в годах 1900, 1940");
    auto pred_year{filters::YearBetween(1900, 1940)};
    std::vector<Book> filtered;
    std::copy_if(db.begin(), db.end(), std::back_inserter(filtered), pred_year);
    for (const auto &book : filtered)
        std::println("{}", book);

    std::println();
    std::println("Книги с рейтингом выше 4.5");
    auto pred_raiting{filters::RatingAbove(4.5)};
    filtered.clear();
    std::copy_if(db.begin(), db.end(), std::back_inserter(filtered), pred_raiting);
    for (const auto &book : filtered)
        std::println("{}", book);

    std::println();
    std::println("Книги в жанре SciFi");
    auto pred_genre{filters::GenreIs(Genre::SciFi)};
    filtered.clear();
    std::copy_if(db.begin(), db.end(), std::back_inserter(filtered), pred_genre);
    for (const auto &book : filtered)
        std::println("{}", book);

    // Создаём сложные фильтры
    auto complex_filter{filters::all_of(pred_year, pred_raiting, pred_genre)};

    auto filtered_complex{filterBooks(db.begin(), db.end(), complex_filter)};
    std::println();
    std::println("Книги в годах 1900, 1940, с рейтингом выше 4.5, в жанре SciFi");
    for (const auto &book : filtered_complex)
        std::println("{}", static_cast<Book>(book));

    auto complex_filter_any{filters::any_of(pred_year, pred_raiting, pred_genre)};

    auto filtered_complex_any{filterBooks(db.begin(), db.end(), complex_filter_any)};
    std::println();
    std::println("Книги или в годах 1900, 1940 или с рейтингом выше 4.5 или в жанре SciFi");
    for (const auto &book : filtered_complex_any)
        std::println("{}", static_cast<Book>(book));

    std::println();
    std::println("3 Рандомные книги");
    try {
        auto sample{sampleRandomBooks(db, 3)};

        for (const auto &book : sample) {
            std::println("{}", static_cast<Book>(book));
        }
    } catch (const std::exception &e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    // Filters
    /*auto filtered = filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
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