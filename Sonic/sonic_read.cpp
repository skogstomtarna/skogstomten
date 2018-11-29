
#include "ros.h"
#include <ros/console.h>
#include "std_msgs/Int64.h"

ros::Publisher sonic_pub;
ros::Subscriber sonic_sub;

std_msgs::Int64 sonic_msg;

int sensor_data [8];
int stop [8];

int distance_warning(sensor)
{
  if(sensor_data[sensor] < 50){
    warning = 1;
    ROS_DEBUG("Object detected in close proximity on sensor %d , sending warning.", sensor);
  }
  else{
    warning = 0;
  }

  return warning;
}

for(int i=0;i<8;i++){
  sensor = i;
  int stop[i] = distance_warning(sensor);

}
  sensor_data[0] = Distance1->data;
  sensor_data[1] = Distance2->data;
  sensor_data[2] = Distance3->data;
  sensor_data[3] = Distance4->data;
  sensor_data[4] = Distance5->data;
  sensor_data[5] = Distance6->data;
  sensor_data[6] = Distance7->data;
  sensor_data[7] = Distance8->data;

  // callback
  void sonic_callback(const std_msgs::Int64::ConstPtr &Distance1)
  {
    sensor_data[0] = Distance1->data;
    ROS_DEBUG("Distance1 %d cm.",sensor_data[0]);
    sensor = 0;
    stop[0] = distance_warning(sensor);
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

    return 0;
  }
