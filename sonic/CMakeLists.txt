cmake_minimum_required(VERSION 2.8.3)
project(sonic)

find_package(catkin REQUIRED COMPONENTS
  std_msgs
  roscpp
)

add_compile_options(-std=c++11)

catkin_package(
  INCLUDE_DIRS #include
  CATKIN_DEPENDS roscpp std_msgs
)

include_directories(
  include ${catkin_INCLUDE_DIRS}
)

add_executable(sonic_read src/sonic_read.cpp)

add_dependencies(sonic_read ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(sonic_read ${catkin_LIBRARIES})
