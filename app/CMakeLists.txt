file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})

# engine brings glad, glfw, glm and their include paths along with it
target_link_libraries(${PROJECT_NAME} PRIVATE engine)

# Lets code use  ASSET_DIR "/shaders/basic.vert"  regardless of working directory
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ASSET_DIR="${CMAKE_SOURCE_DIR}/assets"
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()