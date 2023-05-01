#include <Arduino.h>
#include <MFRC522.h>
#include <LoRa.h>

using namespace std;

//#define LORA_RX 1
//#define LORA_TX 1
#define RPI_GATEWAY 1

#define LORA_SS 5
#define LORA_RS 27
#define LORA_DIO 2

#define DATA_LED 22

uint32_t loraEpochRX = 100000;

void setup()
{

    SPI.begin();
    Serial.begin(9600);

    pinMode(DATA_LED, OUTPUT);
    digitalWrite(DATA_LED, LOW);

    LoRa.setPins(LORA_SS, LORA_RS, LORA_DIO);

    if (!LoRa.begin(915E6))
    {
        Serial.println("LoRa Failed to Initialized");
    }
    else Serial.println("LoRa Successfully Initialized");

}

void loop()
{
  #ifdef LORA_TX
  if (loraEpochRX == 0)
  {
    Serial.println("Sending Testing 123");
    LoRa.beginPacket();
    LoRa.print("Testing 123");
    LoRa.endPacket();
    loraEpochRX = 1000000;
  }

  loraEpochRX--;
  #endif

  #ifdef LORA_RX
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0)
  {
    String LoRaData = LoRa.readString();
    Serial.println(LoRaData);
  }
  #endif

  #ifdef RPI_GATEWAY

  //RX Forwarding to Pi

  //get packet size

  int packetSize = LoRa.parsePacket();

  //New message if larger than 0
  if (packetSize > 0)
  {
    digitalWrite(DATA_LED, HIGH);
    //read message
    String LoRaData = LoRa.readString();

    //forward message to Pi
    Serial.print(LoRaData);
    delayMicroseconds(50000);
    digitalWrite(DATA_LED, LOW);
  }
  

  if (loraEpochRX == 0)
  {
    

    //TX from Pi

    //Check for message
    if (Serial.available() > 0)
    {
      digitalWrite(DATA_LED, HIGH);
      char txByte;
      bool complete = false;
      String packet = "";
    
      while(Serial.available() > 0)
      {
        //read from buffer
        txByte = Serial.read();

        if (txByte == ' ') break;
        //concatenate byte to packet
        packet.concat(txByte);
      }
    
      //transmit Packet
      LoRa.beginPacket();
      LoRa.print(packet);
      LoRa.endPacket();

      //return OK to Pi
      //Serial.println("OK");
      //delayMicroseconds(50000);
      //digitalWrite(DATA_LED, LOW);
    }

    loraEpochRX = 100000;

  }

  loraEpochRX--;

  #endif

}