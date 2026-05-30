ros2 topic pub /shabi_group_controller/joint_trajectory trajectory_msgs/msg/JointTrajectory "
{
  'header': { 'stamp': { 'sec': 0, 'nanosec': 0 } },
  'joint_names': ['joint1','joint2','joint3','joint4','joint5','joint6'],
  'points': [
    {
      'positions': [0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
      'time_from_start': { 'sec': 1 }
    },
    {
      'positions': [0.3, 0.4, -0.2, 0.1, 0.3, 0.0],
      'time_from_start': { 'sec': 3 }
    },
    {
      'positions': [0.5, 0.6, -0.3, 0.2, 0.5, 0.1],
      'time_from_start': { 'sec': 5 }
    }
  ]
}
" --once
