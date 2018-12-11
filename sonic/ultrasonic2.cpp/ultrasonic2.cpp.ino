#include <ros.h>
#include <std_msgs/Int64.h>

ros::NodeHandle nh;
std_msgs::Int64 Distance5,Distance6,Distance7,Distance8;

ros::Publisher sonic5("sonic5",&Distance5);
ros::Publisher sonic6("sonic6",&Distance6);
ros::Publisher sonic7("sonic7",&Distance7);
ros::Publisher sonic8("sonic8",&Distance8);

const int trigPin = 12;
const int echoPin5 = 11;
const int echoPin6 = 10;
const int echoPin7 = 9;
const int echoPin8 = 8;
const int ledPin1 = 7;
const int ledPin2 = 6;
const int ledPin3 = 5;
const int ledPin4 = 4;

long duration;
int distance;

const int bWarning = 30;

const int sideWarning = 30;

void setup() {
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.advertise(sonic5);
  nh.advertise(sonic6);
  nh.advertise(sonic7);
  nh.advertise(sonic8);
 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin5, INPUT);
  pinMode(echoPin6, INPUT);
  pinMode(echoPin7, INPUT);
  pinMode(echoPin8, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  Serial.begin(57600);
}

void trigger(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}


/*void reset_hcsr04() {

  pinMode(echoPin5,OUTPUT);
  digitalWrite(echoPin5,LOW);
  delay(1);
  pinMode(echoPin5,INPUT);

  pinMode(echoPin6,OUTPUT);
  digitalWrite(echoPin6,LOW);
  delay(1);
  pinMode(echoPin6,INPUT);
  
  pinMode(echoPin7,OUTPUT);
  digitalWrite(echoPin7,LOW);
  delay(1);
  pinMode(echoPin7,INPUT);
  
  pinMode(echoPin8,OUTPUT);
  digitalWrite(echoPin8,LOW);
  delay(1);
  pinMode(echoPin8,INPUT);

}*/


void loop() {
  // 5th sensor
  trigger();
  duration = pulseIn(echoPin5, HIGH, 100000);
  distance = duration/29/2;
  Distance5.data=distance;
  if(distance<sideWarning && distance != 0){
    digitalWrite(ledPin1, HIGH);
  }
  else{
    digitalWrite(ledPin1, LOW); 
  }

  //if (duration == 0) {
  //  reset_hcsr04();
  //}
  delay(100);

  //6th sensor
  trigger();
  duration = pulseIn(echoPin6, HIGH, 100000);
  distance = duration/29/2;
  Distance6.data=distance;

  if(distance<bWarning && distance != 0){
    digitalWrite(ledPin2, HIGH);
  }
  else{
    digitalWrite(ledPin2, LOW); 
  }
  //if (duration == 0) {
  //  reset_hcsr04();
  //}
  delay(100);

  // 7th sensor
  trigger();
  duration = pulseIn(echoPin7, HIGH, 100000);
  distance = duration/29/2;
  Distance7.data=distance;
  if(distance<bWarning && distance != 0){
    digitalWrite(ledPin3, HIGH);
  }
  else{
    digitalWrite(ledPin3, LOW); 
  }
  //if (duration == 0) {
  //  reset_hcsr04();
  //}
  delay(100);

  // 8th sensor
  trigger();
  duration = pulseIn(echoPin8, HIGH, 100000);
  distance = duration/29/2;
  Distance8.data=distance;
  if(distance<sideWarning && distance != 0){
    digitalWrite(ledPin4, HIGH);
  }
  else{
    digitalWrite(ledPin4, LOW); 
  }
  //if (duration == 0) {
  //  reset_hcsr04();
  //}
  delay(100);
  
  sonic5.publish(&Distance5);
  sonic6.publish(&Distance6);
  sonic7.publish(&Distance7);
  sonic8.publish(&Distance8);
  
  nh.spinOnce();
  delay(100);
}
