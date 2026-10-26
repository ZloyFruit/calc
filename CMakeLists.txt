cmake_minimum_required(VERSION 3.15)
project(Calculator CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(calc main.cpp)

add_library(funcdeg SHARED plugins/funcdeg.cpp)
add_library(funcsin SHARED plugins/funcsin.cpp)

target_include_directories(funcdeg PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(funcsin PRIVATE ${CMAKE_SOURCE_DIR})

add_custom_command(TARGET funcdeg POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:calc>/plugins"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:funcdeg>" "$<TARGET_FILE_DIR:calc>/plugins/"
    COMMENT "Copying funcdeg.dll to plugins directory"
)

add_custom_command(TARGET funcsin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:calc>/plugins"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:funcsin>" "$<TARGET_FILE_DIR:calc>/plugins/"
    COMMENT "Copying funcsin.dll to plugins directory"
)