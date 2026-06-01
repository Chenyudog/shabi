import os
from launch import LaunchDescription
from launch.actions import ExecuteProcess, RegisterEventHandler, TimerAction
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch.event_handlers import OnProcessExit
import xacro
import re

def remove_comments(text):
    pattern = r'<!--(.*?)-->'
    return re.sub(pattern, '', text, flags=re.DOTALL)

def generate_launch_description():
    robot_name_in_model = 'shabi'
    package_name = 'shabi_ws'
    urdf_name = "shabi.urdf"

    pkg_share = FindPackageShare(package=package_name).find(package_name) 
    urdf_model_path = os.path.join(pkg_share, f'urdf/{urdf_name}')
    # 控制器配置文件路径（无需手动传递，gazebo_ros2_control 会自动加载（需在URDF中配置））
    controller_config_path = os.path.join(pkg_share, 'config/ros2_controllers.yaml')

    gazebo_world=os.path.join(pkg_share,'gazebo_world','cabinet.world')
    # Start Gazebo server
    start_gazebo_cmd =  ExecuteProcess(
        cmd=['gazebo', '--verbose', gazebo_world,'-s', 'libgazebo_ros_init.so', '-s', 'libgazebo_ros_factory.so'],
        output='screen')

    # 编译 URDF，移除注释
    xacro_file = urdf_model_path
    doc = xacro.parse(open(xacro_file))
    xacro.process_doc(doc)
    params = {'robot_description': remove_comments(doc.toxml())}

    # 启动 robot_state_publisher 节点
    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'use_sim_time': True}, params, {"publish_frequency":15.0}],
        output='screen'
    )

    # 在 Gazebo 中生成机械臂模型
    spawn_entity_cmd = Node(
        package='gazebo_ros', 
        executable='spawn_entity.py',
        arguments=['-entity', robot_name_in_model,  '-topic', 'robot_description'],
        output='screen')

    # 关节状态发布器（无 prefix 错误，纯净命令）
    load_joint_state_controller = ExecuteProcess(
        cmd=['ros2', 'control', 'load_controller', '--set-state', 'active',
             'joint_state_broadcaster'],
        output='screen',
    )

    # 关节轨迹控制器（无 prefix 错误，纯净命令）
    load_joint_trajectory_controller = ExecuteProcess(
        cmd=['ros2', 'control', 'load_controller', '--set-state', 'active',
             'shabi_group_controller'],
        output='screen',
    )

    # 优化控制器启动顺序：spawn_entity 退出后延迟5秒启动关节状态发布器（防乱甩）
    close_evt1 =  RegisterEventHandler( 
            event_handler=OnProcessExit(
                target_action=spawn_entity_cmd,
                on_exit=[TimerAction(period=5.0, actions=[load_joint_state_controller])],
            )
    )

    # 关节状态发布器退出后，启动轨迹控制器
    close_evt2 = RegisterEventHandler(
            event_handler=OnProcessExit(
                target_action=load_joint_state_controller,
                on_exit=[load_joint_trajectory_controller],
            )
    )
    
    ld = LaunchDescription()

    # 先添加事件监听
    ld.add_action(close_evt1)
    ld.add_action(close_evt2)

    # 启动顺序：Gazebo → robot_state_publisher → spawn_entity（移除 controller_manager_node）
    ld.add_action(start_gazebo_cmd)
    ld.add_action(node_robot_state_publisher)
    ld.add_action(spawn_entity_cmd)

    return ld