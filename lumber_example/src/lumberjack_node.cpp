/*
  * This is a lumberjack node and it's okay.
  * It loop_rate.sleep()s all night and works all day.
*/

#include "ros/ros.h"
#include "std_msgs/Int64.h" // ros standard message types. It's possible to create your own of course.

// main loop
// note that inits and declarations can be kept in separate calls and functions to keep this clean
int main(int argc, char **argv) // ros arguments in main. always the same, I think
{
  // init ROS and create a node handle
  ros::init(argc, argv, "er_gripper_node");
  ros::NodeHandle n;
 
  // create a publisher object
  ros::Publisher lumberjack_pub;
 
  // advertise to the topic "lumber_to_mill" with a queue size of 10
  // without advertising, nothing will happen as the publisher is just hanging there, empty and alone
  lumberjack_pub = n.advertise<std_msgs::Int64>("lumber_to_mill", 10);

  // create a message object
  // Int64 msg has structrure msg->data
  // you can have msgs with bigger structures of course. Sky is the limit!
  std_msgs::Int64 lumber_msg;
  lumber_msg.data = 0;

  ros::Rate loop_rate(5); // choose ROS loop frequency in Hz. 5Hz is pretty low

  while (ros::ok()) // pretty much while(true) as long as ros is alive
  {
    // update all topics etc once
    // there is also ros::spin() which is pretty much while(true){ros::spinOnce;}
    ros::spinOnce();

    // do something
    int chopped_tree = 1;
    ROS_DEBUG("cut down %d tree", chopped_tree);
    
    // package the message
    lumber_msg.data = chopped_tree;
    ROS_DEBUG("sent lumber from lumberjack to lumbermill");
    
    // publish
    lumberjack_pub.publish(lumber_msg);

    // similar to time.sleep(), but now Powered by ROS (c) (trademark)
    loop_rate.sleep();
 }

  return 0;
}
