cmake_minimum_requireD(VERSION 3.16)
project(ragdol CXX)

# C++20 Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# extra options
add_compile_options(-Wall -Wextra -O3)

# linting .h and .cpp files with clang-format
file(GLOB_RECURSE ALL_CXX_SRC
   "${PROJECT_SOURCE_DIR}/src/*.cpp"
   "${PROJECT_SOURCE_DIR}/src/*.h"
)
add_custom_target(
   format
   COMMAND clang-format -i ${ALL_CXX_SRC}
   COMMENT "Running clang-format on src"
)

# dry run to check files
add_custom_target(
   check-format
   COMMAND clang-format --dry-run --Werror ${ALL_CXX_SRC}
   COMMENT "Checking project format"
)

include_directories(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src)

# SDL support
find_package(SDL2 REQUIRED)
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})

# get source files
file(GLOB_RECURSE SRCS
   src/*.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})

# link
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

target_compile_options(ragdol PRIVATE ${SDL2_CFLAGS_OTHER})


