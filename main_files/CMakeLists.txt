add_executable(main main.cpp)

target_link_libraries(main PUBLIC CONAN_PKG::boost)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(main PUBLIC cont)


add_executable(test test.cpp ../src/big_brain.cpp ../include/controller/big_brain.hpp)

target_link_libraries(test PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(test PUBLIC cont)
target_link_libraries(test PUBLIC CONAN_PKG::boost)




