cmake_minimum_required (VERSION 2.8)
project (rovers)
set (ROVERS_VERSION_MAJOR 0)
set (ROVERS_VERSION_MINOR 6)
#set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")

include_directories(
  ${PROJECT_BINARY_DIR}
  rover
)

# It's reasonable to expect Boost to be installed.
find_package(Boost REQUIRED COMPONENTS system regex program_options)
include_directories(${Boost_INCLUDE_DIR})
add_subdirectory(config)
add_subdirectory(rover)

link_directories(${Boost_LIBRARY_DIRS})
add_executable (rovers rovers.cpp)
set (ROVERS_LIBS 
    rover
    ${Boost_LIBRARIES})
  target_link_libraries(rovers ${ROVERS_LIBS})

# Build and run unit tests
enable_testing()
find_package(GTest REQUIRED)
file(GLOB_RECURSE TEST_SRCS "../tests/*_test.cpp")
link_directories(${GTEST_LIBRARY_DIR})
add_executable(unittest-all ${TEST_SRCS})
include_directories(${GTEST_INCLUDE_DIR})
target_link_libraries(unittest-all ${ROVERS_LIBS} ${GTEST_BOTH_LIBRARIES})
gtest_add_tests(unittest-all ${TEST_SRCS})

# Make sure the build is out of source
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" insource)
if(insource)
  file(REMOVE CMakeCache.txt)
  file(REMOVE CMakeFiles)
  message(FATAL_ERROR "Please don't build in the source dir.")
endif(insource)


