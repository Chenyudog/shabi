#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.h>

using MoveGroupInterface = moveit::planning_interface::MoveGroupInterface;

class Commander
{
public:
    Commander(std::shared_ptr<rclcpp::Node> node)
    {
        node_=node;
        arm_=std::make_shared<MoveGroupInterface>(node_,"arm");
        arm_->setMaxVelocityScalingFactor(1.0);
        arm_->setMaxAccelerationScalingFactor(1.0);
    }

    void NameTarget(const std::string &name)
    {
        arm_->setStartStateToCurrentState();
        arm_->setNamedTarget(name);//这里缺少名字与定义
        planAndExecute(arm_);
    }

    void JointTarget(const std::vector<double> &joints)
    {
        arm_->setStartStateToCurrentState();
        arm_->setJointValueTarget(joints);//这里缺少名字与定义
        planAndExecute(arm_);
    }

    void PoseTarget(double x, double y,double z, double roll ,double pitch ,double yaw,bool cartesian_path=false)
    {
        tf2::Quaternion q;
        q.setRPY(3.14,0.0,0.0);
        q=q.normalize();

        geometry_msgs::msg::PoseStamped target_pose;
        target_pose.header.frame_id="base_link";
        target_pose.pose.position.x = 1;
        target_pose.pose.position.y = -1;
        target_pose.pose.position.z = 1;
        target_pose.pose.orientation.x=q.getX();
        target_pose.pose.orientation.y=q.getY();
        target_pose.pose.orientation.z=q.getZ();
        target_pose.pose.orientation.w=q.getW();

        arm_->setStartStateToCurrentState();

        if (!cartesian_path)
        {
            arm_->setPoseTarget(target_pose);
            planAndExecute(arm_);
        }
        else
        {
            std::vector<geometry_msgs::msg::Pose> waypoints;
            waypoints.push_back(target_pose.pose);
            moveit_msgs::msg::RobotTrajectory trajectory;

            // 计算笛卡尔路径 (步长 0.01)
            double fraction = arm_->computeCartesianPath(waypoints, 0.01, 0.0, trajectory, moveit_msgs::msg::Constraints(), true, nullptr);

            // 若路径规划完成 (fraction == 1 表示全程规划成功)
            if (fraction == 1) {
                arm_->execute(trajectory);
            }
        }

    }

private:
    std::shared_ptr<rclcpp::Node> node_;
    std::shared_ptr<MoveGroupInterface> arm_;

    void planAndExecute(const std::shared_ptr<MoveGroupInterface> &interface)
    {
        MoveGroupInterface::Plan plan;

        bool success = (interface->plan(plan)==moveit::core::MoveItErrorCode::SUCCESS);
        if (success)
        {
            interface->execute(plan);
        }
    }
};

int main (int argc,char **argv)
{
    rclcpp::init(argc,argv);
    auto node=std::make_shared<rclcpp::Node>("commander_template");
    auto commander=Commander(node);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}