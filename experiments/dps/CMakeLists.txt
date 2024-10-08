cmake_minimum_required(VERSION 3.5)

project(dps LANGUAGES CXX C)

############################################################
# Create a library
############################################################

#Generate the shared library from the library sources

set(SHARED_CPP_FILES 
    shared/src/node.cpp
)

set(SRC_CPP_FILES 
    src/dma_proxy.cpp
)

add_library(dps SHARED 
    ${SHARED_CPP_FILES}
    ${SRC_CPP_FILES}
)

target_include_directories(dps
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/shared/include
)
