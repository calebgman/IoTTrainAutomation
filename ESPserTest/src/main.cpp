#include <Arduino.h>
#include <LoRa.h>

#define LORA_NSS 27
#define LORA_RS 5
#define LORA_DIO0 2

int timer = 100000;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial Initialized");

  pinMode(LORA_NSS, OUTPUT);
  pinMode(LORA_RS, OUTPUT);
  pinMode(LORA_DIO0, OUTPUT);

  LoRa.setPins(LORA_NSS, LORA_RS, LORA_DIO0);
  if(!LoRa.begin(915E6))
  {
    Serial.println("LoRa Failed to Activate");
  }
  else
  {
    Serial.println("LoRa Activated");
  }
}

void loop() {
  if (timer == 0)
  {
    if (Serial.available() > 0)
    { 
      char txByte;
      bool complete = false;
      String packet = "";
    
      while(Serial.available() > 0)
      {
        //read from buffer
        txByte = Serial.read();


        //concatenate byte to packet
        packet.concat(txByte);
      }

    Serial.println(packet);

    LoRa.beginPacket();
    LoRa.print(packet);
    LoRa.endPacket();
    
    }
    timer = 100000;
  }
  timer--;
}