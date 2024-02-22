cmake_minimum_required(VERSION 3.29)

# Force same output path for both build types
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")

project(cpp-web LANGUAGES CXX)

add_executable(main
  src/main.cpp
)

target_compile_features(main PUBLIC cxx_std_11)

target_include_directories(main PUBLIC
  ./lib/
)
