//IOT Railway - Locomotive Node Source Code
// 2023 - Nolan Gilmore

#include <Arduino.h>
#include <LoRa.h>

//GPIO Pin Aliases - DO NOT EDIT (unless ya have ta)

#define LORA_NSS 27
#define LORA_RS 5
#define LORA_DIO0 2
#define RFID_NSS 17
#define RFID_RS 16
#define SIGNAL_0 15
#define SIGNAL_1 13
#define LVL_TRIG 12
#define LVL_ECHO 14
#define SW_CTRL 26
#define SW_STATE 25



//Command Word numeric associations - also refrain from editing (except with Nolan's permission)

#define LP 0
#define OB 1
#define OB_CLEAR 2
#define CS 3
#define TM 4
#define SW 5
#define LS 6 //Locomotive Status
#define LC 7 //Locomotive Control

#define EPOCH 100000

//Locomotive Control Vars
int locoID = 0x05;

int speed = 0;
int brakes = 0;
int blockLocation = 0;

int statusEpoch = 100000;

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

// Prepares Message to Send to Pi
// Format (direction;nodeID;Command Word;Payload)
//direction    - 1 Byte  - Message to or from Gateway (0: to Gateway, 1: to Node)
//nodeID       - 2 Bytes - ID of sending/recieving node
//Command Word - 2 Bytes - the Command Word
//Payload      - 1 to 247 Bytes - the payload
//; Separator for parsing 
void sendMessage(int msgType)
{

  Serial.println("Begin TX");

  String message = "1;";
  message.concat(locoID);

  switch(msgType)
  {
    case LS:
      message.concat(speed);
      message.concat("|");
      message.concat(brakes);
      message.concat("|");
      message.concat(blockLocation);
    break;

    default: //Unknown Message
      message.concat(";UN;0");
  }

  Serial.print("Message to Send: ");
  Serial.println(message);

  //Send Message to Pi
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

}

//Code found on the internet for dividing arduino Strings by separator character
//Would rather use C strings but LoRa.h doesn't work with them so this will
//have to suffice
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

//Read message from Gateway
void readRX()
{
  //Read packet from LoRa buffer
  String LoRaRX = LoRa.readString();
  //temp string used for comparision purposes
  String nidTemp = "";
  nidTemp.concat(locoID);
  //Print the recieved message to console
  Serial.print("Received Message: ");
  Serial.println(LoRaRX);

  //Parse Node ID value from packet
  String rxNid = getValue(LoRaRX, ';', 1);

  //Check if message for this Node
  if (rxNid == nidTemp && getValue(LoRaRX, ';', 0) == "0")
  {
    //Parse Command Word and payload value
    String msgType = getValue(LoRaRX, ';', 2);
    String payload = getValue(LoRaRX, ';', 3);

    //Check Command Word
    //would rather use case statement but not sure compatible with
    //arduino Strings
    if (msgType == "LC") //Set Signal Command
    {
      
    }
  }

}




void loop()
{
  if (statusEpoch <= 0)
  {
    sendMessage(LS);
    statusEpoch = EPOCH;
  }
  statusEpoch--;
  readRX();
}