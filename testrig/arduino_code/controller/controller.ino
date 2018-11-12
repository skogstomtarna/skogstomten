#include <ros.h>
#include <std_msgs/Int8.h>
#include <stdio.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>


// OLED pins
#define OLED_CLK    12
#define OLED_MOSI   11
#define OLED_RESET  10
#define OLED_DC     9
#define OLED_CS     8

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// motor pins: enable 1 2 3 4, dir 1 2 3 4
int pin_numbers[] = {14,15,16,17,2,3,4,5};
char pin_data[] = {0,0,0,0,0,0,0,0};

// time vars
int last_callback = 0; // timestamp for last callback
int timeout = 1000; // millis to timeout (->idle state)
int loop_rate = 100;

void updateDisplay()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  // make title
  display.setCursor(8,4);
  display.print("MOTOR CTRL by Misza");
  display.setCursor(45,12);
  display.println("v0.5.0");
  display.setCursor(13,39);
  display.print("en");
  display.setCursor(7,51);
  display.print("dir");

  for(int i=0; i<4; i++)
  {
    display.setTextColor(WHITE);
    display.setCursor(36+i*23,27);
    display.print("M");
    display.print(i+1);
    display.setCursor(41+i*23,39);
    display.print(pin_data[i]+0);
    display.setCursor(41+i*23,51);
    display.print(pin_data[i+4]+0);
  }
  // frame
  display.drawLine(1,1,126,1,2);
  display.drawLine(126,1,126,62,2);
  display.drawLine(126,62,1,62,2);
  display.drawLine(1,62,1,1,2);
  display.display();
}

void motor_callback( const std_msgs::Int8& action_msg )
{
  // unpack_msg and save new timestamp
  int in_data = action_msg.data;
  for(int i=0; i<8; i++)
  {
    pin_data[7-i] = in_data%2;
    in_data = in_data>>1;
  }
  last_callback = millis();
}

ros::NodeHandle nh;
ros::Subscriber<std_msgs::Int8> pin_sub("pins", motor_callback);

void setMotors()
{
  for(int i=0; i<8; i++)
  {
    digitalWrite(pin_numbers[i],pin_data[i]);
  }
}

void setup()
{
  nh.initNode();
  nh.subscribe(pin_sub);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.display();
  // pin init macro
  for(int i=0; i<8; i++)
  {
    pinMode(pin_numbers[i],OUTPUT);
  }
}

int ledPin = 1;

void loop()
{
  // failsafe to reset data if too long time has passed since last cb
  if(millis()-last_callback > timeout)
  {
    for(int i=0; i<8; i++)
    {
      pin_data[i] = 0;
    }
  }
  setMotors();
  updateDisplay();
  delay(loop_rate);
  nh.spinOnce();
}
