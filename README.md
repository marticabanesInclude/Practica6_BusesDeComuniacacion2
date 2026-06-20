# PRÀCTICA 6: Buses de comunicació II - SPI

**Alumnes:** Martí Cabanes i Adriana Bosch

---

## Objectiu

L’objectiu d’aquesta pràctica és entendre el funcionament del bus SPI amb l’ESP32.

El bus SPI permet comunicar un dispositiu mestre, en aquest cas l’ESP32, amb un o més perifèrics esclaus. Per fer-ho utilitza principalment les línies:

```text
MOSI → dades del mestre cap a l’esclau
MISO → dades de l’esclau cap al mestre
SCK  → senyal de rellotge
CS/SS → selecció del dispositiu esclau
```

En aquesta pràctica s’han treballat tres exercicis:

- Exercici 1: lectura d’una targeta SD mitjançant SPI.
- Exercici 2: lectura d’una etiqueta RFID amb el mòdul RC522.
- Exercici 3: exercici de casa utilitzant un perifèric SPI disponible.

---

# Exercici 1: Lectura de memòria SD

## Funcionament

En aquest exercici s’ha intentat inicialitzar un mòdul lector de targetes SD utilitzant el bus SPI. L’objectiu era obrir un fitxer anomenat `archivo.txt` i mostrar-ne el contingut pel monitor sèrie.

El programa primer intenta inicialitzar la targeta SD. Si la inicialització és correcta, obre el fitxer i llegeix el seu contingut caràcter a caràcter. Si la inicialització falla, mostra un missatge d’error pel port sèrie.

---

## Connexió utilitzada

La connexió prevista per al mòdul SD era:

```text
Mòdul SD       ESP32-S3

VCC      →     5V / 3V3
GND      →     GND
MOSI     →     GPIO11
MISO     →     GPIO13
SCK      →     GPIO12
CS       →     GPIO10
```

També es van provar altres configuracions de CS, però la targeta SD no va arribar a inicialitzar-se correctament.

---

## Sortida pel monitor sèrie

La sortida obtinguda pel monitor sèrie va ser:

```text
Iniciando SD...
f_mount failed: (3) The physical drive cannot work
sdSelectCard(): Select Failed
No se pudo inicializar la tarjeta SD
Revisa conexiones, alimentacion, formato FAT32 y pin CS
```

Captura del monitor sèrie:

![Monitor sèrie Exercici 1](Imatges_P6/monitor_ex1_sd.png)

---

## Explicació del resultat

El missatge `sdSelectCard(): Select Failed` indica que l’ESP32 no va aconseguir comunicar-se correctament amb la targeta SD pel bus SPI.

Això pot estar relacionat amb:

- Problemes de cablejat.
- Pin CS incorrecte.
- Alimentació insuficient del mòdul SD.
- Targeta SD no compatible o mal formatada.
- Contacte incorrecte amb el lector.

Per tant, el codi s’executava correctament, però el perifèric SD no va respondre correctament.

---

## Codi generat

El codi complet de l’exercici es troba a:

```text
Exercici_1/src/main.cpp
```

El programa utilitza les llibreries:

```cpp
#include <SPI.h>
#include <SD.h>
```

Aquestes llibreries permeten inicialitzar el bus SPI i accedir a la targeta SD.

---

# Exercici 2: Lectura d’etiqueta RFID

## Funcionament

En aquest exercici s’ha utilitzat el lector RFID RC522, que també funciona mitjançant el bus SPI.

El programa inicialitza el lector RFID i espera que s’apropi una targeta. Quan detecta una targeta, llegeix el seu identificador UID i el mostra pel monitor sèrie.

---

## Connexió utilitzada

La connexió del lector RC522 va ser:

```text
RC522        ESP32-S3

SDA / SS →   GPIO10
SCK      →   GPIO37
MOSI     →   GPIO36
MISO     →   GPIO38
RST      →   GPIO9
3.3V     →   3V3
GND      →   GND
```

El pin `SDA` del RC522 en aquest cas actua com a `SS/CS` del bus SPI. No és el bus I2C.

---

## Sortida pel monitor sèrie

En aquest exercici sí que es va aconseguir llegir una targeta RFID correctament.

La sortida obtinguda va ser:

```text
Test RC522
Firmware Version: 0xB2 = (unknown)
Apropa una targeta...
Card UID: 05 E7 F7 04
```

Captura del monitor sèrie:

![Monitor sèrie Exercici 2](Imatges_P6/monitor_ex2_rfid.png)

---

## Explicació del resultat

El missatge:

```text
Card UID: 05 E7 F7 04
```

confirma que el lector RC522 va detectar correctament una targeta RFID i va llegir el seu UID.

Encara que el firmware aparegui com `unknown`, el lector funciona perquè el programa és capaç de llegir el codi de la targeta.

---

## Codi generat

El codi complet de l’exercici es troba a:

```text
Exercici_2/src/main.cpp
```

El programa utilitza les llibreries:

```cpp
#include <SPI.h>
#include <MFRC522.h>
```

La llibreria `MFRC522` permet inicialitzar el lector RFID, detectar targetes i llegir el UID.

---

## Explicació del codi

Primer s’inicialitza la comunicació sèrie:

```cpp
Serial.begin(115200);
```

Després s’inicia el bus SPI amb els pins utilitzats:

```cpp
SPI.begin(37, 38, 36, 10);
```

A continuació s’inicialitza el lector RFID:

```cpp
mfrc522.PCD_Init();
```

Dins del `loop()`, el programa comprova si hi ha una targeta present:

```cpp
mfrc522.PICC_IsNewCardPresent()
```

Si hi ha una targeta, intenta llegir-la:

```cpp
mfrc522.PICC_ReadCardSerial()
```

Finalment, imprimeix el UID de la targeta pel monitor sèrie.

---

# Exercici 3: Exercici de casa amb perifèric SPI

## Funcionament

Per a l’exercici de casa s’ha utilitzat el perifèric SPI disponible, el lector RFID RC522.

L’objectiu era fer una versió ampliada de la lectura RFID, mostrant més informació pel monitor sèrie i adaptant el cablejat als pins soldats disponibles de la nostra ESP32-S3.

Els pins soldats disponibles eren:

```text
GPIO19, GPIO47, GPIO38, GPIO10, GPIO3, GPIO15
```

Per aquest motiu, es va adaptar el codi per utilitzar aquests pins.

---

## Connexió utilitzada

La connexió plantejada per l’Exercici 3 va ser:

```text
RC522        ESP32-S3

SDA / SS →   GPIO10
SCK      →   GPIO47
MOSI     →   GPIO3
MISO     →   GPIO38
RST      →   GPIO15
3.3V     →   3V3
GND      →   GND
```

---

## Foto del muntatge

Foto del muntatge realitzat:

![Muntatge Exercici 3](Imatges_P6/muntatge_ex3.jpg)

---

## Sortida de depuració

En executar el programa, la sortida obtinguda va ser:

```text
Exercici 3 - Lectura RFID amb SPI
Pins utilitzats:
SDA/SS -> GPIO10
SCK    -> GPIO47
MOSI   -> GPIO3
MISO   -> GPIO38
RST    -> GPIO15
Inicialitzant bus SPI...
Inicialitzant lector RC522...
Informacio del lector:
Firmware Version: 0x0 = (unknown)
WARNING: Communication failure, is the MFRC522 properly connected?
Sistema preparat.
Apropa una targeta RFID al lector...
```

Captura del monitor sèrie:

![Monitor sèrie Exercici 3](Imatges_P6/monitor_ex3.png)

---

## Explicació del resultat

En aquest cas, el lector RC522 rebia alimentació, ja que s’encenia el LED vermell del mòdul. Tot i això, la comunicació SPI no es va establir correctament.

El missatge:

```text
Firmware Version: 0x0 = (unknown)
WARNING: Communication failure
```

indica que l’ESP32 no va rebre una resposta correcta del lector RC522.

El problema probablement està relacionat amb el cablejat o amb el fet que alguns pins de la placa ESP32-S3 no estaven soldats. Per això es va intentar adaptar el programa als GPIO disponibles.

Tot i no obtenir una lectura correcta en aquest exercici, s’ha documentat el muntatge, la sortida de depuració, el codi generat i l’explicació del funcionament, tal com es demana a l’enunciat.

---

## Codi generat

El codi complet de l’exercici es troba a:

```text
Exercici_3/src/main.cpp
```

El programa utilitza el bus SPI i la llibreria `MFRC522` per intentar comunicar-se amb el lector RFID.

---

## Explicació del codi

El programa defineix els pins SPI utilitzats:

```cpp
#define RFID_SS    10
#define RFID_RST   15
#define RFID_SCK   47
#define RFID_MOSI  3
#define RFID_MISO  38
```

Després inicialitza el bus SPI:

```cpp
SPI.begin(RFID_SCK, RFID_MISO, RFID_MOSI, RFID_SS);
```

A continuació inicialitza el lector RC522:

```cpp
mfrc522.PCD_Init();
```

El programa queda esperant una targeta RFID. Si la comunicació amb el lector fos correcta, quan s’apropés una targeta es mostraria el seu UID pel monitor sèrie.

---

# Conclusions

En aquesta pràctica s’ha treballat amb el bus SPI de l’ESP32-S3.

En l’Exercici 1 s’ha intentat inicialitzar i llegir una targeta SD. Tot i que el codi s’executava, el mòdul SD no es va inicialitzar correctament.

En l’Exercici 2 s’ha aconseguit utilitzar correctament el lector RFID RC522 i llegir el UID d’una targeta.

En l’Exercici 3 s’ha realitzat un muntatge addicional amb el lector RC522, adaptant el codi als pins soldats disponibles de la placa. Tot i que la comunicació no es va completar correctament, s’han obtingut sortides de depuració útils per identificar el problema.

La pràctica ha servit per entendre millor el funcionament del bus SPI i la importància del cablejat correcte, el pin CS i la compatibilitat dels pins utilitzats.
