# Modern CMake
cmake_minimum_required(VERSION 3.18)
project(performance_meter)

# Add module path for dependencies
list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Enable solution folder support
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_FOLDER "Dependencies")
include(CPM)
include(zlib)
include(mbedtls)
include(ixwebsocket)

set(CMAKE_FOLDER "")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/graphic)

add_executable(${PROJECT_NAME}
    src/graphic/sender.cpp
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ixwebsocket
)