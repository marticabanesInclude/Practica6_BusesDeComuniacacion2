#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

// Pins disponibles soldats a la nostra ESP32-S3
#define RFID_SS    9   // SDA / SS del RC522
#define RFID_RST   7   // RST del RC522
#define RFID_SCK   48   // SCK del RC522
#define RFID_MOSI  8   // MOSI del RC522
#define RFID_MISO  39   // MISO del RC522

MFRC522 mfrc522(RFID_SS, RFID_RST);

int contadorLecturas = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Exercici 3 - Lectura RFID amb SPI");
  Serial.println("Pins utilitzats:");
  Serial.println("SDA/SS -> GPIO10");
  Serial.println("SCK    -> GPIO47");
  Serial.println("MOSI   -> GPIO19");
  Serial.println("MISO   -> GPIO38");
  Serial.println("RST    -> GPIO15");

  Serial.println("Inicialitzant bus SPI...");
  SPI.begin(RFID_SCK, RFID_MISO, RFID_MOSI, RFID_SS);

  Serial.println("Inicialitzant lector RC522...");
  mfrc522.PCD_Init();

  Serial.println("Informacio del lector:");
  mfrc522.PCD_DumpVersionToSerial();

  Serial.println("Sistema preparat.");
  Serial.println("Apropa una targeta RFID al lector...");
}

void loop() {
  // Si no hi ha targeta nova, no fem res
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Si no es pot llegir la targeta, sortim
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  contadorLecturas++;

  Serial.println();
  Serial.println("Targeta detectada!");
  Serial.print("Lectura numero: ");
  Serial.println(contadorLecturas);

  Serial.print("Card UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }

  Serial.println();

  Serial.print("Mida UID: ");
  Serial.print(mfrc522.uid.size);
  Serial.println(" bytes");

  Serial.println("Lectura finalitzada.");
  Serial.println("Apropa una altra targeta...");

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(1000);
}