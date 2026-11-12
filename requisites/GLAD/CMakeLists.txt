# GLAD 0.1.36 - OpenGL 3.3 core loader
add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})   # dlopen on Linux/macOS, empty on Windows
