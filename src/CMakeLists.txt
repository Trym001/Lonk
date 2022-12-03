add_library(cont controller.cpp)

target_include_directories(cont PUBLIC "${CMAKE_SOURCE_DIR}/include")

add_library(system_timer system_timer.cpp)

target_include_directories(system_timer PUBLIC "${CMAKE_SOURCE_DIR}/include")








#CATH2 E ET UNIT TEST SYSTEM BIBLOTEK!!!!!!