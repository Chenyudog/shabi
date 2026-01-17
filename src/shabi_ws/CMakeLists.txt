cmake_minimum_required(VERSION 3.22)
project(shabi_ws)

# 查找 ROS 2 核心依赖
find_package(ament_cmake REQUIRED)

# 安装规则：移除冗余的 urdf 目录（shabi_ws 包无 urdf 目录）
# 仅安装 shabi_ws 包实际存在的目录：launch、meshes、config
install(
  DIRECTORY launch meshes config urdf
  DESTINATION share/${PROJECT_NAME}
  PATTERN "setup_assistant.launch" EXCLUDE  # 排除无需安装的 launch 文件
)

# 安装 .setup_assistant 文件（MoveIt 配置生成的文件，按需保留）
install(FILES .setup_assistant DESTINATION share/${PROJECT_NAME})

# 声明 ROS 2 功能包（必须放在最后）
ament_package()
