cmake_minimum_required(VERSION 3.5)

project(dps LANGUAGES CXX C)

############################################################
# Create a executable
############################################################

add_library(dps SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(dps PROPERTIES
  IMPORTED_LOCATION "/home/d3dx13/workspace/ips/experiments/dps/build/libdps.so"
  INTERFACE_INCLUDE_DIRECTORIES "/home/d3dx13/workspace/ips/experiments/dps/include"
)

set(MAIN_SRCS "main.cpp")
add_executable(main ${MAIN_SRCS})
target_link_libraries(main dps)
