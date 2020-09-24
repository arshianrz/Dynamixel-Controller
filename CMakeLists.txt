cmake_minimum_required(VERSION 2.8.3)
project(joystick_dynamixel)
set(CMAKE_PREFIX_PATH "/home/arshia/catkin_ws/devel" "/opt/ros/melodic")
## Compile as C++11, supported in ROS Kinetic and newer

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  message_generation
  message_runtime
  roscpp
  sensor_msgs
  std_msgs
)

find_package(Qt5 REQUIRED COMPONENTS Core Network)

generate_messages(
   DEPENDENCIES
   geometry_msgs   
   sensor_msgs
   std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES joystick_dynamixel
  CATKIN_DEPENDS geometry_msgs message_generation message_runtime roscpp sensor_msgs std_msgs 
  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)


add_executable(joystick_dynamixel src/main.cpp include/joystick_dynamixel/Transmit.h src/Transmit.cpp ${QT_SOURCES} ${RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(joystick_dynamixel ${catkin_LIBRARIES} ${QT_LIBRARIES} Qt5::Network Qt5::Core -lpthread)
add_dependencies(joystick_dynamixel geometry_msgs sensor_msgs generate_messages_cpp)


