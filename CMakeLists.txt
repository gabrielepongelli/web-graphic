# Modern CMake
cmake_minimum_required(VERSION 3.18)
project(webgraphic)

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
include(json)

set(CMAKE_FOLDER "")

add_executable(${PROJECT_NAME}
    src/graphic/sender.cpp
    src/graphic/receiver.cpp
    src/utilities/string_utilities.cpp
    src/graphic/web_graphic.cpp
    src/main.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_${CMAKE_CXX_STANDARD})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ixwebsocket
    nlohmann_json::nlohmann_json
)