add_executable(monitor_sample monitor_sample.cpp)
add_executable(window_sample window_sample.cpp)
target_include_directories(window_sample PUBLIC ${CMAKE_SOURCE_DIR}/include/Citrus/graphics) #temp: for glad
target_link_libraries(window_sample citrus::graphics citrus::sys)
target_link_libraries(monitor_sample citrus::sys)