# 현재 폴더(tests/)에 있는 모든 .cpp 파일을 찾아서 TEST_SOURCES 변수에 저장
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

# 찾은 파일들을 순회하면서 자동으로 타겟을 생성
foreach(SRC_FILE ${TEST_SOURCES})
    # 파일 이름에서 확장자를 제거하여 타겟 이름으로 사용
    get_filename_component(TARGET_NAME ${SRC_FILE} NAME_WE)

    # 헬퍼 함수로 실행 파일 생성 및 코어 링크
    add_cpp26_test(${TARGET_NAME} ${SRC_FILE})
endforeach()
