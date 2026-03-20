#include "utils.hpp"

#include <meta>
#include <print>

// 참고할 만한 글
// https://brevzin.github.io/c++/2025/06/26/json-reflection/

// =======================================================================

struct MyStruct;

consteval {
    std::vector<std::meta::info> members;

    // 타입은 i32, 이름은 id로 설정
    members.push_back(std::meta::data_member_spec(^^i32, { .name = "id" }));

    // 타입은 f32, 이름은 hp로 설정
    members.push_back(std::meta::data_member_spec(^^f32, { .name = "hp" }));

    // 빈 껍데기(^^MyStruct)에 방금 만든 멤버들(members)을 조립
    std::meta::define_aggregate(^^MyStruct, members);
}

// =======================================================================

struct Transform
{
    float x;
    float y;
    float z;
};

struct MirroredTransform;

consteval {
    std::vector<std::meta::info> new_members;

    // 원본 구조체의 멤버들을 순회
    constexpr auto ctx = std::meta::access_context::unchecked();
    template for (constexpr auto member_info : std::define_static_array(std::meta::nonstatic_data_members_of(^^Transform, ctx)))
    {
        // 원본 이름(x, y, z)을 가져와서 앞에 "m_"을 붙입니다.
        std::string new_name = "m_" + std::string(std::meta::identifier_of(member_info));

        // 새로운 멤버 명세서를 만들어 벡터에 넣습니다.
        new_members.push_back(std::meta::data_member_spec(
            std::meta::type_of(member_info), // 타입은 원본과 동일하게 유지
            { .name = new_name }             // 이름만 새로 만든 이름으로 교체
        ));
    }

    // 준비된 new_members를 껍데기(^^MirroredTransform)에 조립
    std::meta::define_aggregate(^^MirroredTransform, new_members);
}

int main()
{
    MyStruct s = { .id = 1, .hp = 100 };
    std::println("s.id = {}, s.hp = {}", s.id, s.hp);

    MirroredTransform transform = {
        .m_x = 10.0f,
        .m_y = 20.0f,
        .m_z = 30.0f
    };

    std::println(
        "Mirrored Transform: x={}, y={}, z={}",
        transform.m_x, transform.m_y, transform.m_z
    );
}
