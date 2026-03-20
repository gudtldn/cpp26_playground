#include "utils.hpp"

#include <meta>
#include <print>

constexpr u64 DefaultFNVHash = 0xcbf29ce484222325ULL;
constexpr u64 FNV_Prime = 0x100000001b3ULL;

constexpr u64 fnv_hash(std::string_view view) noexcept
{
    u64 hash = DefaultFNVHash; // FNV_offset_basis
    for (const char c : view)
    {
        hash ^= static_cast<u8>(c);
        hash *= FNV_Prime;
    }
    return hash;
}

class TypeId
{
public:
    constexpr TypeId() = default;

    template <typename T>
    [[nodiscard]] constexpr static TypeId Of()
    {
        constexpr auto info = ^^std::remove_cvref_t<T>;
        constexpr auto display_name = std::meta::display_string_of(std::meta::dealias(info));
        return TypeId{ fnv_hash(display_name) };
    }

    [[nodiscard]] constexpr u64 Hash() const { return hash; }

    [[nodiscard]] constexpr bool IsValid() const { return hash != 0; }

    [[nodiscard]] constexpr bool operator==(const TypeId& other) const { return hash == other.hash; }
    [[nodiscard]] constexpr auto operator<=>(const TypeId& other) const { return hash <=> other.hash; }
    [[nodiscard]] explicit constexpr operator bool() const { return IsValid(); }

private:
    explicit constexpr TypeId(u64 hash)
        : hash(hash)
    {
    }

    u64 hash = 0;
};

struct PropertyMetadata
{
    std::string_view display_name;
    std::string_view category;
    std::string_view tooltip;

    float range_min = 0.0f;
    float range_max = 0.0f;

    double clamp_min = 0.0;
    double clamp_max = 0.0;
};

struct PropertyAccessor
{
    using PtrFunc    = void*(*)(void* instance);
    using GetterFunc = void(*)(const void* instance, void* out_value);
    using SetterFunc = void(*)(void* instance, const void* in_value);

public:
    PtrFunc get_ptr = nullptr;
    GetterFunc getter = nullptr;
    SetterFunc setter = nullptr;
};

struct PropertyInfo
{
    TypeId type_id;
    std::string_view name;

    usize size;
    usize offset;

public:
    PropertyMetadata metadata;
    PropertyAccessor accessor;
};

struct TypeInfo
{
    using ConstructorFunc = void*(*)();
    using DestructorFunc  = void(*)(void*);

public:
    TypeId type_id;
    std::string_view name;

    usize size;
    usize alignment;

    std::vector<PropertyInfo> properties;

public:
    ConstructorFunc constructor = nullptr;
    DestructorFunc destructor = nullptr;
};

template <typename T>
TypeInfo get_type_info()
{
    constexpr auto meta_info = ^^std::remove_cvref_t<T>;
    TypeInfo type_info = {
        .type_id = TypeId::Of<T>(),
        .name = std::meta::display_string_of(std::meta::dealias(meta_info)),
        .size = std::meta::size_of(meta_info),
        .alignment = std::meta::alignment_of(meta_info),
        .properties = {},
    };

    constexpr auto access_ctx = std::meta::access_context::unchecked();
    template for (constexpr auto member_info : std::define_static_array(std::meta::nonstatic_data_members_of(meta_info, access_ctx)))
    {
        using MemberType = [:std::meta::type_of(member_info):];
        static constexpr auto member_ptr = &[:member_info:];
        type_info.properties.push_back(PropertyInfo{
            .type_id = TypeId::Of<MemberType>(),
            .name = std::meta::identifier_of(member_info),
            .size = std::meta::size_of(member_info),
            .offset = std::meta::offset_of(member_info).bytes,
            .metadata = {},
            .accessor = {
                .get_ptr = [](void* instance) static -> void*
                {
                    T* typed = static_cast<T*>(instance);
                    return &(typed->*member_ptr);
                },
                .getter = [](const void* instance, void* out_value) static
                {
                    const T* typed = static_cast<const T*>(instance);
                    MemberType* out = static_cast<MemberType*>(out_value);
                    *out = typed->*member_ptr;
                },
                .setter = [](void* instance, const void* in_value) static
                {
                    T* typed = static_cast<T*>(instance);
                    const MemberType* in = static_cast<const MemberType*>(in_value);
                    typed->*member_ptr = *in;
                },
            },
        });
    }

    if constexpr (std::is_default_constructible_v<T>)
    {
        type_info.constructor = []() static -> void* { return new T(); };
    }
    type_info.destructor = [](void* ptr) static { delete static_cast<T*>(ptr); };

    return type_info;
}

class MyClass
{
public:
    int a;
    int b;
};

int main()
{
    TypeInfo info = get_type_info<MyClass>();

    MyClass* instance = static_cast<MyClass*>(info.constructor());
    instance->a = 1;
    instance->b = 2;

    for (const auto& property : info.properties)
    {
        int value = 99999;
        property.accessor.getter(instance, &value);
        std::println("{} = {}", property.name, value);
    }

    info.destructor(instance);

    // a = 1
    // b = 2
}
