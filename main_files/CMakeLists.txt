add_executable(main main.cpp)

target_link_libraries(main PUBLIC Boost::system)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(main PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(main PUBLIC cpp)

target_link_libraries(main PRIVATE opencv::opencv)
if (UNIX)
    target_link_libraries(main PRIVATE pthread)
endif()

#######################

add_executable(test test.cpp)

target_link_libraries(test PUBLIC Boost::system)
target_link_libraries(test PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(test PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(test PUBLIC cpp)

target_link_libraries(test PRIVATE opencv::opencv)
if (UNIX)
    target_link_libraries(test PRIVATE pthread)
endif()

#######################

add_executable(wasd wasd_controller.cpp)
target_link_libraries(wasd PUBLIC Boost::system)
target_link_libraries(wasd PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(wasd PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(wasd PUBLIC cpp)



#######################

add_executable(fargekamera find_farge_kamera.cpp)

target_include_directories(fargekamera PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(fargekamera PRIVATE opencv::opencv)
if (UNIX)
    target_link_libraries(fargekamera PRIVATE pthread)
endif()

add_executable(fokk recieve_pic_from_Lonk.cpp)

target_link_libraries(fokk PUBLIC Boost::system)
target_link_libraries(fokk PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(fokk PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(fokk PUBLIC cpp)

target_link_libraries(fokk PRIVATE opencv::opencv)
if (UNIX)
    target_link_libraries(fokk PRIVATE pthread)
endif()