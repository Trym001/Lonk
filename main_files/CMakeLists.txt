add_executable(main main.cpp)

target_link_libraries(main PUBLIC Boost::system)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(main PUBLIC cpp_libraries)

target_link_libraries(main PRIVATE opencv::opencv)
if (UNIX)
    target_link_libraries(main PRIVATE pthread)
endif()

#######################

add_executable(wasd wasd_controller.cpp)
target_link_libraries(wasd PUBLIC Boost::system)
target_link_libraries(wasd PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(wasd PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(wasd PUBLIC cpp_libraries)

