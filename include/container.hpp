#pragma once

#include <concepts>
#include <functional>
#include <numeric>
#include <optional>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;

template <typename T>
class Container final {
public:
    using value_type = T;

    template <std::convertible_to<T> U>
    void add(U&& val) {
        items_.push_back(std::forward<U>(val));
    }

    template <typename... Args>
    T& emplace(Args&&... args) {
        return items_.emplace_back(std::forward<Args>(args)...);
    }

    [[nodiscard]] std::size_t size() const noexcept { return items_.size(); }
    [[nodiscard]] bool empty() const noexcept { return items_.empty(); }

    [[nodiscard]] const std::vector<T>& data() const noexcept { return items_; }

    template <typename Pred>
    [[nodiscard]] std::optional<std::reference_wrapper<const T>> find_if(Pred&& pred) const {
        if (auto it = std::ranges::find_if(items_, std::forward<Pred>(pred)); it != items_.end()) {
            return std::cref(*it);
        }
        return std::nullopt;
    }

    template <typename Pred>
    [[nodiscard]] std::vector<T> filter(Pred&& pred) const {
        std::vector<T> out;
        out.reserve(items_.size());
        for (const auto& item : items_) {
            if (pred(item)) {
                out.push_back(item);
            }
        }
        return out;
    }

    template <typename Func>
    [[nodiscard]] auto transform(Func&& fn) const
        -> std::vector<std::invoke_result_t<Func, const T&>> {
        std::vector<std::invoke_result_t<Func, const T&>> out;
        out.reserve(items_.size());
        for (const auto& item : items_) {
            out.push_back(std::invoke(fn, item));
        }
        return out;
    }

    template <Arithmetic U = T>
    [[nodiscard]] std::optional<double> mean() const requires Arithmetic<T> {
        if (items_.empty()) {
            return std::nullopt;
        }
        const auto sum = std::accumulate(items_.begin(), items_.end(), 0.0,
                                         [](double acc, const T& v) { return acc + v; });
        return sum / static_cast<double>(items_.size());
    }

private:
    std::vector<T> items_;
};

template <Arithmetic T>
constexpr T square(T x) {
    return x * x;
}
