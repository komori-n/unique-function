cmake_minimum_required(VERSION 3.0)
project(func_test CXX)
set(CMAKE_CXX_STANDARD 17)
set(PROGRAM func_test)

option(BUILD_GMOCK "Builds the googlemock subprojects" OFF)
option(INSTALL_GTEST "Enables installation of googletest" OFF)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Google Testをこのプロジェクトに組み込む

add_subdirectory(third-party/googletest)
file(GLOB TEST_SOURCES
  test/*.cpp)
add_executable(${PROGRAM}
  ${TEST_SOURCES})

target_link_libraries(${PROGRAM}
  PRIVATE
    gtest
    gtest_main
)
target_include_directories(${PROGRAM}
  PUBLIC
    include
    third-party/googletest/googletest/include
)

enable_testing()
add_test(NAME KomoriFunctionTest COMMAND func_test)
