#ifndef JBUS_H
#define JBUS_H

#include <Arduino.h>

class JBus{
  public:
  JBus(int requestPin, int dataPin);

  bool requestAvailable();
  bool write(byte message);
  byte read();
  

  private:
  int requestLine;
  int dataLine;
  
  void sendReadyMessage();
  byte decodeByte();
  
  void sendRequestMessage();
  bool waitForAnswer();
  void sendByte(byte message);
  void endTransmission();
};

#endif
