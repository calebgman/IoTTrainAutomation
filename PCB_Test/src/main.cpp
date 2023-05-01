#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

long count = 0;

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Message No. ");
  Serial.println(count);
  delayMicroseconds(10000);
  count++;

}