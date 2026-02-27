#include "container.hpp"

#include <string>
#include <vector>

namespace {

auto test_find_matching_element() -> bool {
    Container<int> c;
    c.add(1);
    c.add(5);
    const auto found = c.find_if([](int x) { return x == 5; });
    return found.has_value() && found->get() == 5;
}

auto test_filters_elements() -> bool {
    Container<int> c;
    c.add(1);
    c.add(5);
    c.add(9);
    const auto filtered = c.filter([](int x) { return x > 3; });
    return filtered.size() == 2 && filtered[0] == 5 && filtered[1] == 9;
}

auto test_squares_work_for_numeric_types() -> bool {
    constexpr auto i = square(4);
    constexpr auto d = square(2.5);
    return i == 16 && d == 6.25;
}

auto test_transform_produces_expected_types() -> bool {
    Container<std::string> c;
    c.add("hello");
    c.add("world");
    const auto sizes = c.transform([](const std::string& s) { return s.size(); });
    static_assert(std::is_same_v<std::decay_t<decltype(sizes)>, std::vector<std::size_t>>);
    return sizes.size() == 2 && sizes[0] == 5 && sizes[1] == 5;
}

auto test_mean_calculates_average_for_arithmetic() -> bool {
    Container<double> c;
    c.add(10.0);
    c.add(20.0);
    c.add(30.0);
    const auto avg = c.mean();
    return avg.has_value() && *avg == 20.0;
}

}  // namespace

auto main() -> int {
    if (!test_find_matching_element()) return 1;
    if (!test_filters_elements()) return 1;
    if (!test_squares_work_for_numeric_types()) return 1;
    if (!test_transform_produces_expected_types()) return 1;
    if (!test_mean_calculates_average_for_arithmetic()) return 1;
    return 0;
}
