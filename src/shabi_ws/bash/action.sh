ros2 action send_goal /shabi_group_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory "
{
  'trajectory': {
    'joint_names': ['joint1','joint2','joint3','joint4','joint5','joint6'],
    'points': [
      {
        'positions': [0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
        'time_from_start': {sec: 2}
      },
      {
        'positions': [0.3, 0.4, -0.2, 0.1, 0.3, 0.1],
        'time_from_start': {sec: 5}
      },
      {
        'positions': [0.5, 0.6, -0.3, 0.2, 0.5, 0.2],
        'time_from_start': {sec: 8}
      }
    ]
  }
}
" --feedback