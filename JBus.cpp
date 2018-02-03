#include "JBus.h"

JBus::JBus(int requestPin, int dataPin) {
  requestLine = requestPin;
  dataLine = dataPin;
  pinMode(requestLine, INPUT_PULLUP);
  pinMode(dataLine, INPUT_PULLUP);
}

bool JBus::requestAvailable() {
  return !digitalRead(requestLine);
}

bool JBus::write(byte message) {
  sendRequestMessage();
  bool success = waitForAnswer();
  if (success) {
    sendByte(message);
  }
  endTransmission();
  return success;
}

byte JBus::read() {
  if (requestAvailable());
  sendReadyMessage();
  return  decodeByte();
}

void JBus::sendReadyMessage() {
  pinMode(dataLine, OUTPUT);
  digitalWrite(dataLine, LOW);
  delayMicroseconds(200);
  digitalWrite(dataLine, HIGH);
  pinMode(dataLine, INPUT_PULLUP);
}

byte JBus::decodeByte() {
  byte data = 0;

  for (int i = 0; i < 8; i++){
    byte bitmask = (1 << i);

    unsigned long bitLength = pulseIn(dataLine, HIGH,10000);
    if(bitLength == 0){
      return 0;
    }
    if (bitLength > 450 && bitLength < 550) {
      data = data | bitmask;
    }
  }

  return data;
}

void JBus::sendRequestMessage() {
  pinMode(requestLine, OUTPUT);
  digitalWrite(requestLine, LOW);
}

bool JBus::waitForAnswer() {
  return pulseIn(dataLine, LOW, 100000);
}

void JBus::sendByte(byte data) {
#ifdef DEBUG
  Serial.println("Sending NEC Code");
#endif

  pinMode(dataLine, OUTPUT);
  digitalWrite(dataLine, LOW);
  //sending the data Byte
  delayMicroseconds(100);
  for (int i = 0; i < 8; i++) {
    bool bit = data % 2;
    data = data / 2;

    digitalWrite(dataLine, HIGH);
    if (bit) {
      delayMicroseconds(500);
    }
    else {
      delayMicroseconds(100);
    }
    digitalWrite(dataLine, LOW);
    delayMicroseconds(100);
  }
  pinMode(dataLine, INPUT_PULLUP);

#ifdef DEBUG
  Serial.println("finished transmission");
#endif
}

void JBus::endTransmission() {
  digitalWrite(requestLine, HIGH);
  pinMode(requestLine, INPUT_PULLUP);
}

