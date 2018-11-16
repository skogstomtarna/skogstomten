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
ros::Subscriber override_sub;

std_msgs::Int64 pin_msg;
int motor_override = 0;

int pinCalc(int action)
// translates motor action to pin output
{
  // format from MSB: enable1-4, dir1-4
  int pins;
  if(action==0) // idle
  {
    // all 0
    pins = 0;
  }
  else if(action==1) // forward
  {
    pins = 0b11110101;
  }
  else if(action==2) // left
  {
    pins = 0b00110011;
  }
  else if(action==3) // right
  {
    pins = 0b00110000;
  }
  else if(action==4) // backward
  {
    pins = 0b11111010;
  }
  return pins;
}

void action_callback(const std_msgs::Int64::ConstPtr &action_msg)
// get new action, translate to pins and publish
{
  // unpack the message
  int action = action_msg->data;

  // override stops forward/left/right driving
  if(motor_override==0){ pin_msg.data = pinCalc(action); }
  else if(action!=4){ pin_msg.data = 0; }

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
    if(motor_override==0){ ROS_INFO("Motor override not active."); }
    else{ ROS_INFO("Motor override active."); }
    ROS_INFO("Sent pins:");
    ROS_INFO("M1: enable(%d) direction(%d)", pin_unpk[0], pin_unpk[4]);
    ROS_INFO("M2: enable(%d) direction(%d)", pin_unpk[1], pin_unpk[5]);
    ROS_INFO("M3: enable(%d) direction(%d)", pin_unpk[2], pin_unpk[6]);
    ROS_INFO("M4: enable(%d) direction(%d)", pin_unpk[3], pin_unpk[7]);
  }
}

void override_callback(const std_msgs::Int64::ConstPtr &override_msg)
// save new override data
{
  motor_override = override_msg->data;

  // immediately stop motor as well
  if(motor_override != 0)
  {
    pin_msg.data = 0;
    pin_pub.publish(pin_msg);
  }
}

int main(int argc, char **argv)
{
  // init node and subs/pubs
  ros::init(argc, argv, "motor_controller_node");
  ros::NodeHandle n;
  action_sub = n.subscribe("motor_action", 1, action_callback);
  override_sub = n.subscribe("motor_override", 1, override_callback);
  pin_pub = n.advertise<std_msgs::Int64>("pins", 1);

  // loop away!
  ros::Rate loop_rate(10);
  ros::spin();
  return 0;
}
