
#include "ros.h"
#include <ros/console.h>
#include "std_msgs/Int64.h"

ros::Publisher sonic_pub;
ros::Subscriber sonic_sub;

std_msgs::Int64 sonic_msg;

int sensor_data [8];
int stop [8];
bool warning;
const int warningDistance = 100;

int flag = 0;

bool distance_warning(sensor_data, sensor)
{
  if(sensor_data[sensor] < warningDistance){
    warning = true;
    ROS_DEBUG("Object detected in close proximity on sensor %d , sending warning.", sensor);
  }
  else{
    warning = false;
  }
  return warning;
}

// for(int i=0;i<8;i++){
//   sensor = i;
//   int stop[i] = distance_warning(sensor);
//
// }

// callbacks
void sonic_cb1(const std_msgs::Int64::ConstPtr &Distance1)
{
  sensor_data[0] = Distance1->data;
  sensor = 1;
  ROS_DEBUG("Sensor %d", sensor,  "Distance1 %d cm.",sensor_data[0]);
  flag = flag | 1;
  //stop[0] = distance_warning(sensor_data, sensor);
}
void sonic_cb2(const std_msgs::Int64::ConstPtr &Distance2)
{
  sensor_data[1] = Distance2->data;
  sensor = 2;
  ROS_DEBUG("Sensor %d", sensor,  "Distance1 %d cm.",sensor_data[1]);
  flag = flag | 1;
  //stop[0] = distance_warning(sensor_data, sensor);
}
void sonic_cb3(const std_msgs::Int64::ConstPtr &Distance3)
{
  sensor_data[2] = Distance3->data;
  sensor = 3;
  ROS_DEBUG("Sensor %d", sensor,  "Distance1 %d cm.",sensor_data[2]);
  flag = flag | 1;
  //stop[0] = distance_warning(sensor_data, sensor);
}
void sonic_cb4(const std_msgs::Int64::ConstPtr &Distance4)
{
  sensor_data[3] = Distance4->data;
  sensor = 4;
  ROS_DEBUG("Sensor %d", sensor,  "Distance1 %d cm.",sensor_data[3]);
  flag = flag | 1;
  //stop[0] = distance_warning(sensor_data, sensor);
}
void sonic_cb5(const std_msgs::Int64::ConstPtr &Distance5)
{
  sensor_data[4] = Distance5->data;
  sensor = 5;
  ROS_DEBUG("Sensor %d", sensor,  "Distance1 %d cm.",sensor_data[4]);
  flag = flag | 1;
  //stop[0] = distance_warning(sensor_data, sensor);
}
void sonic_cb6(const std_msgs::Int64::ConstPtr &Distance6)
{
  sensor_data[5] = Distance6->data;
  sensor = 6;
  ROS_DEBUG("Sensor %d", sensor,  "Distance1 %d cm.",sensor_data[5]);
  flag = flag | 1;
  //stop[0] = distance_warning(sensor_data, sensor);
}
void sonic_cb7(const std_msgs::Int64::ConstPtr &Distance7)
{
  sensor_data[6] = Distance7->data;
  sensor = 7;
  ROS_DEBUG("Sensor %d", sensor,  "Distance1 %d cm.",sensor_data[6]);
  flag = flag | 1;
  //stop[0] = distance_warning(sensor_data, sensor);
}
void sonic_cb8(const std_msgs::Int64::ConstPtr &Distance8)
{
  sensor_data[7] = Distance8->data;
  sensor = 8;
  ROS_DEBUG("Sensor %d", sensor,  "Distance8 %d cm.",sensor_data[7]);
  flag = flag | 1;
  //stop[0] = distance_warning(sensor_data, sensor);
}

bool warnings[8];

void sensorWrapper()
{
  for(int i=0; i<8; i++)
  {
    if(sensor_data[i]<warningDistance)
    {
      warnings[i] = true;
    }
    else
    {
      warnings[i] = false;
    }
  }
}

void node_init()
{
  ros::NodeHandle nh;
  sonic_sub = n.subscribe("Getting distance data", sonic_callback);
}

// very clean main loop
int main(int argc, char **argv)
{
  ros::init(argc, argv, "sonic_read_node");
  node_init();
  ros::Rate loop_rate(5);
  ros::spin();
  if(flags==255)
  {
    sensorWrapper();
  }
  return 0;
}
