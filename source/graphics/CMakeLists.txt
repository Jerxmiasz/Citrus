add_library(citrus_graphics STATIC glad.c)
add_library(citrus::graphics ALIAS citrus_graphics)
target_include_directories(citrus_graphics PUBLIC ${CMAKE_SOURCE_DIR}/include/Citrus/graphics)
