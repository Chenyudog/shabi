#include "rclcpp/rclcpp.hpp"
#include <moveit/move_group_interface/move_group_interface.h>
#include <geometry_msgs/msg/pose.hpp>
#include <moveit_msgs/msg/robot_trajectory.hpp>
#include <moveit_msgs/msg/constraints.hpp>
int main(int argc,char **argv)
{
    rclcpp::init(argc,argv);

    auto node=std::make_shared<rclcpp::Node>("test_moveit");
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    auto spinner= std::thread([&executor](){executor.spin();});

    auto arm=moveit::planning_interface::MoveGroupInterface(node,"shabi_group");

    arm.setMaxVelocityScalingFactor(1.0);
    arm.setMaxAccelerationScalingFactor(1.0);

    std::vector<geometry_msgs::msg::Pose> waypoints;
    geometry_msgs::msg::Pose pose1 = arm.getCurrentPose().pose;
    pose1.position.z += -0.2;
    waypoints.push_back(pose1);

    moveit_msgs::msg::RobotTrajectory trajectory;

    // 计算笛卡尔路径 (步长 0.01)
    double fraction = arm.computeCartesianPath(waypoints, //当前点和目标点
                                                    0.01, //每一步的步长
                                                    0.0,  //跳跃变化的最大值 写0即可
                                                    trajectory, //输出的轨迹
                                                    moveit_msgs::msg::Constraints(),  //输出约束 这里不约束
                                                    true, //是否避障
                                                    nullptr);

    // 若路径规划完成 (fraction == 1 表示全程规划成功)
    if (fraction == 1) {
        arm.execute(trajectory);
    }

    rclcpp::shutdown();
    spinner.join();
    return 0;
}