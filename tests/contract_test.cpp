#include "utils.hpp"

#include <print>
#include <contracts>
#include <debugging>


// 핸들러 선언
void handle_contract_violation(const std::contracts::contract_violation& violation)
{
    std::println(stderr,
        "Contract Violation Error!\nLocation: {}:{}\nCondition: {}",
        violation.location().file_name(),
        violation.location().line(),
        violation.comment()
    );
    std::breakpoint_if_debugging();
    // std::terminate();
}

// post에서 검사하는 변수는 const임이 보장 되어야 함
i32 my_divide(const i32 a, i32 b)
    pre (b != 0)     // 사전 조건: b는 0이 아니어야 함
    post (r: r <= a) // 사후 조건: 결과값 r은 a보다 작거나 같아야 함
{
    auto my_func = [](int x) pre(x > 0) { return x; };
    return my_func(a) / b;
}

int main()
{
    // i32 result = my_divide(10, 0); // error!
    i32 result = my_divide(10, 2);
    contract_assert(result == 5);

    std::println("result = {}", result);
    return 0;
}
