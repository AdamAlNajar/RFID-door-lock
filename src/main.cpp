// PIN OUT ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RST - 22
//MOSI - 23
//MISO - 19
//SCK - 18
//SDA - 21
// SERVO - 13
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SS_PIN 21 
#define RST_PIN 22

#include <MFRC522.h>
#include <ESP32Servo.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
Servo doorServo;

byte ADAM_TAG[]  = {}; // PUT UR OWN TAG, NO NEED TO CLONE MINE 

void setup() 
{
  Serial.begin(9600);
  doorServo.attach(13);
  doorServo.write(0);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.print("Tap Card/Tag...");
}

void loop() 
{
  bool card_match = true;
  if(!mfrc522.PICC_IsNewCardPresent())
  {
    // if no card tapped, wait till one is
    return;
  }

  if(!mfrc522.PICC_ReadCardSerial()){
    return;
  }

  Serial.print("UID TAG :");
  

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);

    if(mfrc522.uid.uidByte[i] != ADAM_TAG[i]){
      card_match = false;
    }
  }

  if(card_match){
    doorServo.write(180);
    Serial.print("ENTRY");
    delay(10000);
    doorServo.write(0);
  }
  else{
    Serial.print("NO ENTRY");
  }
  Serial.println();
  mfrc522.PICC_HaltA();
  delay(1000);
}
