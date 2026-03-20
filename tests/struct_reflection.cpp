#include <meta>
#include <print>

struct MyStruct
{
    int a;
    int b;
};

class MyClass
{
public:
    int a;

protected:
    int b;

private:
    int c;
};

template <typename T>
void print_member_info()
{
    constexpr auto info = ^^T;
    std::println("Members of {}:", std::meta::display_string_of(info));

    constexpr auto ctx = std::meta::access_context::current();
    template for (constexpr auto member : std::define_static_array(std::meta::members_of(info, ctx)))
    {
        std::println("- {}", std::meta::display_string_of(member));
    }
}

int main()
{
    print_member_info<MyStruct>();
    // Members of MyStruct:
    // - MyStruct::a
    // - MyStruct::b
    // - constexpr MyStruct::MyStruct()
    // - constexpr MyStruct::MyStruct(const MyStruct&)
    // - constexpr MyStruct& MyStruct::operator=(const MyStruct&)
    // - constexpr MyStruct::MyStruct(MyStruct&&)
    // - constexpr MyStruct& MyStruct::operator=(MyStruct&&)
    // - constexpr MyStruct::~MyStruct()

    print_member_info<MyClass>();
    // Members of MyClass:
    // - MyClass::a
    // - constexpr MyClass::MyClass()
    // - constexpr MyClass::MyClass(const MyClass&)
    // - constexpr MyClass& MyClass::operator=(const MyClass&)
    // - constexpr MyClass::MyClass(MyClass&&)
    // - constexpr MyClass& MyClass::operator=(MyClass&&)
    // - constexpr MyClass::~MyClass()
}
