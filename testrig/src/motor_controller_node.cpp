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

// pin LUT for different actions, corresponding to the keypad on a standard PC
int pin_actions[] = {0b11111011, 0b11111010, 0b11111000,
                     0b00110011, 0b00000000, 0b00110000,
                     0b11110111, 0b11110101, 0b11110100};

int pinCalc(int action)
// translates motor action to pin output
{

  int pins;
  if( action<0 || action>8 )
  // guard: if index is "invalid", set to idle
  {
    pins = 0;
  }
  else
  // set to actual action
  {
    pins = pin_actions[action];
  }
  return pins;
}

void action_callback(const std_msgs::Int64::ConstPtr &action_msg)
// get new action, translate to pins and publish
{
  // unpack the message
  int action = action_msg->data;

  // override stops all driving at the moment
  if(motor_override==0){ pin_msg.data = pinCalc(action); }
  else{ pin_msg.data = 0; }

  // publish pins to arduino
  pin_pub.publish(pin_msg);

  // Debug msgs
  if(DEBUG)
  {
    std::string action_list[] = {"Backward Left", "Backward", "Backward Right",
                                 "Left",          "Idle",     "Right",
                                 "Forward Left",  "Forward",  "Forward Right"};
    ROS_INFO("Received action:");
    ROS_INFO_STREAM(action_list[action]);
    char pin_unpk[8];
    for(int i=0; i<8; i++)
    {
      pin_unpk[7-i] = (pin_msg.data)%2;
      pin_msg.data = (pin_msg.data)>>1;
    }
    if(motor_override==0){ ROS_INFO("Motor override inactive."); }
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

  // immediately stop motors as well
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
