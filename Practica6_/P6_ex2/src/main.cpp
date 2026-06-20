#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN 10
#define RST_PIN 9


MFRC522 mfrc522(SS_PIN, RST_PIN);


void setup() {
 Serial.begin(115200);
 delay(1000);


 SPI.begin(37, 38, 36, 10);
 mfrc522.PCD_Init();


 Serial.println("Test RC522");
 mfrc522.PCD_DumpVersionToSerial();
 Serial.println("Apropa una targeta...");
}


void loop() {
 if (!mfrc522.PICC_IsNewCardPresent()) return;
 if (!mfrc522.PICC_ReadCardSerial()) return;


 Serial.print("Card UID:");
 for (byte i = 0; i < mfrc522.uid.size; i++) {
   Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
   Serial.print(mfrc522.uid.uidByte[i], HEX);
 }
 Serial.println();


 mfrc522.PICC_HaltA();
}
