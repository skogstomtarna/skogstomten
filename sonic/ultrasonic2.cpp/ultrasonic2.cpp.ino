#include <ros.h>
#include <std_msgs/Int64.h>

ros::NodeHandle nh;
std_msgs::Int64 Distance5,Distance6,Distance7,Distance8;

ros::Publisher sonic5("sonic5",&Distance5);
ros::Publisher sonic6("sonic6",&Distance6);
ros::Publisher sonic7("sonic7",&Distance7);
ros::Publisher sonic8("sonic8",&Distance8);

// defines pins numbers
const int trigPin = 12;
const int echoPin5 = 11;
const int echoPin6 = 10;
const int echoPin7 = 9;
const int echoPin8 = 8;
const int ledPin1 = 7;
const int ledPin2 = 6;
const int ledPin3 = 5;
const int ledPin4 = 4;

// defines variables
long duration;
int distance;
const int warning = 100;  // Sets the distance of which the LED's react to.

void setup() {
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.advertise(sonic5); // Advertises the topics to ROS
  nh.advertise(sonic6);
  nh.advertise(sonic7);
  nh.advertise(sonic8);
 
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin5, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin6, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin7, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin8, INPUT); // Sets the echoPin as an Input
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
  duration = pulseIn(echoPin5, HIGH, 100000);
  distance = duration/29/2;               // "/29/2" speed of sound travelling back and forth
  Distance5.data=distance;
  if(distance<warning && distance != 0){  // Distance results in "0" if no return signal is recieved.
    digitalWrite(ledPin1, HIGH);
  }
  else{
    digitalWrite(ledPin1, LOW); 
  }
  delay(10);

  //Second sensor
  trigger();
  duration = pulseIn(echoPin6, HIGH, 100000);
  distance = duration/29/2;
  Distance6.data=distance;
  if(distance<warning && distance != 0){
    digitalWrite(ledPin2, HIGH);
  }
  else{
    digitalWrite(ledPin2, LOW); 
  }
  delay(10);

  // Third sensor
  trigger();
  duration = pulseIn(echoPin7, HIGH, 100000);
  distance = duration/29/2;
  Distance7.data=distance;
  if(distance<warning && distance != 0){
    digitalWrite(ledPin3, HIGH);
  }
  else{
    digitalWrite(ledPin3, LOW); 
  }
  delay(10);

  // Fourth sensor
  trigger();
  duration = pulseIn(echoPin8, HIGH, 100000);
  distance = duration/29/2;
  Distance8.data=distance;
  if(distance<warning && distance != 0){
    digitalWrite(ledPin4, HIGH);
  }
  else{
    digitalWrite(ledPin4, LOW); 
  }
  delay(10);
  
  //Publishing data
  sonic5.publish(&Distance5);
  sonic6.publish(&Distance6);
  sonic7.publish(&Distance7);
  sonic8.publish(&Distance8);
  
  nh.spinOnce();
  delay(100);
}
