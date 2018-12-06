#include "ros/ros.h"
#include <ros/console.h>
#include "std_msgs/Int64.h"

ros::Publisher sonic_front_pub;
ros::Subscriber sonic_sub1, sonic_sub2, sonic_sub3, sonic_sub4, sonic_sub5, sonic_sub6, sonic_sub7, sonic_sub8;

//std_msgs::Int64 sonic_msg;
std_msgs::Int64 front_warning;

int sensor_data [8];
bool warnings[8];

//istället för att skicka 8, publisha 1 topic var, skapa egen message

//int stop [8];
//bool warning;
const int warningDistance = 100;
const int proximityStop = 30;
int sensor = 0;
int flag = 0;

// callbacks
void sonic_cb1(const std_msgs::Int64::ConstPtr &Distance1){
  sensor_data[0] = Distance1->data;
  sensor = 1;
  ROS_INFO("Sensor (%d), Distance1 %d cm.", sensor, sensor_data[0]);
  flag++;
  //stop[0] = distance_warning(sensor_data, sensor);
}
void sonic_cb2(const std_msgs::Int64::ConstPtr &Distance2){
  sensor_data[1] = Distance2->data;
  sensor = 2;
  ROS_INFO("Sensor (%d), Distance2 %d cm.", sensor, sensor_data[1]);
  flag++;
}
void sonic_cb3(const std_msgs::Int64::ConstPtr &Distance3){
  sensor_data[2] = Distance3->data;
  sensor = 3;
  ROS_INFO("Sensor (%d), Distance3 %d cm.", sensor, sensor_data[2]);
  flag++;
}
void sonic_cb4(const std_msgs::Int64::ConstPtr &Distance4){
  sensor_data[3] = Distance4->data;
  sensor = 4;
  ROS_INFO("Sensor (%d), Distance4 %d cm.", sensor, sensor_data[3]);
  flag++;
}
void sonic_cb5(const std_msgs::Int64::ConstPtr &Distance5){
  sensor_data[4] = Distance5->data;
  sensor = 5;
  ROS_INFO("Sensor (%d), Distance5 %d cm.", sensor, sensor_data[4]);
  flag++;
}
void sonic_cb6(const std_msgs::Int64::ConstPtr &Distance6){
  sensor_data[5] = Distance6->data;
  sensor = 6;
  ROS_INFO("Sensor (%d), Distance6 %d cm.", sensor, sensor_data[5]);
  flag++;
}
void sonic_cb7(const std_msgs::Int64::ConstPtr &Distance7){
  sensor_data[6] = Distance7->data;
  sensor = 7;
  ROS_INFO("Sensor (%d), Distance7 %d cm.", sensor, sensor_data[6]);
  flag = flag | 1;
}
void sonic_cb8(const std_msgs::Int64::ConstPtr &Distance8){
  sensor_data[7] = Distance8->data;
  sensor = 8;
  ROS_INFO("Sensor (%d), Distance8 %d cm.", sensor, sensor_data[7]);
  flag++;
}

void sensorWrapper(){
  for(int i=0; i<8; i++){
    if(sensor_data[i]<warningDistance){
      warnings[i] = true;
    }
    else{
      warnings[i] = false;
    }
    if(sensor_data[i]<proximityStop){

    }
  }
  if(warnings[1] == true){
    front_warning.data = 1;
  }
  else{
    front_warning.data = 0;
  }
  //Publishes true on this topic if item detected in front if the test rig
  sonic_front_pub.publish(front_warning);
}

void node_init(){
  ros::NodeHandle nh;
  sonic_sub1 = nh.subscribe("Getting sensor1 data", 1, sonic_cb1);
  sonic_sub2 = nh.subscribe("Getting sensor2 data", 1, sonic_cb2);
  sonic_sub3 = nh.subscribe("Getting sensor3 data", 1, sonic_cb3);
  sonic_sub4 = nh.subscribe("Getting sensor4 data", 1, sonic_cb4);
  sonic_sub5 = nh.subscribe("Getting sensor5 data", 1, sonic_cb5);
  sonic_sub6 = nh.subscribe("Getting sensor6 data", 1, sonic_cb6);
  sonic_sub7 = nh.subscribe("Getting sensor7 data", 1, sonic_cb7);
  sonic_sub8 = nh.subscribe("Getting sensor8 data", 1, sonic_cb8);

  if(flag==8){
    sensorWrapper();
    flag = 0;
    sonic_front_pub = nh.advertise<std_msgs::Int64>("front_warning", 1);
  }
}

int main(int argc, char **argv){
  ros::init(argc, argv, "sonic_read");
  node_init();
  ros::Rate loop_rate(10);
  ros::spin();
  return 0;
}

// bool distance_warning(sensor_data)
// {
//   if(sensor_data[sensor] < warningDistance){
//     warning = true;
//     ROS_DEBUG("Object detected in close proximity on sensor %d , sending warning.", sensor);
//   }
//   else{
//     warning = false;
//   }
//   return warning;
// }

// for(int i=0;i<8;i++){
//   sensor = i;
//   int stop[i] = distance_warning(sensor);
//
// }
