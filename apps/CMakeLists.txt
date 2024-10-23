add_executable(create_dma_buffer create_dma_buffer.cpp)
target_compile_features(create_dma_buffer PRIVATE cxx_std_17)
target_link_libraries(create_dma_buffer PRIVATE dps)

add_executable(open_dma_buffer open_dma_buffer.cpp)
target_compile_features(open_dma_buffer PRIVATE cxx_std_17)
target_link_libraries(open_dma_buffer PRIVATE dps)

add_executable(create_publisher_base create_publisher_base.cpp)
target_compile_features(create_publisher_base PRIVATE cxx_std_17)
target_link_libraries(create_publisher_base PRIVATE dps)

add_executable(inotify_topic inotify_topic.cpp)
target_compile_features(inotify_topic PRIVATE cxx_std_17)
target_link_libraries(inotify_topic PRIVATE dps)

# add_executable(epoll_dma_buffer epoll_dma_buffer.cpp)
# target_compile_features(epoll_dma_buffer PRIVATE cxx_std_17)
# target_link_libraries(epoll_dma_buffer PRIVATE dps)

# add_executable(simple_node simple_node.cpp)
# target_compile_features(simple_node PRIVATE cxx_std_17)
# target_link_libraries(simple_node PRIVATE dps)
