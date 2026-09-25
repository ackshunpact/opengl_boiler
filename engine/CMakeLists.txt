# Pick up every source/header automatically (re-checked on each build)
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_library(engine STATIC ${ENGINE_SOURCES} "include/engine/Application.h" "src/Application.cpp" "include/engine/VertexBuffer.h" "src/VertexBuffer.cpp" "include/engine/VertexArray.h" "src/VertexArray.cpp" "include/engine/VertexLayout.h" "src/VertexLayout.cpp" "include/engine/Shader.h" "src/Shader.cpp")

target_include_directories(engine
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"   # app sees  #include <engine/Shader.h>
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"       # engine-internal headers only
)

# PUBLIC = anything linking engine also gets these (headers + libs)
target_link_libraries(engine PUBLIC glad glfw)

if (TARGET glm)
    target_link_libraries(engine PUBLIC glm)
endif()
if (TARGET stb)
    target_link_libraries(engine PRIVATE stb)       # only engine's Texture code needs it
endif()

# Stops GLFW from including OpenGL headers, so glad/GLFW include order no longer matters
target_compile_definitions(engine PUBLIC GLFW_INCLUDE_NONE)

# Stronger warnings on our own code only (not glad)
if (MSVC)
    target_compile_options(engine PRIVATE /W4)
endif()