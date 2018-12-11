#include <ros.h>
#include <std_msgs/Int64.h>

ros::NodeHandle nh;

// Define the type of the ROS topics
std_msgs::Int64 Distance1,Distance2,Distance3,Distance4;

// Define the ROS topics
ros::Publisher sonic1("sonic1",&Distance1);
ros::Publisher sonic2("sonic2",&Distance2);
ros::Publisher sonic3("sonic3",&Distance3);
ros::Publisher sonic4("sonic4",&Distance4);

// Define pin numbers
const int trigPin = 12;
const int echoPin1 = 11;
const int echoPin2 = 10;
const int echoPin3 = 9;
const int echoPin4 = 8;
const int ledPin1 = 7;
const int ledPin2 = 6;
const int ledPin3 = 5;
const int ledPin4 = 4;

// Define variables
long duration;
int distance;
const int fbWarning = 100;   // Sets the front and back warning distance
const int sideWarning = 30;  // Sets the side warning distance

void setup() {
  nh.getHardware()->setBaud(57600);
  nh.initNode();

  // Advertises the topics to ROS
  nh.advertise(sonic1);
  nh.advertise(sonic2);
  nh.advertise(sonic3);
  nh.advertise(sonic4);

  // Sets the pins to either Output or Input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  pinMode(echoPin4, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  Serial.begin(57600); // Starts the serial communication
}

// Function to activate the trigger pulse
void trigger(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

// Reset function if any sensor get stuck on "0"
void reset_hcsr04() {
  pinMode(echoPin1,OUTPUT);
  digitalWrite(echoPin1,LOW);
  delay(1);
  pinMode(echoPin1,INPUT);

  pinMode(echoPin2,OUTPUT);
  digitalWrite(echoPin2,LOW);
  delay(1);
  pinMode(echoPin2,INPUT);
  
  pinMode(echoPin3,OUTPUT);
  digitalWrite(echoPin3,LOW);
  delay(1);
  pinMode(echoPin3,INPUT);
  
  pinMode(echoPin4,OUTPUT);
  digitalWrite(echoPin4,LOW);
  delay(1);
  pinMode(echoPin4,INPUT);
}

void loop() {
  // pulseIn is a blocking function, thus we need to run each sensor one at a time.
  
  // 1st sensor
  trigger();
  duration = pulseIn(echoPin1, HIGH, 100000);
  distance = duration/29/2;                   // "duration/29/2" to calculate distance from the speed of sound travelling back and forth
  Distance1.data=distance;                    // Assign the calculated distance to the correct topic
  if(distance<sideWarning && distance != 0){  // Lighting up the LED if within a certain distance. Distance results in "0" if no return signal is recieved.
    digitalWrite(ledPin1, HIGH);
  }
  else{
    digitalWrite(ledPin1, LOW); 
  }
  if (duration == 0) {
    reset_hcsr04();
  }
  delay(10);

  // 2nd sensor
  trigger();
  duration = pulseIn(echoPin2, HIGH, 100000);
  distance = duration/29/2;
  Distance2.data=distance;
  if(distance<fbWarning && distance != 0){
    digitalWrite(ledPin2, HIGH);
  }
  else{
    digitalWrite(ledPin2, LOW); 
  }
  if (duration == 0) {
    reset_hcsr04();
  }
  delay(10);

  // 3rd sensor
  trigger();
  duration = pulseIn(echoPin3, HIGH, 100000);
  distance = duration/29/2;
  Distance3.data=distance;
  if(distance<fbWarning && distance != 0){
    digitalWrite(ledPin3, HIGH);
  }
  else{
    digitalWrite(ledPin3, LOW); 
  }
  if (duration == 0) {
    reset_hcsr04();
  }
  delay(10);

  // 4th sensor
  trigger();
  duration = pulseIn(echoPin4, HIGH, 100000);
  distance = duration/29/2;
  Distance4.data=distance;
  if(distance<sideWarning && distance != 0){
    digitalWrite(ledPin4, HIGH);
  }
  else{
    digitalWrite(ledPin4, LOW); 
  }
  if (duration == 0) {
    reset_hcsr04();
  }
  delay(10);

  // Publishing topics
  sonic1.publish(&Distance1);
  sonic2.publish(&Distance2);
  sonic3.publish(&Distance3);
  sonic4.publish(&Distance4);
  
  nh.spinOnce();
  delay(100);
}
