
// IOT Railway Automation - ESP32 Source
// 2023 - Nolan Gilmore

#include <Arduino.h>
#include <MFRC522.h>
#include <LoRa.h>
#include <EEPROM.h>

//GPIO Pin Aliases - DO NOT EDIT (unless ya have ta)

#define LORA_NSS 5
#define LORA_RS 27
#define LORA_DIO0 2
#define RFID_NSS 17
#define RFID_RS 15
#define SIGNAL_0 15
#define SIGNAL_1 13
#define LVL_TRIG 12
#define LVL_ECHO 14
#define SW_CTRL 26
#define SW_STATE 25


//Flash Addresses

#define ID_ADDR_0 0x00
#define ID_ADDR_1 0x01
#define SETUP_ADDR 0x02
#define LVL_XING_ADDR 0x03
#define RFID_ADDR 0x04
#define SWITCH_ADDR 0x05

//Command Word numeric associations

#define LP 0
#define OB 1
#define OB_CLEAR 2
#define CS 3
#define TM 4
#define SW 5
#define EX 99

//Exception Numbers

#define NO_SWITCH_EN 5


#define XING_INT 50000 //sets interval for obstacle sensor

//Macros for Signal State
#define CLEAR 2
#define CAUTION 1
#define DANGER 0

MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(RFID_NSS, RFID_RS);
byte nuidPICC[4] = {0, 0, 0, 0};

//Configuration Flags
//Deprecated in favor of autoconfig. Exists to maintain combatibility with non-autoconfig version
#define LORA_EN 1
#define UART_EN 1
#define WIFI_EN 1
#define LVL_XING_EN 1
//#define SW_EN 1
#define I2C_EN 1
//#define AUTO_SETUP 1

//new self configuration flags

#define ENABLE 0xFF
#define DISABLE 0x00

unsigned char lvlXingPresent = 0xFF;
unsigned char swPresent = 0xFF;


#define LOCOMOTIVE 1

//uint16_t nid = 0x2F7A;  //Default Node ID
String nid = "117E";

//Deprecated. For now, Node limited to single switch
uint8_t noSw = 0;     //Number of Switches attached to Node (0-2)

#define TIME_ZERO 0
uint32_t xingTimer = XING_INT; 
uint32_t testLoRaTimer = XING_INT;

//Global Variables

bool obstacle = false;
bool locoInBlock = false;
uint8_t signalState = DANGER;
bool swState[] = {0, 0, 0, 0};
bool init_boot = 1;


//Stores the last exception no. Used when calling sendMessage() to send an exception
unsigned char lastException = 0;

void init_setup();
void loadConfig();
void sendMessage(int msgType);

void setup()
{

  
  SPI.begin();
  Serial.begin(115200);
  Serial.println("Serial Initialized");

  //pinMode(RFID_NSS, OUTPUT);
  //pinMode(RFID_RS, OUTPUT);

  pinMode(SIGNAL_0, OUTPUT);
  pinMode(SIGNAL_1, OUTPUT);

  rfid.PCD_Init();

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


  #ifndef AUTO_SETUP

  #ifdef LVL_XING_EN
  pinMode(LVL_TRIG, OUTPUT);
  pinMode(LVL_ECHO, INPUT);
  pinMode(SW_STATE, INPUT);
  #endif


  

  #ifdef SW_EN
  pinMode(SW_CTRL, OUTPUT);
  pinMode(SW_STATE, INPUT);
  #endif

  #ifdef WIFI_EN
  //Code goes here for possible future WiFi Functionality
  #endif

  #ifdef I2C_EN
  //Code goes here for possible future I2C Functionality
  #endif

  

  #endif

  #ifdef AUTO_SETUP
  //Check if First Boot
  if (EEPROM.read(SETUP_ADDR) & ENABLE)
  {
    loadConfig();
  }
  else
  {
    init_setup();
  }

  #endif

  if (lvlXingPresent & ENABLE)
  {
    pinMode(LVL_TRIG, OUTPUT);
    pinMode(LVL_ECHO, INPUT);
    pinMode(SW_CTRL, OUTPUT);
    pinMode(SW_STATE, INPUT);
  }
  else if (swPresent & ENABLE)
  {
    pinMode(SW_CTRL, OUTPUT);
    pinMode(SW_STATE, INPUT);
  }


}

/*
//Used to setup node during initial boot. 
void init_setup()
{

  Serial.println("Begin First Time Setup");

  init_boot = 0;
  char id_0 = LoRa.random();
  char id_1 = LoRa.random();

  unsigned char lvlXing_set, sw_set;

  Serial.print("Device NID = ");
  Serial.println(nid, HEX);

  nid = id_0 | (id_1 << 4);

  pinMode(LVL_TRIG, OUTPUT);
  pinMode(LVL_ECHO, INPUT);  
  pinMode(SW_CTRL, OUTPUT);
  pinMode(SW_STATE, INPUT);

  //Probes for Level Crossing
  uint32_t rTime = 0;
  digitalWrite(LVL_TRIG, HIGH);
  delayMicroseconds(10); 
  digitalWrite(LVL_TRIG, LOW);
  rTime = pulseIn(LVL_ECHO, HIGH);
  delayMicroseconds(10);

  if (rTime != 0)
  {
    lvlXing_set = ENABLE;
    Serial.println("Level Crossing Operation Set");
  }
  else
  {
    lvlXing_set = DISABLE;
  }

  //Check for Switch Operation
  //Only if level crossing is not present
  if (lvlXing_set != 0)
  {
    digitalWrite(SW_CTRL, HIGH);
    delayMicroseconds(10);
    if (digitalRead(SW_STATE))
    {
       sw_set = ENABLE;
       Serial.println("Switch Enable");
    }
    else sw_set = DISABLE;
    digitalWrite(SW_CTRL, LOW);
  }

  EEPROM.write(ID_ADDR_0, id_0);
  EEPROM.write(ID_ADDR_1, id_1);
  EEPROM.write(SETUP_ADDR, ENABLE);
  EEPROM.write(LVL_XING_ADDR, lvlXing_set);
  EEPROM.write(SWITCH_ADDR, sw_set);
  EEPROM.commit();

  Serial.println("Configuration Written to Flash");

}

//Loads Configuration Data from Flash
void loadConfig()
{

  Serial.print("Loading Configuration from Flash");

  char id_0 = EEPROM.read(ID_ADDR_0);
  char id_1 = EEPROM.read(ID_ADDR_1);

  nid = id_0 | (id_1 << 4);

  Serial.print("Device NID = ");
  Serial.println(nid, HEX);

  lvlXingPresent = EEPROM.read(LVL_XING_ADDR);
  swPresent = EEPROM.read(SWITCH_ADDR);

  Serial.println("Configuration Loaded");
}
*/

void setSwitch()
{
  //Actuates the Switch
  digitalWrite(SW_CTRL, !digitalRead(SW_STATE));
  sendMessage(SW);
}


bool getSwState()
{
  //Reads the state of the switch
  return digitalRead(SW_STATE);
}



#ifdef LVL_XING_EN
void obstacleCheck()
{
    uint32_t rTime = 0;
    digitalWrite(LVL_TRIG, HIGH);
    delayMicroseconds(10); 
    digitalWrite(LVL_TRIG, LOW);
    rTime = pulseIn(LVL_ECHO, HIGH);
    delayMicroseconds(10);
    
    if (rTime / 58.2 < 5 && !obstacle)
    {
      obstacle = true;
      Serial.println("An obstacle is Detected!");
      sendMessage(OB);
    } 
    else if (rTime / 58.2 >= 5 && obstacle)
    {
      obstacle = false;
      sendMessage(OB_CLEAR);
    }
}
#endif

int ToggleSwitch(uint8_t switchNo)
{
  return 0;
}


//Prints the bytes read from the RFID card.
void printDec(byte *buffer, byte bufferSize) {
	for (byte i = 0; i < bufferSize; i++) {
			Serial.print(buffer[i] < 0x10 ? " 0" : " ");
			Serial.print(buffer[i], DEC);
	}
}

//Function to read RFID data from device
void readRFID(void )
{
	////Read RFID card
	for (byte i = 0; i < 6; i++) {
			key.keyByte[i] = 0xFF;
	}
	// Look for new 1 cards
	if ( ! rfid.PICC_IsNewCardPresent())
			return;
	// Verify if the NUID has been readed
	if ( 	!rfid.PICC_ReadCardSerial())
			return;
	// Store NUID into nuidPICC array
	for (byte i = 0; i < 4; i++) {
			nuidPICC[i] = rfid.uid.uidByte[i];
	}
	Serial.print(F("RFID In dec: "));
  //Prints the data read from RFID card
	printDec(rfid.uid.uidByte, rfid.uid.size);
	Serial.println();
	// Halt PICC
	rfid.PICC_HaltA();
	// Stop encryption on PCD
	rfid.PCD_StopCrypto1();
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

  String message = "?1;";
  
  message.concat(nid);

  switch(msgType)
  {
    case LP: //Locomotive Pass
      message.concat(";LP;");
      message.concat(nuidPICC[0]);

      break;
    case OB: //Obstacle on Track
      message.concat(";OB;1");
      break;

    case OB_CLEAR: //Obstacle Clear
      message.concat(";OB;0");
      break;

    case CS: //signal State
      message.concat(";CS;");
      message.concat(signalState);
      break;

    case TM: //Test Message
      message.concat(";TM;XXXX");
    break;

    case SW: //Switch State
      message.concat(";SW;");
      message.concat(getSwState());
      break;

    case EX:
      message.concat(";EX;");
      switch (lastException)
      {
        case NO_SWITCH_EN:
          message.concat("S|1|0005");
          break;

        default:
          message.concat("C|0|0000");
      }

    default: //Unknown Message
      message.concat(";UN;0");
  }

  message.concat(" ");

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


//Sets the signal state from hub
// 10 - Clear
// 01 - Caution
// 00 - DANGER

void setSignal()
{
  switch(signalState)
  {
    case CLEAR:
      digitalWrite(SIGNAL_0, LOW);
      digitalWrite(SIGNAL_1, HIGH);
      break;
    case CAUTION:
      digitalWrite(SIGNAL_0, HIGH);
      digitalWrite(SIGNAL_1, LOW);
      break;
    default:
      digitalWrite(SIGNAL_0, LOW);
      digitalWrite(SIGNAL_1, LOW);
  }

  Serial.print("Signal set to: ");
  Serial.println(signalState);
}





//Read message from Gateway
void readRX()
{
  //Read packet from LoRa buffer
  String LoRaRX = LoRa.readString();
  //temp string used for comparision purposes
  String nidTemp = "";
  nidTemp.concat(nid);
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
    if (msgType == "SS") //Set Signal Command
    {
      if (payload == "2")
      {
        signalState = CLEAR;
      }
      else if (payload == "1")
      {
        signalState = CAUTION;
      }
      else
      {
        signalState = DANGER;
      }

      setSignal();

    }
    else if (msgType == "CS") //Check Signal Status
    {
      sendMessage(CS);
    }
    else if (msgType == "LX") //Locomotive exits Node
    {
      locoInBlock = true;
    }
    else if (msgType == "SW") //read switch state
    {
      if (swPresent & ENABLE)
      {
        sendMessage(SW);
      }
      else
      {
        Serial.println("Error: No Switch Present");
        lastException = NO_SWITCH_EN;
        sendMessage(EX);
      }
      
    }
    else if (msgType == "AS") //actuate switch
    {
      if (swPresent & ENABLE)
      {
        setSwitch();
      }
      else
      {
        Serial.println("Error: No Switch Present");
        lastException = NO_SWITCH_EN;
        sendMessage(EX);
      }
    }


  }

}



void loop()
{

  
  #ifndef AUTO_SETUP
  #ifdef LVL_XING_EN
  if (xingTimer == 0)
  {
    Serial.println("Check for Obstacle");
    obstacleCheck();
    xingTimer = XING_INT;
  }
  #endif

  //readRFID();

  if (nuidPICC[0] != 0 && !locoInBlock)
  {
      sendMessage(LP);
      locoInBlock = true;
  }

  #ifdef LVL_XING_EN
  xingTimer--;
  #endif

  int packetSize = LoRa.parsePacket();
  if (packetSize > 0)
  {
    readRX();
  }
  /*
  if (testLoRaTimer < 1)
  {
    sendMessage(4);
    testLoRaTimer = XING_INT;
  }

  testLoRaTimer--;
  */
  #endif

  #ifdef AUTO_SETUP
  
  if (lvlXingPresent & ENABLE)
  {
    if (xingTimer & TIME_ZERO)
    {
      obstacleCheck();
      xingTimer = XING_INT;
    }
    xingTimer--;
  }

  readRFID();

  if (nuidPICC[0] != 0 && !locoInBlock)
  {
      sendMessage(LP);
      locoInBlock = true;
  }

  int packetSize = LoRa.parsePacket();
  if (packetSize > 0)
  {
    readRX();
  }
  
  if (testLoRaTimer < 1)
  {
    sendMessage(4);
    testLoRaTimer = XING_INT;
  }

  testLoRaTimer--;

  #endif

}

//This code done with no assistance from generative AI