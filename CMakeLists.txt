cmake_minimum_required(VERSION 3.20)
project(containers)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ -static-liblsan -static-libgcc") # для того чтобы можно было запускать приложение в Windows двойным
    #кликом и из командной строки без прописывания в PATH путей к бинарникам libstdc++
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "CLANG")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ -static-liblsan -static-libgcc") # для того чтобы можно было запускать приложение в Windows двойным
    #кликом и из командной строки без прописывания в PATH путей к бинарникам libstdc++
elseif(...)

endif()

set(CMAKE_CXX_STANDARD 17)

add_executable(containers
        Main.cpp
        Vector.h
        List.h
        Redirect.h
)

target_compile_options(containers PUBLIC
        -g
        -Wall
        -pedantic
)

set(WITH_TESTS ON CACHE BOOL "" FORCE)
if (WITH_TESTS)
    find_package(googletest)
    if(NOT googletest_FOUND)
        include(FetchContent)
        FetchContent_Declare(
                googletest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG        v1.14.0
        )
        FetchContent_MakeAvailable(googletest)
    endif()

    # Test
    set(test_DIR ${CMAKE_CURRENT_LIST_DIR}/tests/)
    add_executable(containers_tests
            ${test_DIR}/Vector_t.cpp
            ${test_DIR}/List_t.cpp
            ${test_DIR}/Main_t.cpp
            ${test_DIR}/Redirect_t.cpp)

    # Link test target against gtest libraries
    target_link_libraries(containers_tests PRIVATE
            gtest
            gmock
            gtest_main
            gmock_main
    )
    target_compile_options(containers_tests PUBLIC
            -g
            -Wall
            -pedantic
)
endif ()