cmake_minimum_required(VERSION 2.8.11)

project(WorkYourEnglish)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH "C:\\Libs\\Qt\\5.7\\mingw53_32")
set(CMAKE_BUILD_TYPE Release)

# Executable
file(GLOB SRC_FILES *.cpp)
add_executable(WorkYourEnglish ${SRC_FILES})

# Qt Widgets
find_package(Qt5Widgets)
target_link_libraries(WorkYourEnglish Qt5::Widgets)

# Qt SQL
find_package(Qt5Sql)
target_link_libraries(WorkYourEnglish Qt5::Sql)
