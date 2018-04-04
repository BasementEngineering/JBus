/** \brief JBus is a very simple bidirectional two wire *communication protocol that can be used to transfer one bit at a *time between two Arduino MCU's. 
*
*The special thing about it is that ist does not require any interrupts. 
*After sending a request the sending MCU is blocked while waiting for an answer (or for the response timeout to run out). 
This allows the receiving MCU to manage other functions until it has time to listen to the answer.
\author Jan Kettler aka Basement Engineering
*/

#ifndef JBUS_H
#define JBUS_H

#include <Arduino.h>

//#define DEBUG //uncomment this if you want to see whats going on

class JBus{
  public:
  JBus(uint8_t requestPin, uint8_t dataPin, unsigned long responseTimeout = 6000000 );

  bool requestAvailable(void) const;
  bool write(byte message) const;
  byte read(void) const;
  byte read(bool &success) const;
  

  private:
  uint8_t _requestLine;
  uint8_t _dataLine;
  unsigned long _responseTimeout;
  
  void sendReadyMessage(void) const;
  byte decodeByte(bool &success) const;
  
  void sendRequestMessage(void) const;
  bool waitForAnswer(void) const;
  void sendByte(byte message) const;
  void endTransmission(void) const;
};

#endif
