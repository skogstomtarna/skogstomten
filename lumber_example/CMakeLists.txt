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

add_executable(lumberjack_node src/lumberjack_node.cpp)
add_executable(lumbermill_node src/lumbermill_node.cpp)
add_executable(lumbershop_node src/lumbershop_node.cpp)

add_dependencies(lumberjack_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(lumbermill_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(lumbershop_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})


target_link_libraries(lumberjack_node ${catkin_LIBRARIES})
target_link_libraries(lumbermill_node ${catkin_LIBRARIES})
target_link_libraries(lumbershop_node ${catkin_LIBRARIES})
