#include <meta>
#include <print>
#include <format>

consteval bool has_annotation(std::meta::info info, std::meta::info anno_type)
{
    return !std::meta::annotations_of_with_type(info, anno_type).empty();
}

namespace detail
{
struct Debug{};
}

constexpr detail::Debug Debug;

struct [[=Debug]] MyStruct
{
    int a;
    int b;
    std::string asdasd;

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

            constexpr auto access_ctx = std::meta::access_context::unchecked();
            template for (constexpr auto member : std::define_static_array(std::meta::nonstatic_data_members_of(info, access_ctx)))
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
        .asdasd = "Hello, World!"
    };
    std::println("{}", abc);
    std::println("{:?}", abc);

    // MyStruct(a=1, b=2)
    // MyStruct {
    //     MyStruct::a: 1,
    //     MyStruct::b: 2,
    // }
}
