#include <Arduino.h>
#include <LoRa.h>

#define LORA_NSS 5
#define LORA_RS 27
#define LORA_DIO0 2
#define RFID_NSS 17
#define RFID_RS 16
#define SIGNAL_0 15
#define SIGNAL_1 13
#define LVL_TRIG 12
#define LVL_ECHO 14
#define SW_CTRL 26
#define SW_STATE 25

int loraTime = 10000;

void setup() {
  SPI.begin();
  Serial.begin(115200);
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


//Read message from Gateway
void readRX()
{
  //Read packet from LoRa buffer
  String LoRaRX = LoRa.readString();
  //temp string used for comparision purposes
  Serial.print(LoRaRX);
  Serial.println("");
}


void loop() {

int packetSize = LoRa.parsePacket();

  if (packetSize > 0)
  {
    readRX();
  }


}