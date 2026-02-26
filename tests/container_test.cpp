#include "container.hpp"

#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(Container, FindsMatchingElement) {
    Container<int> c;
    c.add(1);
    c.add(5);

    const auto found = c.find_if([](int x) { return x == 5; });
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->get(), 5);
}

TEST(Container, FiltersElements) {
    Container<int> c;
    c.add(1);
    c.add(5);
    c.add(9);

    const auto filtered = c.filter([](int x) { return x > 3; });
    ASSERT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered[0], 5);
    EXPECT_EQ(filtered[1], 9);
}

TEST(Container, SquaresWorkForNumericTypes) {
    constexpr auto i = square(4);
    constexpr auto d = square(2.5);
    EXPECT_EQ(i, 16);
    EXPECT_DOUBLE_EQ(d, 6.25);
}

TEST(Container, TransformProducesExpectedTypes) {
    Container<std::string> c;
    c.add("hello");
    c.add("world");

    const auto sizes = c.transform([](const std::string& s) { return s.size(); });
    static_assert(std::is_same_v<std::decay_t<decltype(sizes)>, std::vector<std::size_t>>);
    ASSERT_EQ(sizes.size(), 2);
    EXPECT_EQ(sizes[0], 5);
    EXPECT_EQ(sizes[1], 5);
}

TEST(Container, MeanCalculatesAverageForArithmetic) {
    Container<double> c;
    c.add(10.0);
    c.add(20.0);
    c.add(30.0);

    const auto avg = c.mean();
    ASSERT_TRUE(avg.has_value());
    EXPECT_DOUBLE_EQ(*avg, 20.0);
}
