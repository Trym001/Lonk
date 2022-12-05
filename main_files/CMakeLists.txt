add_executable(main main.cpp)

target_link_libraries(main PUBLIC Boost::system)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(main PUBLIC cont)

target_link_libraries(main PRIVATE opencv::opencv)
if (UNIX)
    target_link_libraries(main PRIVATE pthread)
endif()


add_executable(test test.cpp)

target_link_libraries(test PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(test PUBLIC cont)

add_executable(webcam_demo webcam.cpp)
target_link_libraries(webcam_demo PRIVATE opencv::opencv)
if (UNIX)
    target_link_libraries(webcam_demo PRIVATE pthread)
endif()



