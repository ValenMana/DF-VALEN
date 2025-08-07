#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 13
#define RST_PIN 5

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

// Variable to store the scanned card ID as a string
String cardID = "";

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scans the MIFARE Classic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}

void loop() {

  // Check if a serial input is available and read it
  if (Serial.available()) {
    int input = Serial.parseInt();  // Read the integer input from Serial
    if (input == -1) {
      // If input is -1, restart the microcontroller
      Serial.println(F("Received -1, restarting..."));
      reset();  // Call the reset function to restart the Arduino
    }
  }
  /*
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been read
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check if the PICC is of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && 
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  // Check if this is a new card
  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3]) {

    Serial.println(F("A new card has been detected."));
    
    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    // Create a string to store the card ID
    cardID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      cardID += String(rfid.uid.uidByte[i], HEX);  // Append each byte to the string
    }

    // Print the card ID in Hex format and store it
    Serial.print(F("Card ID: "));
    Serial.println(cardID);
  } else {
    Serial.println(F("Card read previously."));
  }

  // Halt the PICC (stop communication with the card)
  rfid.PICC_HaltA();

  // Stop encryption on the PCD (reader)
  rfid.PCD_StopCrypto1();*/

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    // Reiniciar cardID
    cardID = "";

    // Leer los bytes del UID y construir la cadena hexadecimal
    for (byte i = 0; i < rfid.uid.size; i++) {
      if (rfid.uid.uidByte[i] < 0x10) {
        cardID += "0";  // Agregar un cero si el byte es menor a 0x10
      }
      cardID += String(rfid.uid.uidByte[i], HEX);
    }

    // Convertir el ID en un valor decimal
    unsigned long cardIDInt = strtoul(cardID.c_str(), NULL, 16);
   // Serial.print("Card ID (Decimal): ");
    Serial.println(cardIDInt);

    // Detener la comunicación con la tarjeta
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}

/**
 * Function to reset the Arduino (microcontroller).
 */
void reset() {
  ESP.restart();
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
