cmake_minimum_required(VERSION 3.22)
project(shabi_ws)


find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(moveit_msgs REQUIRED)

add_executable(test_moveit  src/joint_space_trajectory_control.cpp)
add_executable(cartian_pose  src/cartian_pose.cpp)
add_executable(cartian_path  src/cartian_path.cpp)
add_executable(obstacle_pose  src/obstacle_pose.cpp)

ament_target_dependencies(test_moveit
  rclcpp
  moveit_ros_planning_interface
  geometry_msgs
  tf2_geometry_msgs
  moveit_msgs
)

ament_target_dependencies(cartian_pose
  rclcpp
  moveit_ros_planning_interface
  geometry_msgs
  tf2_geometry_msgs
  moveit_msgs
)

ament_target_dependencies(cartian_path
  rclcpp
  moveit_ros_planning_interface
  geometry_msgs
  tf2_geometry_msgs
  moveit_msgs
)

ament_target_dependencies(obstacle_pose
  rclcpp
  moveit_ros_planning_interface
  geometry_msgs
  tf2_geometry_msgs
  moveit_msgs
)

install(
  DIRECTORY launch meshes config urdf bash gazebo_world
  DESTINATION share/${PROJECT_NAME}
  PATTERN "setup_assistant.launch" EXCLUDE  # 排除无需安装的 launch 文件
)


install(TARGETS
  test_moveit cartian_pose cartian_path obstacle_pose
  DESTINATION lib/${PROJECT_NAME}
)



ament_package()
