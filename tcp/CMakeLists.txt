
add_executable(tcp_client tcp_client.cpp)
target_link_libraries(tcp_client PUBLIC CONAN_PKG::boost)
target_link_libraries(tcp_client PRIVATE nlohmann_json::nlohmann_json)
