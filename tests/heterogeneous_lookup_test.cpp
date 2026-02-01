#include <gtest/gtest.h>
#include <heterogeneous_lookup.hpp>
#include <unordered_set>

using namespace bookdb;
using namespace std::literals;

TEST(HeterogeneousLookupTest, TransparentStringLess) {
    TransparentStringLess less;

    std::string a = "apple";
    std::string b = "banana";
    std::string_view av = "apple";
    std::string_view bv = "banana";

    EXPECT_EQ(less(a, b), 1);
    EXPECT_EQ(less(b, a), 0);
    EXPECT_EQ(less(a, bv), 1);
    EXPECT_EQ(less(av, b), 1);
    EXPECT_EQ(less(av, bv), 1);
    EXPECT_EQ(less(a, a), 0);
    EXPECT_EQ(less(av, av), 0);
}

TEST(HeterogeneousLookupTest, TransparentStringEqual) {
    TransparentStringEqual eq;

    std::string a = "hello";
    std::string b = "world";
    std::string_view av = "hello";
    std::string_view bv = "world";

    EXPECT_EQ(eq(a, a), 1);
    EXPECT_EQ(eq(a, av), 1);
    EXPECT_EQ(eq(av, a), 1);
    EXPECT_EQ(eq(a, b), 0);
    EXPECT_EQ(eq(av, bv), 0);
    EXPECT_EQ(eq(av, "hello"sv), 1);
    EXPECT_EQ(eq(av, "hello"), 1);
}

TEST(HeterogeneousLookupTest, test_TransparentStringHash) {
    TransparentStringHash hash;

    std::string str = "test";
    std::string_view sv = "test";

    size_t h1 = hash(str);
    size_t h2 = hash(sv);

    EXPECT_EQ(h1, h2);

    std::string other = "other";
    size_t h3 = hash(other);

    EXPECT_NE(h1, h3);
}

TEST(HeterogeneousLookupTest, Set) {
    std::set<std::string, TransparentStringLess> my_set;
    my_set.insert("apple");
    my_set.insert("banana");
    my_set.insert("cherry");

    std::string_view key = "banana";

    auto it = my_set.find(key);
    EXPECT_EQ(*it, key);

    key = "grape";
    it = my_set.find(key);
    EXPECT_EQ(it, my_set.end());
}

TEST(HeterogeneousLookupTest, UnorderedSet) {
    std::unordered_set<std::string, TransparentStringHash, TransparentStringEqual> my_set;

    my_set.insert("hello");
    my_set.insert("world");

    std::string_view key = "hello";

    auto it = my_set.find(key);
    EXPECT_EQ(*it, key);

    key = "not_present";
    it = my_set.find(key);
    EXPECT_EQ(it, my_set.end());
}