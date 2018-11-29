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

void setup() {
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.advertise(sonic5);
  nh.advertise(sonic6);
  nh.advertise(sonic7);
  nh.advertise(sonic8);
 
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin5, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin6, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin7, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin8, INPUT); // Sets the echoPin as an Input
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  Serial.begin(57600); // Starts the serial communication
}

void loop() {
  
  // Reads the echoPin, returns the sound wave travel time in microseconds

  for(int i=4;i<8;i++){
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    switch(i){
      case 4:
        duration = pulseIn(echoPin5, HIGH);
        distance = duration/29/2;
        Distance5.data=distance;
        if(distance<50){
          digitalWrite(ledPin1, HIGH);
        }
        else{
          digitalWrite(ledPin1, LOW); 
        }
        //delay(10);
      break;
      case 5:
        duration = pulseIn(echoPin6, HIGH);
        distance = duration/29/2;
        Distance6.data=distance;
        if(distance<50){
          digitalWrite(ledPin2, HIGH);
        }
        else{
          digitalWrite(ledPin2, LOW); 
        }
        //delay(10);
      break;
      case 6:
        duration = pulseIn(echoPin7, HIGH);
        distance = duration/29/2;
        Distance7.data=distance;
        if(distance<50){
          digitalWrite(ledPin3, HIGH);
        }
        else{
          digitalWrite(ledPin3, LOW); 
        }
        //delay(10);
      break;
      case 7:
        duration = pulseIn(echoPin8, HIGH);
        distance = duration/29/2;
        Distance8.data=distance;
        if(distance<50){
          digitalWrite(ledPin4, HIGH);
        }
        else{
          digitalWrite(ledPin4, LOW); 
        }
        //delay(10);
        break;
    }
    //delay(10);
  }
  
  //Publishing data
  sonic5.publish(&Distance5);
  //delay(10);
  sonic6.publish(&Distance6);
  //delay(10);
  sonic7.publish(&Distance7);
  //delay(10);
  sonic8.publish(&Distance8);
  //delay(10);
  
  nh.spinOnce();
  
  delay(100);
}
