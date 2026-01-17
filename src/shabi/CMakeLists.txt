# 指定CMake最低版本（ROS 2 Humble要求3.16及以上）
cmake_minimum_required(VERSION 3.16)

# 对应package.xml中的包名，必须完全一致（shabi）
project(shabi)

# 启用C++17标准（ROS 2 Humble推荐）
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 查找ament_cmake依赖（对应package.xml中的buildtool_depend）
find_package(ament_cmake REQUIRED)

# （可选）安装URDF、launch等资源文件（若你的包中有这些文件，必须添加该步骤）
# 请根据实际文件路径修改：
# 1. 若有urdf文件：放在shabi/urdf/ 目录下
install(DIRECTORY urdf launch meshes config
  DESTINATION share/${PROJECT_NAME}
)

# 关键步骤：将包注册到ament索引，让ROS 2能识别到该包
ament_package()
