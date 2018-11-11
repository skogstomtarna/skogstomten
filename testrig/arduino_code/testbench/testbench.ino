// pinout for MEGA ADK
int dirpin = 48; // O motor enable
int enbpin = 49; // O motor direction
int spdpin = 69; // O motor speed ANALOG
int fb1pin = 60; // I motor feedback 1
int fb2pin = 61; // I motor feedback 2

int rpm = 0;

void setup()
{
  pinMode(dirpin, OUTPUT);
  pinMode(enbpin, OUTPUT);
  //  pinMode(spdpin, OUTPUT);
  //  pinMode(fb1pin, INPUT);
  //  pinMode(fb2pin, INPUT);
}

void loop()
{
  rpm = (rpm+1)%256;
  digitalWrite(dirpin, HIGH);
  digitalWrite(enbpin, HIGH);
  analogWrite(spdpin, 255);
  //delay(1000);
  //digitalWrite(enbpin, LOW);
  delay(500);
}

