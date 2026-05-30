#include "rclcpp/rclcpp.hpp"
#include <moveit/move_group_interface/move_group_interface.h>
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
    
    tf2::Quaternion q;
    q.setRPY(3.14,0.0,0.0);
    q=q.normalize();

    geometry_msgs::msg::PoseStamped target_pose;
    target_pose.header.frame_id="base_link";
    target_pose.pose.position.x = 0.21;
    target_pose.pose.position.y = -0.11;
    target_pose.pose.position.z = 0.11;
    target_pose.pose.orientation.x=q.getX();
    target_pose.pose.orientation.y=q.getY();
    target_pose.pose.orientation.z=q.getZ();
    target_pose.pose.orientation.w=q.getW();

    arm.setStartStateToCurrentState();
    arm.setPoseTarget(target_pose);

    moveit::planning_interface::MoveGroupInterface::Plan plan1;
    bool success1= (arm.plan(plan1)==moveit::core::MoveItErrorCode::SUCCESS);

    if (success1)
    {
        arm.execute(plan1);
    }

    rclcpp::shutdown();
    spinner.join();
    return 0;
}