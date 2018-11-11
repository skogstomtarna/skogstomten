#include <stdio.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

// OLED pins
#define OLED_CLK   12
#define OLED_MOSI  11
#define OLED_RESET 10
#define OLED_DC    9
#define OLED_CS    8

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

int dT = 10;
char text[168];
int lineCounter = 0;
char line[21];
char incomingByte;
/* motor ctrl
 int dirpin = 48; // O motor enable
 int enbpin = 49; // O motor direction
 int spdpin = 69; // O motor speed ANALOG
 int fb1pin = 60; // I motor feedback 1
 int fb2pin = 61; // I motor feedback 2

 int rpm = 0;
 */

void lineCopy()
{
  int width = 21;
  int height = 8;

  // for each character in line
  for(int i=0; i<width; i++)
  {
    // for each line, copy char from next line
    for(int j=0; j<height-1; j++)
    {
      text[i+j*width] = text[i+(j+1)*width];
    }
    // copy char from input line to last line
    text[width*(height-1) + i] = line[i];
    // flush line
    line[i] = 32;
  }
}


void dispUpdate()
{
  display.clearDisplay();
  display.setCursor(0,0);
  lineCopy();
  display.println(text);
  display.display();
}

void setup()
{
  Serial.begin(9600);
  Serial1.begin(57600);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();
  /* motor ctrl
   pinMode(dirpin, OUTPUT);
   pinMode(enbpin, OUTPUT);
   */
}

int lc = 0;
void loop()
{
  /* motor ctrl
  rpm = (rpm+1)%256;
  digitalWrite(dirpin, HIGH);
  digitalWrite(enbpin, HIGH);
  analogWrite(spdpin, 255);
  delay(500);
  */
  while(Serial.available() > 0)
  {
    incomingByte = Serial.read();
    if(incomingByte==10 || incomingByte==13)
    {
      lc = 0;
      dispUpdate();
      break;
    }
    if(lc<21){ line[lc] = incomingByte; }
    lc++;
  }
  delay(dT);
}
