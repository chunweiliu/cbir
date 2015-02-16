cmake_minimum_required(VERSION 2.8)
# complier

# project
set(ProjectName demo)
project(${ProjectName})

# compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Boost
find_package(Boost COMPONENTS system filesystem program_options regex REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Gflags
#find_package(GFLAGS REQUIRED)
#include_directories(${gflags_INCLUDE_DIR})


# Sources and libraries for the compiler
add_executable(${ProjectName} ${ProjectName}.cc Retrieval.cc TextRetrieval.cc ImageRetrieval.cc HybridRetrieval.cc)
target_link_libraries(${ProjectName} ${OpenCV_LIBS} ${Boost_LIBRARIES})
# ${gflags_LIBRARIES})
