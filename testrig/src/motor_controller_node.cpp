/*
  * "translates" action from keyboard node to pin msg sent to arduino
*/

#include "ros/ros.h"
#include <ros/console.h>
#include "std_msgs/Int8.h"

bool DEBUG = true;
// publisher and subscriber objects
ros::Publisher pin_pub;
ros::Subscriber action_sub;

int pinCalc(int action)
{
  // format from MSB: enable0-3, dir0-3
  int pin_msg;
  if(action==0) // idle
  {
    // all 0
    pin_msg = 0;
  }
  else if(action==1) // forward
  {
    // enable all, dir 0 all
    pin_msg = 0xF0;
  }
  else if(action==2) // left
  {
    // enable all, dir0,dir2 = 1, dir1,dir3 = 0
    pin_msg = 0xFA;
  }
  else if(action==3) // right
  {
    pin_msg = 0xF5;
  }
  else if(action==4) // backward
  {
    // enable all, dir 1 all
    pin_msg = 0xFF;
  }
  return pin_msg;
}

// callback for the subscriber
void action_callback(const std_msgs::Int8::ConstPtr &action_msg)
{
  // unpack the message
  int action = action_msg->data;

  std_msgs::Int8 pin_msg;
  pin_msg.data = pinCalc(action);

  // publish the planks
  pin_pub.publish(pin_msg);

  if(DEBUG)
  {
    ROS_INFO("Received action:");
    if(action==1)
    {
      ROS_INFO("Drive forward");
    }
    else if(action==2)
    {
      ROS_INFO("Turn left");
    }
    else if(action==3)
    {
      ROS_INFO("Turn right");
    }
    else if(action==4)
    {
      ROS_INFO("Drive backward");
    }
    else
    {
      ROS_INFO("Idle");
    }
    /*
    ROS_INFO("Sent pins:");
    ROS_INFO("M1: enable(%d) direction(%d)", pin_msg.en1, pin_msg.dir1);
    ROS_INFO("M2: enable(%d) direction(%d)", pin_msg.en2, pin_msg.dir2);
    ROS_INFO("M3: enable(%d) direction(%d)", pin_msg.en3, pin_msg.dir3);
    ROS_INFO("M4: enable(%d) direction(%d)", pin_msg.en4, pin_msg.dir4);
    */
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "motor_controller_node");
  ros::NodeHandle n;

  action_sub = n.subscribe("motor_action", 1, action_callback);
  pin_pub = n.advertise<std_msgs::Int8>("pin", 1);

  ros::Rate loop_rate(10);
  ros::spin();
  return 0;
}
