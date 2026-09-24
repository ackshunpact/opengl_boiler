# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#



cmake_minimum_required(VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(opengl_boiler LANGUAGES C CXX)   # rename per project; the .exe takes this name

# Settings applied to every target
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")

# ---------- Third-party libraries ----------

# glad: source, compiled as a static lib
add_library(glad STATIC "${LIB_DIR}/glad/src/glad.c")
target_include_directories(glad PUBLIC "${LIB_DIR}/glad/include")

# GLFW: prebuilt static lib (MSVC 2022, x64)
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION             "${LIB_DIR}/glfw/lib-vc2022/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${LIB_DIR}/glfw/include"
)

# glm: header-only. Only defined once you've added libraries/glm
if (EXISTS "${LIB_DIR}/glm/glm/glm.hpp")
    add_library(glm INTERFACE)
    target_include_directories(glm INTERFACE "${LIB_DIR}/glm")
endif()

# stb_image: header-only. Only defined once you've added libraries/stb
if (EXISTS "${LIB_DIR}/stb/stb_image.h")
    add_library(stb INTERFACE)
    target_include_directories(stb INTERFACE "${LIB_DIR}/stb")
endif()

# ---------- Our code ----------
add_subdirectory(engine)
add_subdirectory(app)
