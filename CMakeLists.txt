cmake_minimum_required(VERSION 3.15)
project(Lonk)

option(WITH_ALL "Build all demos (overrides individual settings)" OFF)

set(CMAKE_CXX_STANDARD 17)
set(generatedSourcesDir ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY "${generatedSourcesDir}")

if (MSVC)
    # Boost asio wants this...
    add_compile_definitions("_WIN32_WINNT=0x0601")
endif ()

include(cmake/conan.cmake)

set(DEPS boost/1.71.0)

conan_cmake_run(
        REQUIRES
        ${DEPS}
        GENERATORS cmake
        BUILD missing
        BASIC_SETUP CMAKE_TARGETS
)

Include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

add_subdirectory(src)

add_subdirectory(main_files)





