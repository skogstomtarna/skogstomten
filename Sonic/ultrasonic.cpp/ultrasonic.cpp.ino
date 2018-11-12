#include <ros.h>
#include <std_msgs/Int64.h>

ros::NodeHandle nh;
std_msgs::Int64 Distance1,Distance2,Distance3,Distance4;

ros::Publisher sonic1("sonic1",&Distance1);
ros::Publisher sonic2("sonic2",&Distance2);
ros::Publisher sonic3("sonic3",&Distance3);
ros::Publisher sonic4("sonic4",&Distance4);

// defines pins numbers
const int trigPin = 12;
const int echoPin1 = 8;
const int echoPin2 = 9;
const int echoPin3 = 10;
const int echoPin4 = 11;

// defines variables
long duration;
int distance;

void setup() {
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.advertise(sonic1);
  nh.advertise(sonic2);
  nh.advertise(sonic3);
  nh.advertise(sonic4);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
//  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin3, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin4, INPUT); // Sets the echoPin as an Input
  Serial.begin(57600); // Starts the serial communication
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds

  for(int i=0;i<8;i++){
    switch(i){
      case 0:
        duration = pulseIn(echoPin1, HIGH);
        distance = duration/29/2;
        Distance1.data=distance;
        //delay(10);
        //distance = 0;
      break;
      case 1:
        duration = pulseIn(echoPin2, HIGH);
        distance = duration/29/2;
        Distance2.data=distance;
        //delay(10);
      break;
      case 2:
        duration = pulseIn(echoPin3, HIGH);
        distance = duration/29/2;
        Distance3.data=distance;
        //delay(10);
      break;
      case 3:
        duration = pulseIn(echoPin4, HIGH);
        distance = duration/29/2;
        Distance4.data=distance;
        //delay(10);
      break;
    }
    //delay(10);
  }
  
  //Publishing data
  sonic1.publish(&Distance1);
  //delay(10);
  sonic2.publish(&Distance2);
  //delay(10);
  sonic3.publish(&Distance3);
  //delay(10);
  sonic4.publish(&Distance4);
  //delay(10);
  
  nh.spinOnce();
  
  delay(100);
}
