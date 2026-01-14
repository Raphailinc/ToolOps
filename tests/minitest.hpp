#pragma once
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace minitest {
struct Case { std::string name; std::function<void()> fn; };
inline std::vector<Case>& registry() { static std::vector<Case> r; return r; }
struct Registrar { Registrar(const std::string& n, std::function<void()> f) { registry().push_back({n, f}); } };
inline void check(bool ok, const char* expr, const char* file, int line) {
    if (!ok) throw std::runtime_error(std::string(file) + ":" + std::to_string(line) + " CHECK failed: " + expr);
}
} // namespace minitest

#define TEST(name) \
    static void name(); \
    static minitest::Registrar reg_##name(#name, name); \
    static void name()

#define CHECK(expr) minitest::check((expr), #expr, __FILE__, __LINE__)

#ifdef MINITEST_IMPLEMENTATION
int main() {
    int failed = 0;
    for (auto& c : minitest::registry()) {
        try {
            c.fn();
        } catch (const std::exception& e) {
            ++failed;
            std::cerr << "[FAIL] " << c.name << " -> " << e.what() << "\n";
        }
    }
    if (failed) {
        std::cerr << failed << " test(s) failed\n";
        return 1;
    }
    std::cout << "All tests passed (" << minitest::registry().size() << ")\n";
    return 0;
}
#endif
