#include <concepts>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

template <typename... Fs>
struct overloaded : Fs... {
    using Fs::operator()...;
};
template <typename... Fs>
overloaded(Fs...) -> overloaded<Fs...>;

template <typename T>
concept ActionPayload = requires(const T& value) {
    { value.describe() } -> std::convertible_to<std::string>;
};

struct OpenBuffer {
    std::string name;
    auto describe() const -> std::string { return "open buffer '" + name + "'"; }
};

struct CloseBuffer {
    std::string name;
    auto describe() const -> std::string { return "close buffer '" + name + "'"; }
};

struct SetTheme {
    std::string theme;
    auto describe() const -> std::string { return "set theme to '" + theme + "'"; }
};

struct PutSetting {
    std::string key;
    std::string value;
    auto describe() const -> std::string { return "set " + key + "=" + value; }
};

struct Notify {
    std::string message;
    auto describe() const -> std::string { return "notify '" + message + "'"; }
};

using Action = std::variant<OpenBuffer, CloseBuffer, SetTheme, PutSetting, Notify>;

class WorkspaceState {
  public:
    auto run(std::span<const Action> actions) -> void {
        auto trace = []<ActionPayload T>(const T& payload) {
            std::cout << "action: " << payload.describe() << '\n';
        };

        for (const auto& action : actions) {
            std::visit(
                overloaded{
                    [&](const OpenBuffer& a) {
                        trace(a);
                        buffers_.push_back(a.name);
                    },
                    [&](const CloseBuffer& a) {
                        trace(a);
                        if (const auto iter = std::ranges::find(buffers_, a.name); iter != buffers_.end()) {
                            buffers_.erase(iter);
                        }
                    },
                    [&](const SetTheme& a) {
                        trace(a);
                        theme_ = a.theme;
                    },
                    [&](const PutSetting& a) {
                        trace(a);
                        settings_[a.key] = a.value;
                    },
                    [&](const Notify& a) {
                        trace(a);
                        notifications_.push_back(a.message);
                    }},
                action);
        }
    }

    auto find_setting(std::string_view key) const -> std::optional<std::reference_wrapper<const std::string>> {
        if (const auto iter = settings_.find(std::string(key)); iter != settings_.end()) {
            return std::cref(iter->second);
        }
        return std::nullopt;
    }

    auto print_summary() const -> void {
        std::cout << "\ncurrent theme: " << theme_ << '\n';
        std::cout << "visible buffers:";

        auto visible_buffers = buffers_ | std::views::filter([](const std::string& name) {
                                   return !name.starts_with(".");
                               });

        for (const auto& name : visible_buffers) {
            std::cout << ' ' << name;
        }
        std::cout << '\n';

        if (const auto autosave = find_setting("autosave")) {
            std::cout << "autosave: " << autosave->get() << '\n';
        }

        std::cout << "notifications:";
        for (const auto& item : notifications_) {
            std::cout << " [" << item << ']';
        }
        std::cout << '\n';
    }

  private:
    std::vector<std::string> buffers_;
    std::string theme_ = "paper";
    std::unordered_map<std::string, std::string> settings_;
    std::vector<std::string> notifications_;
};

auto main() -> int {
    const std::vector<Action> script{
        OpenBuffer{"notes.md"},
        OpenBuffer{"todo.txt"},
        SetTheme{"oxide"},
        PutSetting{"autosave", "on"},
        Notify{"workspace restored"},
        CloseBuffer{"todo.txt"},
        OpenBuffer{".cache"},
    };

    WorkspaceState state;
    state.run(script);
    state.print_summary();

    return 0;
}
