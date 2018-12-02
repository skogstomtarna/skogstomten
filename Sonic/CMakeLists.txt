cmake_minimum_required(VERSION 2.8.3)
project(lumber_example)

find_package(catkin REQUIRED COMPONENTS
  std_msgs
  roscpp
)

add_compile_options(-std=c++11)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp std_msgs
)

include_directories(
  include ${catkin_INCLUDE_DIRS}
)

add_executable(sonic1_node src/ultrasonic.cpp/ultrasonic.cpp.ino)
add_executable(sonic2_node src/ultrasonic2.cpp/ultrasonic.cpp.ino)
add_executable(sonic_read_node src/sonic_read.cpp)

add_dependencies(sonic1_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(sonic2_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(sonic_read_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})


target_link_libraries(sonic1_node ${catkin_LIBRARIES})
target_link_libraries(sonic2_node ${catkin_LIBRARIES})
target_link_libraries(sonic_read_node ${catkin_LIBRARIES})
