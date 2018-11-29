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
const int echoPin1 = 11;
const int echoPin2 = 10;
const int echoPin3 = 9;
const int echoPin4 = 8;
const int ledPin1 = 7;
const int ledPin2 = 6;
const int ledPin3 = 5;
const int ledPin4 = 4;

// defines variables
long duration1, duration2, duration3, duration4;
int distance1, distance2, distance3, distance4;
//int warning1, warning2, warning3, warning4;

void setup() {
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.advertise(sonic1);
  nh.advertise(sonic2);
  nh.advertise(sonic3);
  nh.advertise(sonic4);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin3, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin4, INPUT); // Sets the echoPin as an Input
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
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

  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1/29/2;
  Distance1.data=distance1;
  if(distance1<50){
    digitalWrite(ledPin1, HIGH);
  }
  else{
    digitalWrite(ledPin1, LOW); 
  }
  delay(10);
  
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2/29/2;
  Distance2.data=distance2;
  if(distance2<20){
    digitalWrite(ledPin2, HIGH);
  }
  else{
    digitalWrite(ledPin2, LOW); 
  }
  delay(10);
  
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = duration3/29/2;
  Distance3.data=distance3;
  if(distance3<20){
    digitalWrite(ledPin3, HIGH);
  }
  else{
    digitalWrite(ledPin3, LOW); 
  }
  delay(10);
  
  duration4 = pulseIn(echoPin4, HIGH);
  distance4 = duration4/29/2;
  Distance4.data=distance4;
  if(distance4<50){
    digitalWrite(ledPin4, HIGH);
  }
  else{
    digitalWrite(ledPin4, LOW); 
  }
  delay(10);
  
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
  
  delay(10);
}