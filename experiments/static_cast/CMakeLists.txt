cmake_minimum_required(VERSION 3.5)

project(dps LANGUAGES CXX C)

############################################################
# Create a executable
############################################################

set(MAIN_SRCS "main.cpp")
add_executable(main ${MAIN_SRCS})
