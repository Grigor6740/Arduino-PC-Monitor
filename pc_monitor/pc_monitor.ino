#include <string.h>
#include "WifiHandler.h"
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include "WiFiS3.h"
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix2;

WifiHandler wifi;

LiquidCrystal_I2C lcd(0x27, 20, 4);

WiFiServer server(8080);

const uint32_t blankFrame[] = {
  0x00000000,
  0x00000000,
  0x00000000
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  matrix2.begin();

  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.clear();

  wifi.connectToWifi();
  
  server.begin(); 
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);
  wifi.printCurrentNet();

  WiFiClient client = server.available(); 

  if(client) {
    Serial.println("Client connected"); 

    while(client.connected()) {
      if(client.available()) {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, client);

        if(error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.c_str());
          return;
        }

        //CPU
        double cpu_usage = doc["cpu"]["cpu_usage"];
        double cpu_frequency = doc["cpu"]["cpu_frequency"];

        //RAM 
        double ram_percent_usage = doc["ram"]["percent"];
        double ram_used = doc["ram"]["used"];
        double ram_total = doc["ram"]["total"];    

        //Disk
        double disk_percent_usage = doc["disk"]["percent"];
        double disk_used = doc["disk"]["used"];
        double disk_total = doc["disk"]["total"];

        //Battery
        double battery_percent = doc["battery"]["percent"];

        //Showing the data on the lcd display
        lcd.setCursor(0, 0);
        lcd.print((String)"C:" + cpu_usage + "%|" + cpu_frequency + "Mhz");

        lcd.setCursor(0, 1);
        lcd.print((String)"R:" + ram_percent_usage + "%|" + ram_used + "/" + ram_total);

        lcd.setCursor(0, 2);
        lcd.print((String)"B:" + battery_percent + "%|" + "D:" + disk_percent_usage + "%");

        lcd.setCursor(0, 3);
        lcd.print((String)disk_used + "/" + disk_total);

        if (ram_percent_usage > 80.00 || disk_percent_usage > 80.00 || cpu_usage > 80.00) {
          
          matrix2.loadFrame(LEDMATRIX_EMOJI_SAD);
          delay(10000);
          matrix2.loadFrame(blankFrame);
        } 
      }
    }
    
    client.stop();
    Serial.println("Client disconnected");
  }
  else {
    lcd.clear();
  }
}


