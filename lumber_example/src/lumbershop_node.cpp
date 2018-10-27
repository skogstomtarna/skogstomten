/*
  * Lumbershop node. Takes planks and sells them or whatever.
*/

#include "ros/ros.h"
#include <ros/console.h>
#include "std_msgs/Int64.h"

// subscriber
ros::Subscriber planks_sub;

// processing planks
int process_planks(int planks)
{
  // haha I'm just trashing them
  int planks_before = planks;
  planks = 0;
  ROS_DEBUG("threw away %d planks, fuck nature and profit", planks_before);
  return planks;
}

// callback
void shop_callback(const std_msgs::Int64::ConstPtr &planks_msg)
{
  int planks = planks_msg->data;
  ROS_DEBUG("received %d planks",planks);

  // now do something with them, like sell them or whatever
  planks = process_planks(planks);
}

void node_init()
{
  ros::NodeHandle n;
  planks_sub = n.subscribe("planks_to_shop", 10, shop_callback);
}

// very clean main loop
int main(int argc, char **argv)
{
  ros::init(argc, argv, "lumbershop_node");
  node_init();
  ros::Rate loop_rate(5);
  ros::spin();

  return 0;
}
