# https://gitlab.com/CLIUtils/modern-cmake/-/blob/master/examples/extended-project/CMakeLists.txt

cmake_minimum_required(VERSION 3.14...3.28)
project(
        IPS
        VERSION 0.0
        DESCRIPTION "Lib"
        LANGUAGES CXX
)

set(MSG_INCLUDE_DIRS "${IPS_SOURCE_DIR}/msg")
include_directories( ${MSG_INCLUDE_DIRS} )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 17)

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(apps)

# Python library is here
# add_subdirectory(python)
