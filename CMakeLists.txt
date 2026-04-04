cmake_minimum_required(VERSION 3.15)
project(MyDB VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------------------------------------------
# 1. DEPENDENCY INJECTION (GLFW & OPENGL)
# -------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

# -------------------------------------------------------------------
# 2. SOURCE TREE MAPPING
# -------------------------------------------------------------------
# Recursively captures main.cpp and all nested .cpp files inside src/
file(GLOB_RECURSE USER_SRCS "src/*.cpp")

set(IMGUI_SRCS
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
    external/imgui/misc/cpp/imgui_stdlib.cpp
)

# -------------------------------------------------------------------
# 3. THIRD-PARTY LIBRARY COMPILATION (Shielded from strict flags)
# -------------------------------------------------------------------
add_library(imgui_lib STATIC ${IMGUI_SRCS})
target_include_directories(imgui_lib PUBLIC 
    external/imgui 
    external/imgui/backends
)
target_link_libraries(imgui_lib PRIVATE glfw OpenGL::GL)

# -------------------------------------------------------------------
# 4. THE EXECUTABLE CONTRACT (Pure proprietary code)
# -------------------------------------------------------------------
add_executable(db_engine ${USER_SRCS})

# -------------------------------------------------------------------
# 5. COMPILER INCLUDE & LINKER FLAGS
# -------------------------------------------------------------------
target_include_directories(db_engine PRIVATE
    src/Crud
    src/Database
    src/Interface
    src/Interpreter
    src/Misc
)

target_link_libraries(db_engine PRIVATE imgui_lib glfw OpenGL::GL)

# -------------------------------------------------------------------
# 6. STRICT COMPILER FLAGS (Target-Scoped to protect external libs)
# -------------------------------------------------------------------
if(DEFINED WARNINGS_AS_ERRORS)
elseif(DEFINED ENV{WARNINGS_AS_ERRORS})
    set(WARNINGS_AS_ERRORS $ENV{WARNINGS_AS_ERRORS})
else()
    set(WARNINGS_AS_ERRORS ON)
endif()

if(GITHUB_ACTIONS)
    target_compile_definitions(db_engine PRIVATE GITHUB_ACTIONS)
endif()

if(MSVC)
    target_compile_options(db_engine PRIVATE /W4 /permissive- /wd4244 /wd4267 /wd4996 /external:anglebrackets /external:W0)
    if(WARNINGS_AS_ERRORS)
        target_compile_options(db_engine PRIVATE /WX)
    endif()
else()
    target_compile_options(db_engine PRIVATE -Wall -Wextra -pedantic)
    if(WARNINGS_AS_ERRORS)
        target_compile_options(db_engine PRIVATE -Werror)
    endif()
endif()

# -------------------------------------------------------------------
# 7. SANITIZERS (Target-Scoped)
# -------------------------------------------------------------------
set(is_debug "$<CONFIG:Debug>")
set(is_rel_with_deb "$<CONFIG:RelWithDebInfo>")
set(debug_mode "$<OR:${is_debug},${is_rel_with_deb}>")

if(MSVC)
    target_compile_options(db_engine PRIVATE "$<${debug_mode}:/fsanitize=address>")
elseif(APPLE)
    target_compile_options(db_engine PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
    target_link_options(db_engine PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
elseif(UNIX)
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        # GCC left clean for Valgrind
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER_EQUAL "14.0")
            target_compile_options(db_engine PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
            target_link_options(db_engine PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
        endif()
    endif()
endif()

# -------------------------------------------------------------------
# 8. INSTALLATION
# -------------------------------------------------------------------
install(TARGETS db_engine DESTINATION bin)