#include "utils.hpp"

#include <meta>
#include <print>
#include <vector>
#include <ranges>


int main()
{
    constexpr auto vector_info = ^^std::vector;
    constexpr auto u32_info = ^^u32;

    constexpr auto u32_vector_info = std::meta::substitute(vector_info, { u32_info });
    typename[:u32_vector_info:] u32_vector = { 1, 2, 3 };

    for (const auto [n, value] : u32_vector | std::views::enumerate)
    {
        std::println("Idx {}. {}", n, value);
    }

    // Output:
    // Idx 0. 1
    // Idx 1. 2
    // Idx 2. 3
}
