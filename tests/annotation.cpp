#include "utils.hpp"

#include <meta>
#include <print>

template <typename... Ts>
struct derive_t{};

template <typename... Ts>
constexpr derive_t<Ts...> derive;

struct Debug{};
struct Copy{};
struct Clone{};

struct [[=derive<Debug, Copy, Clone>]] MyStruct
{
    int a;
    int b;
};

int main()
{
    constexpr auto info = ^^MyStruct;

    template for (constexpr auto anno_info : std::define_static_array(std::meta::annotations_of(info)))
    {
        constexpr auto anno_type = std::meta::type_of(anno_info);
        if constexpr (std::meta::template_of(anno_type) == ^^derive_t)
        {
            template for (constexpr auto arg_info : std::define_static_array(std::meta::template_arguments_of(anno_type)))
            {
                std::println("Annotation: {}", std::meta::display_string_of(arg_info));
                // Annotation: Debug
                // Annotation: Copy
                // Annotation: Clone
            }
        }
    }
}
