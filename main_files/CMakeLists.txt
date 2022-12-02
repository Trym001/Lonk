add_executable(main main.cpp)

target_link_libraries(main PUBLIC CONAN_PKG::boost)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

#target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/include")


