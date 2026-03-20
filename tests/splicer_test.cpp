#include <meta>
#include <print>

int main()
{
    auto a = 1;
    constexpr auto a_info = ^^decltype(a);

    typename[:a_info:] b = 2;
    std::println("a + b = {}", a + b);

    std::println(
        "a type: {}, b type: {}",
        std::meta::display_string_of(a_info),
        std::meta::display_string_of(^^decltype(b))
    );

    // a + b = 3
    // a type: int, b type: int
}
