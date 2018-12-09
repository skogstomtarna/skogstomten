#include "ros/ros.h"
#include <ros/console.h>
#include "std_msgs/Int64.h"

ros::Publisher sonic_front_pub;
ros::Subscriber sonic_sub1, sonic_sub2, sonic_sub3, sonic_sub4, sonic_sub5, sonic_sub6, sonic_sub7, sonic_sub8;

std_msgs::Int64 sonic_front_warning;

// Define variables
int sensor_data [8];
bool warnings[8];
bool frontStop;
const int fWarning = 100;     // Front warning distance
const int bWarning = 30;      // Back warning distance
const int sideWarning = 30;   // Side warning distance
int sensor = 0;
int flag = 0;

// Callbacks, assigning sensor data from topics to the variable sensor_data and setting the flag
void sonic_cb1(const std_msgs::Int64::ConstPtr &Distance1){
  sensor_data[0] = Distance1->data;
  //ROS_INFO("Sensor (%d), Distance1 %d cm.", sensor, sensor_data[0]);    // In case printing the distance is desired
  flag = flag | 1;
  //ROS_INFO("Flag value %d", flag);
  /*if(sensor_data[0] < sideWarning && sensor_data[0] !=0 ){
    ROS_INFO("Warning FRONT LEFT");
  }
  sonic_front_pub.publish(front_warning);*/
}
void sonic_cb2(const std_msgs::Int64::ConstPtr &Distance2){
  sensor_data[1] = Distance2->data;
  //ROS_INFO("Distance2 %d cm.", sensor_data[1]);
  flag = flag | 2;
  //ROS_INFO("Flag value %d", flag);
  /*if(sensor_data[1] < fbWarning && sensor_data[1] != 0){
    front_warning.data = 1;   // Set the warning to the decision_node
    ROS_INFO("Warning FRONT");
  }
  else{
    front_warning.data = 0;
  }
  sonic_front_pub.publish(front_warning);*/
}
void sonic_cb3(const std_msgs::Int64::ConstPtr &Distance3){
  sensor_data[2] = Distance3->data;
  //ROS_INFO("Sensor (%d), Distance3 %d cm.", sensor, sensor_data[2]);
  flag = flag | 4;
  //ROS_INFO("Flag value %d", flag);
  /*if(sensor_data[2] < fbWarning && sensor_data[2] != 0){
    front_warning.data = 1;   // Set the warning to the decision_node
    ROS_INFO("Warning FRONT");
  }
  else{
    front_warning.data = 0;
  }*/
}
void sonic_cb4(const std_msgs::Int64::ConstPtr &Distance4){
  sensor_data[3] = Distance4->data;
  //ROS_INFO("Sensor (%d), Distance4 %d cm.", sensor, sensor_data[3]);
  flag = flag | 8;
  //ROS_INFO("Flag value %d", flag);
  /*if(sensor_data[3] < sideWarning && sensor_data[3] !=0 ){
    ROS_INFO("Warning FRONT RIGHT");
  }*/
}
void sonic_cb5(const std_msgs::Int64::ConstPtr &Distance5){
  sensor_data[4] = Distance5->data;
  //ROS_INFO("Sensor (%d), Distance5 %d cm.", sensor, sensor_data[4]);
  flag = flag | 16;
  //ROS_INFO("Flag value %d", flag);
  if(sensor_data[4] < sideWarning && sensor_data[4] !=0 ){
    ROS_INFO("Warning BACK RIGHT");
  }
}
void sonic_cb6(const std_msgs::Int64::ConstPtr &Distance6){
  sensor_data[5] = Distance6->data;
  //ROS_INFO("Sensor (%d), Distance6 %d cm.", sensor, sensor_data[5]);
  flag = flag | 32;
  //ROS_INFO("Flag value %d", flag);
  /*if(sensor_data[5] < sideWarning && sensor_data[5] !=0){
    ROS_INFO("Warning BACKWARDS");
  }*/
}
void sonic_cb7(const std_msgs::Int64::ConstPtr &Distance7){
  sensor_data[6] = Distance7->data;
  //ROS_INFO("Sensor (%d), Distance7 %d cm.", sensor, sensor_data[6]);
  flag = flag | 64;
  //ROS_INFO("Flag value %d", flag);
  /*if(sensor_data[6] < sideWarning && sensor_data[6] != 0){
      ROS_INFO("Warning BACKWARDS");
    }*/
}
void sonic_cb8(const std_msgs::Int64::ConstPtr &Distance8){
  sensor_data[7] = Distance8->data;
  //ROS_INFO("Sensor (%d), Distance8 %d cm.", sensor, sensor_data[7]);
  flag = flag | 128;
  //ROS_INFO("Flag value %d", flag);
  /*if(sensor_data[7] < sideWarning && sensor_data[7] != 0){
    ROS_INFO("Warning BACK LEFT");
  }*/
}

void sensorWrapper(){
   //ROS_INFO("HELLO WORLD");
   if(sensor_data[0] < sideWarning && sensor_data[0] !=0 ){
     ROS_INFO("Warning FRONT LEFT");
    }
  //if(sensor_data[1] < fWarning && sensor_data[1] != 0 || sensor_data[2] < fWarning && sensor_data[2] != 0){	//Sensor3 does not work properly at the moment
  if(sensor_data[1] < fWarning && sensor_data[1] != 0){ 
    sonic_front_warning.data = 1;   // Set the warning to the decision_node
    ROS_INFO("Warning FRONT");
  }
  else{
    sonic_front_warning.data = 0;
  }
  if(sensor_data[3] < sideWarning && sensor_data[3] !=0 ){
    ROS_INFO("Warning FRONT RIGHT");
  }
  if(sensor_data[4] < sideWarning && sensor_data[4] !=0 ){
    ROS_INFO("Warning BACK RIGHT");
  }
  if(sensor_data[5] < bWarning && sensor_data[5] !=0 || sensor_data[6] < bWarning && sensor_data[6] != 0){
    ROS_INFO("Warning BACKWARDS");
  }
  if(sensor_data[7] < sideWarning && sensor_data[7] != 0){
    ROS_INFO("Warning BACK LEFT");
  }
  // Publishes true on the topic if item detected in front if the test rig
  sonic_front_pub.publish(sonic_front_warning);
  flag = 0;
}

void node_init(){
  ros::NodeHandle nh;
  // Telling ROS which topics this node subscribes and publishes to
  sonic_sub1 = nh.subscribe("sonic1", 1, sonic_cb1);
  sonic_sub2 = nh.subscribe("sonic2", 1, sonic_cb2);
  sonic_sub3 = nh.subscribe("sonic3", 1, sonic_cb3);
  sonic_sub4 = nh.subscribe("sonic4", 1, sonic_cb4);
  sonic_sub5 = nh.subscribe("sonic5", 1, sonic_cb5);
  sonic_sub6 = nh.subscribe("sonic6", 1, sonic_cb6);
  sonic_sub7 = nh.subscribe("sonic7", 1, sonic_cb7);
  sonic_sub8 = nh.subscribe("sonic8", 1, sonic_cb8);
  ROS_INFO("SUCCESS");
  sonic_front_pub = nh.advertise<std_msgs::Int64>("sonic_front_warning", 1);
}

int main(int argc, char **argv){
  ros::init(argc, argv, "sonic_node");
  node_init();
  ros::Rate loop_rate(100);
  while(ros::ok()){
    if(flag==255){  // The flag is set in the callbacks to ensure the wrapper only runs after recieving data from all the sensors.
      sensorWrapper();
      //flag = 0;
    }
    ros::spinOnce();
  }
  return 0;
}
