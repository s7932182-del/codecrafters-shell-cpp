cmake_minimum_required(VERSION 3.28)

project(shell-starter-cpp)

# ============================================
# BUILD CONFIGURATION
# ============================================

# Force debug build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================
# COMPILER FLAGS
# ============================================

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# ============================================
# SOURCE FILES
# ============================================

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp)
message(STATUS "Source files: ${SOURCE_FILES}")

# ============================================
# EXECUTABLE
# ============================================

add_executable(shell ${SOURCE_FILES})

# ============================================
# READLINE LIBRARY (USING PKG-CONFIG)
# ============================================

# Find and use pkg-config
find_package(PkgConfig REQUIRED)

# Check for readline
pkg_check_modules(READLINE REQUIRED readline)

# Link readline and its dependencies
target_link_libraries(shell PRIVATE ${READLINE_LIBRARIES})
target_include_directories(shell PRIVATE ${READLINE_INCLUDE_DIRS})
target_compile_options(shell PRIVATE ${READLINE_CFLAGS_OTHER})

# ============================================
# DEBUGGING SUPPORT
# ============================================

# Enable debug symbols
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(shell PRIVATE -g)
    message(STATUS "Debug build enabled with symbols")
endif()

# Output build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Readline libraries: ${READLINE_LIBRARIES}")
message(STATUS "Readline include: ${READLINE_INCLUDE_DIRS}")

# ============================================
# CUSTOM TARGETS
# ============================================

# Show debug info
add_custom_target(debug-info
    COMMAND ${CMAKE_COMMAND} -E echo "======================================"
    COMMAND ${CMAKE_COMMAND} -E echo "Build Configuration:"
    COMMAND ${CMAKE_COMMAND} -E echo "  Build Type: ${CMAKE_BUILD_TYPE}"
    COMMAND ${CMAKE_COMMAND} -E echo "  C++ Standard: ${CMAKE_CXX_STANDARD}"
    COMMAND ${CMAKE_COMMAND} -E echo "  Readline: ${READLINE_LIBRARIES}"
    COMMAND ${CMAKE_COMMAND} -E echo "======================================"
)
