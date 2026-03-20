# 테스트 실행 파일을 쉽게 추가하기 위한 헬퍼 함수
function(add_cpp26_test TARGET_NAME SOURCE_FILE)
    add_executable(${TARGET_NAME} ${SOURCE_FILE})
    target_link_libraries(${TARGET_NAME} PRIVATE core_lib)
endfunction()
