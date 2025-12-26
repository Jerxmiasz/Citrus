include(FetchContent)
FetchContent_Declare(GLFW
  GIT_REPOSITORY https://github.com/glfw/glfw.git/
  GIT_TAG 3.4
)
FetchContent_MakeAvailable(GLFW)


add_library(citrus_sys STATIC
    globals.cpp
    Monitor.cpp
)

target_link_libraries(citrus_sys PUBLIC glfw)

# Provide namespaced target citrus::sys
add_library(citrus::sys ALIAS citrus_sys)

target_include_directories(citrus_sys
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        
)