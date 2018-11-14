/*
  * This "translates" action from keyboard node to pin msg sent to arduino
  * Pretty easy and can change motor functionality a lot without ever touching the 'ino
*/

#include "ros/ros.h"
#include <ros/console.h>
#include "std_msgs/Int64.h"

bool DEBUG = true;
// publisher and subscriber objects
ros::Publisher pin_pub;
ros::Subscriber action_sub;

int pinCalc(int action)
{
  // format from MSB: enable1-4, dir1-4
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
void action_callback(const std_msgs::Int64::ConstPtr &action_msg)
{
  // unpack the message
  int action = action_msg->data;

  std_msgs::Int64 pin_msg;
  pin_msg.data = pinCalc(action);

  // publish pins to arduino
  pin_pub.publish(pin_msg);

  // Debug msgs
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
    char pin_unpk[8];
    for(int i=0; i<8; i++)
    {
      pin_unpk[7-i] = (pin_msg.data)%2;
      pin_msg.data = (pin_msg.data)>>1;
    }
    ROS_INFO("Sent pins:");
    ROS_INFO("M1: enable(%d) direction(%d)", pin_unpk[0], pin_unpk[4]);
    ROS_INFO("M2: enable(%d) direction(%d)", pin_unpk[1], pin_unpk[5]);
    ROS_INFO("M3: enable(%d) direction(%d)", pin_unpk[2], pin_unpk[6]);
    ROS_INFO("M4: enable(%d) direction(%d)", pin_unpk[3], pin_unpk[7]);
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "motor_controller_node");
  ros::NodeHandle n;

  action_sub = n.subscribe("motor_action", 1, action_callback);
  pin_pub = n.advertise<std_msgs::Int64>("pins", 1);

  ros::Rate loop_rate(10);
  ros::spin();
  return 0;
}
