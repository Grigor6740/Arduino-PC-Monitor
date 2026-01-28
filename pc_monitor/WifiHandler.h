#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H
#include <Arduino.h>

class WifiHandler {
  public: 
    void connectToWifi();
    void printWifiData();
    void printCurrentNet();
    void printMacAddress(byte mac[]);
  private:
};

#endif