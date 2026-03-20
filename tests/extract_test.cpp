#include "utils.hpp"

#include <meta>
#include <print>


int main()
{
    constexpr u32 test_value = 42;
    u32 value = std::meta::extract<u32>(^^test_value);
    std::println("value = {}", value);


}
