add_executable(main main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE modern_library)

add_executable(cvcpp cvcpp.cpp)
target_compile_features(cvcpp PRIVATE cxx_std_17)
target_link_libraries( cvcpp PRIVATE ${OpenCV_LIBS} )