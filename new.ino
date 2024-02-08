#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <SPI.h>

#define RST_PIN         22
#define SS_PIN          5
#define inputPin        2
#define Sensor_Pin      2
#define lightPin 4

// Firebase credentials
#define FIREBASE_HOST "https://studyroommanagement-e7b39-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyAQlSsONu-pGX01C8FtdI4Beiro3id4n58"

// Wi-Fi credentials
const char* ssid = "OPPOF17";
const char* password ="Onaliy12334";

MFRC522 mfrc522(SS_PIN, RST_PIN);
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial port to connect. Needed for native USB port only

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  config.host = FIREBASE_HOST;
  config.api_key = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println("Hold your Student ID Card near to the Scanner..");
  Serial.println();

  pinMode(inputPin, INPUT);
  pinMode(Sensor_Pin, INPUT);
  pinMode(lightPin, OUTPUT);
  Serial.println("\n\nLet's Begin\n");
}

void loop() {
  // Handle Firebase events if needed
  // Firebase.process(); '

  // Motion sensor
  bool motion = digitalRead(inputPin);
  if(motion)
  {
    Serial.println("Motion detected: " +String(motion));
    digitalWrite(lightPin,LOW);
  }
  else{
    Serial.println("Motion not detected: " +String(motion));
    digitalWrite(lightPin,HIGH);
  }
  

  bool Sensor_State = digitalRead(Sensor_Pin);
  int Senor_Value = analogRead(A0);
  Serial.println("\nSensor_State: " + String(Sensor_State));
  // Serial.println("\nSensor_Value: " + String(Sensor_Value));

  if (Sensor_State == true) {
    Serial.println("Sound Detected");
  } else {
    Serial.println("Sound Not Detected");
  }

  delay(3000);

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

 Serial.print("Card ID: ");
    String cardID = ""; //store the card's ID
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        cardID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        cardID.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    cardID.toUpperCase();
    if (cardID.substring(1) == "73 33 84 A9") {
        Serial.println("Access Granted");
        Serial.println();
       
       
    } else {
        Serial.println("Access Denied");
        
        
    }
     delay(1500);
}


