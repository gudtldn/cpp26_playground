add_library(DefaultOptions INTERFACE)

# C++26 표준 설정 및 확장 기능 끄기
target_compile_features(DefaultOptions INTERFACE cxx_std_26)
set_target_properties(DefaultOptions PROPERTIES
        INTERFACE_CXX_EXTENSIONS OFF
        INTERFACE_CXX_STANDARD_REQUIRED ON
)

# 실험적 기능 플래그 추가
target_compile_options(DefaultOptions INTERFACE
        -freflection # 실험적 리플렉션 활성화

        -finput-charset=UTF-8
        -fexec-charset=UTF-8

        -Wall -Wextra
        -pedantic
)

# 실험적 라이브러리 링크
target_link_libraries(DefaultOptions INTERFACE
        stdc++exp
)
