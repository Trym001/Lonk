add_executable(main main.cpp)

target_link_libraries(main PUBLIC CONAN_PKG::boost)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(main PUBLIC cont Catch2::Catch2)

target_link_libraries(main PUBLIC system_timer Catch2::Catch2)






add_executable(test test.cpp)

target_link_libraries(test PUBLIC cont Catch2::Catch2)

target_link_libraries(test PUBLIC system_timer Catch2::Catch2)



#target_include_directories(test PUBLIC "${CMAKE_SOURCE_DIR}/include")



