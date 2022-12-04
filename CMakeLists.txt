cmake_minimum_required(VERSION 3.15)
project(Lonk)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

set(generatedSourcesDir ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY "${generatedSourcesDir}")

include(cmake/conan.cmake)

conan_cmake_configure(
        REQUIRES
        opencv/4.5.5
        GENERATORS cmake_find_package
        IMPORTS "res, haarcascades* -> cascades"
        IMPORTS "res, lbpcascades* -> cascades"
)

conan_cmake_autodetect(settings)
#itte her
conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        SETTINGS ${settings})

find_package(opencv REQUIRED)

if (MSVC)
    # Boost asio wants this...
    add_compile_definitions("_WIN32_WINNT=0x0601")
endif ()



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





