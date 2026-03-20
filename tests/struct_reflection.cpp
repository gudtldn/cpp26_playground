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

/**
 * NTTP로 주어진 클래스/구조체 타입의 info에 대해 멤버 목록을 가져옵니다.
 * @tparam TypeInfo 멤버를 가져올 구조체/클래스 타입의 info
 * @tparam Ctx 멤버에 접근할 때 어디까지 보일지 결정하는 context
 * @return std::array<info, N>으로 이루어진 멤버의 목록
 */
template <std::same_as<std::meta::info> auto TypeInfo, std::same_as<std::meta::access_context> auto Ctx>
    requires (std::meta::is_class_type(TypeInfo))
consteval auto get_member_array()
{
    // 멤버의 개수를 가져옵니다. (이 과정에서 생긴 vector는 바로 파괴되므로 안전함)
    constexpr std::size_t N = std::meta::members_of(TypeInfo, Ctx).size();

    // 동적 할당이 없는 고정 길이 array를 생성합니다.
    std::array<std::meta::info, N> arr{};

    // 실제 멤버 정보들을 가져와서 array에 복사합니다.
    auto vec = std::meta::members_of(TypeInfo, Ctx);
    for (std::size_t i = 0; i < N; ++i)
    {
        arr[i] = vec[i];
    }

    // 함수가 끝나면서 vec는 파괴되고(메모리 해제 조건 충족), 안전한 arr만 반환됩니다.
    return arr;
}

template <typename T>
void print_member_info()
{
    constexpr auto info = ^^T;
    std::println("Members of {}:", std::meta::display_string_of(info));

    constexpr auto ctx = std::meta::access_context::current();
    // constexpr auto ctx = std::meta::access_context::unchecked();
    template for (constexpr auto member : get_member_array<info, ctx>())
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
