#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>  
#include <geometry_msgs/msg/pose_stamped.hpp>      
int main(int argc,char** argv)
{
    rclcpp::init(argc,argv);
    auto node=std::make_shared<rclcpp::Node>("learning");
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    auto spinner=std::thread([&executor](){executor.spin();});

    auto shabi_group=moveit::planning_interface::MoveGroupInterface(node,"shabi_group");

    shabi_group.setMaxAccelerationScalingFactor(1.0);
    shabi_group.setMaxVelocityScalingFactor(1.0);

    //----------------------------------joint_space---------------------------------
    std::vector <double> joints{0.2,0.4,0.5,0.6,0.7,0.8};
    shabi_group.setStartStateToCurrentState();
    shabi_group.setJointValueTarget(joints);


    //----------------------------------cartian_space---------------------------------
    // tf2::Quaternion q;
    // q.setRPY(3.14,0.0,0.0);
    // q=q.normalize();

    // geometry_msgs::msg::PoseStamped target_pose;
    // target_pose.header.frame_id="base_link";
    // target_pose.pose.position.x = 0.21;
    // target_pose.pose.position.y = -0.11;
    // target_pose.pose.position.z = 0.11;
    // target_pose.pose.orientation.x=q.getX();
    // target_pose.pose.orientation.y=q.getY();
    // target_pose.pose.orientation.z=q.getZ();
    // target_pose.pose.orientation.w=q.getW();

    // shabi_group.setStartStateToCurrentState();
    // shabi_group.setPoseTarget(target_pose);

    //----------------------------------calculating reality------------------
    moveit::planning_interface::MoveGroupInterface::Plan plan1;
    bool success1= (shabi_group.plan(plan1)==moveit::core::MoveItErrorCode::SUCCESS);

    if (success1)
    {
        shabi_group.execute(plan1);
    }
    rclcpp::shutdown();
    spinner.join();
    return 0;
}