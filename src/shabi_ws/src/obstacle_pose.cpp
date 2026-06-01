#include <memory>
#include <thread>
#include <chrono>

#include "rclcpp/rclcpp.hpp"

#include "moveit/move_group_interface/move_group_interface.h"
#include "moveit/planning_scene_interface/planning_scene_interface.h"

#include "moveit_msgs/msg/collision_object.hpp"
#include "shape_msgs/msg/solid_primitive.hpp"
#include "geometry_msgs/msg/pose.hpp"

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);

    auto node = rclcpp::Node::make_shared("obstacle_pose_node");

    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    std::thread spinner([&executor]() {
        executor.spin();
    });

    moveit::planning_interface::MoveGroupInterface move_group(node, "shabi_group");
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

    move_group.setPlanningTime(10.0);
    move_group.setMaxVelocityScalingFactor(0.2);
    move_group.setMaxAccelerationScalingFactor(0.2);

    // 1. 添加柜子障碍物
    moveit_msgs::msg::CollisionObject object;
    object.header.frame_id = move_group.getPlanningFrame();  // 一般是 base_link
    object.id = "cabinet";

    shape_msgs::msg::SolidPrimitive primitive;
    primitive.type = shape_msgs::msg::SolidPrimitive::BOX;
    primitive.dimensions.resize(3);
    primitive.dimensions[shape_msgs::msg::SolidPrimitive::BOX_X] = 0.45;
    primitive.dimensions[shape_msgs::msg::SolidPrimitive::BOX_Y] = 0.45;
    primitive.dimensions[shape_msgs::msg::SolidPrimitive::BOX_Z] = 1.02;

    geometry_msgs::msg::Pose box_pose;
    box_pose.orientation.w = 1.0;
    box_pose.position.x = -0.501;
    box_pose.position.y = 0.016;
    box_pose.position.z = 0.51;

    object.primitives.push_back(primitive);
    object.primitive_poses.push_back(box_pose);
    object.operation = object.ADD;

    planning_scene_interface.applyCollisionObject(object);

    RCLCPP_INFO(node->get_logger(), "已添加 cabinet 障碍物到 MoveIt Planning Scene");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 2. 设置一个目标位姿
    std::vector<double> target_pose {2.011,-1.13, -0.016, 1.076, -0.934, -1.563};

    move_group.setStartStateToCurrentState();
    move_group.setJointValueTarget(target_pose);

    // 3. 开始规划
    moveit::planning_interface::MoveGroupInterface::Plan plan;

    bool success = static_cast<bool>(move_group.plan(plan));

    if (success)
    {
        RCLCPP_INFO(node->get_logger(), "规划成功，开始执行");
        move_group.execute(plan);
    }
    else
    {
        RCLCPP_ERROR(node->get_logger(), "规划失败，可能是目标点不可达或障碍物挡住了");
    }

    rclcpp::shutdown();
    spinner.join();

    return 0;
}