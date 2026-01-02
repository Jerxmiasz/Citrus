add_library(citrus_graphics STATIC glad.c OpenGL/Renderer.cpp OpenGL/Shader.cpp)
add_library(citrus::graphics ALIAS citrus_graphics)
target_include_directories(citrus_graphics PUBLIC ${CMAKE_SOURCE_DIR}/include/)
target_include_directories(citrus_graphics PUBLIC ${CMAKE_SOURCE_DIR}/include/Citrus/graphics) #temp for glad
target_link_libraries(citrus_graphics citrus::sys)
