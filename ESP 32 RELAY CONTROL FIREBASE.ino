#include "FirebaseESP32.h"
#include <WiFi.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

const char *ssid = "USERNAME WIFI";
const char *password = "PASSWORD WIFI ";

FirebaseData firebaseData;
FirebaseJson json;

#define relay 16
#define LED 23

void setup()
{
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(relay, LOW); // nilai awal relay akan off

  konekWifi();
  Firebase.begin("LINK REALTIME DATABASE", "SECRET KODE DATABASE FIREBASE");
  // Firebase.begin("firebase host", "firebase auth database");
}

void konekWifi()
{
  WiFi.begin(ssid, password);
  // memulai menghubungkan ke wifi router
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("."); // status saat mengkoneksikan
  }
  Serial.println("Sukses terkoneksi wifi!");
  Serial.println("IP Address:"); // alamat ip lokal
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (Firebase.getString(firebaseData, "/RelayControl/nilai"))
  { // misal database diberikan nama relay1
    if (firebaseData.dataType() == "string")
    {
      String FBStatus = firebaseData.stringData();
      if (FBStatus == "1")
      {
        Serial.println("Relay ON");
        Serial.print("Pompa ON");
        digitalWrite(relay, HIGH);
        digitalWrite(LED, HIGH);
        Firebase.RTDB.setString(&firebaseData, "/RelayControl/status", "Pompa_Hidup");
      }
      else if (FBStatus == "0")
      {
        Serial.println("Relay OFF");
        Serial.print("Pompa OFF");
        digitalWrite(relay, LOW);
        digitalWrite(LED, LOW);
        Firebase.RTDB.setString(&firebaseData, "/RelayControl/status", "Pompa_Mati");
      }
      else
      {
        Serial.println("Salah kode! isi dengan data ON/OFF");
      }
    }
  }
}
