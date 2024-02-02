# Optionally glob, but only for CMake 3.12 or later:
# file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${IPS_SOURCE_DIR}/include/*.hpp")
set(HEADER_LIST "${IPS_SOURCE_DIR}/include/ips.h")

# Make an automatic library - will be static or dynamic based on user setting
add_library(modern_library ips.cpp ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(modern_library PUBLIC ../include)

# This depends on (header only) boost
# target_link_libraries(modern_library PRIVATE Boost::boost)

# All users of this library will need at least C++11
target_compile_features(modern_library PUBLIC cxx_std_11)

# IDEs should put the headers in a nice place
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)
