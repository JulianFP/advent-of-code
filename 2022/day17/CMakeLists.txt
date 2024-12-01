# Set minimum Cmake version
cmake_minimum_required(VERSION 3.11)
# Start project and set its name
project(day17 LANGUAGES CXX)
# Add executable
add_executable(day17 main.cc)
# Add libraries
add_library(jetPatterns jetPatterns.cc)
# Link library to executable
target_link_libraries(day17 PUBLIC jetPatterns)
