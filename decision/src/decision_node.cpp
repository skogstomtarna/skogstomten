#include "ros/ros.h"
#include <ros/console.h>
#include "std_msgs/Int64.h"

ros::Publisher decision_pub;
ros::Subscriber sonic_sub,lidar_sub;

std_msgs::Int64 motor_override;

// Define variables
int sonic_data, lidar_data, camera_data;
const float w_sonic = 1;
const float w_lidar = 1;
const float w_camera = 0;
float sum=0;
const float threshold = 0.9;

// Callback, for the sonic
void sonic_callback(const std_msgs::Int64::ConstPtr &sonic_front_pub){
  sonic_data = sonic_front_pub->data;
}

// Callback, for the lidar
void lidar_callback(const std_msgs::Int64::ConstPtr &lidar_front_pub){
  lidar_data = lidar_front_pub->data;
}

void obj_decision(){
    sum=(w_sonic*sonic_data)+(w_lidar*lidar_data)+(w_camera*camera_data);
    if(sum > threshold){
      motor_override.data = 1;   // Override the Motor Node
      ROS_INFO("STOP NOW");
    }
    else{
      ROS_INFO("SAFE TO MOVE");
      motor_override.data = 0;
    }
    // Publishes true on the topic if item detected in front if the test rig
    decision_pub.publish(motor_override);
}

void node_init(){
  ros::NodeHandle nh;
  // Telling ROS which topics this node subscribes and publishes to
  sonic_sub = nh.subscribe("sonic_front_warning", 1, sonic_callback);
  lidar_sub = nh.subscribe("lidar_front_warning", 1, lidar_callback);

  decision_pub = nh.advertise<std_msgs::Int64>("motor_override", 1);
}

int main(int argc, char **argv){
  ros::init(argc, argv, "decision_node");
  node_init();
  ros::Rate loop_rate(10);
  while(ros::ok()){  
  	obj_decision();
  	ros::spinOnce();
  }
  return 0;
}
