add_executable(tcp_client main.cpp)

target_link_libraries(tcp_client PUBLIC CONAN_PKG::boost)
target_link_libraries(tcp_client PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(tcp_client PUBLIC "${CMAKE_SOURCE_DIR}/include")