#include "WifiHandler.h"

#include "arduino_secrets.h"
#include <WiFiS3.h>
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS; 

void WifiHandler::connectToWifi() {
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    matrix.loadSequence(LEDMATRIX_ANIMATION_WIFI_SEARCH);
    matrix.begin();
    matrix.play(true);
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.print("You're connected to the network");
  matrix.loadFrame(LEDMATRIX_CLOUD_WIFI);
  printCurrentNet();
  printWifiData();
}

void WifiHandler::printWifiData() {
  // print your board's IP address:
  Serial.print("Arduino Wifi Data: ");
  Serial.print("\n");
  IPAddress ip = WiFi.localIP();
  Serial.print("    IP Address: ");
  
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("    MAC address: ");
  printMacAddress(mac);
}

void WifiHandler::printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void WifiHandler::printMacAddress(byte mac[]) {
  for (int i = 0; i < 6; i++) {
    if (i > 0) {
      Serial.print(":");
    }
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
  }
  Serial.println();
}



