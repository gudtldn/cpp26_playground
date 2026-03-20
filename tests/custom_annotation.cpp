#include <meta>
#include <print>
#include <format>

consteval bool has_annotation(std::meta::info info, std::meta::info anno_type)
{
    return !std::meta::annotations_of_with_type(info, anno_type).empty();
}

template <std::same_as<std::meta::info> auto TypeInfo, std::same_as<std::meta::access_context> auto Ctx>
    requires (std::meta::is_class_type(TypeInfo))
consteval auto nonstatic_data_members_of_array()
{
    constexpr std::size_t N = std::meta::nonstatic_data_members_of(TypeInfo, Ctx).size();
    std::array<std::meta::info, N> arr{};

    auto vec = std::meta::nonstatic_data_members_of(TypeInfo, Ctx);
    for (std::size_t i = 0; i < N; ++i)
    {
        arr[i] = vec[i];
    }

    return arr;
}

namespace detail
{
struct Debug{};
}

constexpr detail::Debug Debug;

struct [[=Debug]] MyStruct
// struct MyStruct
{
    int a;
    int b;

    [[nodiscard]] std::string custom_format() const
    {
        return std::format("MyStruct(a={}, b={})", a, b);
    }
};

template <typename T>
    requires (has_annotation(^^T, ^^detail::Debug))
struct std::formatter<T>
{
    bool debug_mode = false;

    constexpr auto parse(std::format_parse_context& ctx)
    {
        auto it = ctx.begin();
        if (it != ctx.end() && *it == '?')
        {
            // {:?} 감지
            debug_mode = true;
            ++it;
        }

        if (it != ctx.end() && *it != '}')
        {
            throw std::format_error("Invalid format specifier for annotated struct");
        }
        return it;
    }

    auto format(const T& value, std::format_context& ctx) const
    {
        if (debug_mode)
        {
            constexpr auto info = ^^T;
            std::format_to(ctx.out(), "{} {{\n", std::meta::display_string_of(info));
            template for (constexpr auto member : nonstatic_data_members_of_array<info, std::meta::access_context::current()>())
            {
                std::format_to(ctx.out(), "    {}: {},\n", std::meta::display_string_of(member), value.[:member:]);
            }
            return std::format_to(ctx.out(), "}}");
        }

        if constexpr (requires { value.custom_format(); })
        {
            return std::format_to(ctx.out(), "{}", value.custom_format());
        }
        else
        {
            throw std::format_error("No custom format function");
        }
    }
};

int main()
{
    MyStruct abc = {
        .a = 1,
        .b = 2,
    };
    std::println("{}", abc);
    std::println("{:?}", abc);

    // MyStruct(a=1, b=2)
    // MyStruct {
    //     MyStruct::a: 1,
    //     MyStruct::b: 2,
    // }
}
