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
long duration;
int distance;
const int warning = 100;  // Sets the distance of which the LED's react to

void setup() {
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.advertise(sonic1); // Advertises the topics to ROS
  nh.advertise(sonic2);
  nh.advertise(sonic3);
  nh.advertise(sonic4);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin1 as an Input
  pinMode(echoPin2, INPUT); // Sets the echoPin2 as an Input
  pinMode(echoPin3, INPUT); // Sets the echoPin3 as an Input
  pinMode(echoPin4, INPUT); // Sets the echoPin4 as an Input
  pinMode(ledPin1, OUTPUT); // Sets the ledPin1 as an Output
  pinMode(ledPin2, OUTPUT); // Sets the ledPin2 as an Output
  pinMode(ledPin3, OUTPUT); // Sets the ledPin3 as an Output
  pinMode(ledPin4, OUTPUT); // Sets the ledPin4 as an Output
  Serial.begin(57600); // Starts the serial communication
}

// Function to activate the trigger pulse
void trigger(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

void loop() {
  // pulseIn is a blocking function, thus we need to run each sensor one at a time.
  
  // First sensor
  trigger();
  duration = pulseIn(echoPin1, HIGH, 100000);
  distance = duration/29/2;               // "/29/2" speed of sound travelling back and forth
  Distance1.data=distance;
  if(distance<warning && distance != 0){  // Distance results in "0" if no return signal is recieved.
    digitalWrite(ledPin1, HIGH);
  }
  else{
    digitalWrite(ledPin1, LOW); 
  }
  delay(10);

  //Second sensor
  trigger();
  duration = pulseIn(echoPin2, HIGH, 100000);
  distance = duration/29/2;
  Distance2.data=distance;
  if(distance<warning && distance != 0){
    digitalWrite(ledPin2, HIGH);
  }
  else{
    digitalWrite(ledPin2, LOW); 
  }
  delay(10);

  // Third sensor
  trigger();
  duration = pulseIn(echoPin3, HIGH, 100000);
  distance = duration/29/2;
  Distance3.data=distance;
  if(distance<warning && distance != 0){
    digitalWrite(ledPin3, HIGH);
  }
  else{
    digitalWrite(ledPin3, LOW); 
  }
  delay(10);

  // Fourth sensor
  trigger();
  duration = pulseIn(echoPin4, HIGH, 100000);
  distance = duration/29/2;
  Distance4.data=distance;
  if(distance<warning && distance != 0){
    digitalWrite(ledPin4, HIGH);
  }
  else{
    digitalWrite(ledPin4, LOW); 
  }
  delay(10);

  // Publishing data
  sonic1.publish(&Distance1);
  sonic2.publish(&Distance2);
  sonic3.publish(&Distance3);
  sonic4.publish(&Distance4);
  
  nh.spinOnce();
  delay(100);
}
