add_executable(main main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE modern_library)

add_executable(cvcpp cvcpp.cpp)
target_compile_features(cvcpp PRIVATE cxx_std_17)
target_link_libraries( cvcpp PRIVATE ${OpenCV_LIBS})

add_executable(gen_struct gen_struct.cpp)
target_compile_features(gen_struct PRIVATE cxx_std_17)
target_link_libraries(gen_struct PRIVATE modern_library)

add_executable(cma_allocate cma_allocate.cpp)
target_compile_features(cma_allocate PRIVATE cxx_std_17)
target_link_libraries(cma_allocate PRIVATE modern_library)