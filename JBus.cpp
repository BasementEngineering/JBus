#include "JBus.h"

/** \brief Constructs a JBus object.
*
*The JBus can work with any pin on an Arduino board, that can be used as a digital output.
*It is furthermore possible to set the response timeout in microseconds, the default value is 6000000 or 6 seconds.
*/
JBus::JBus(uint8_t requestPin, uint8_t dataPin, unsigned long responseTimeout) 
{
  _requestLine = requestPin;
  _dataLine = dataPin;
  _responseTimeout = responseTimeout;
  pinMode(_requestLine, INPUT_PULLUP);
  pinMode(_dataLine, INPUT_PULLUP);
}

/** \brief Checks if the other MCU wants to send a message.
*/
bool JBus::requestAvailable(void) const
{
  return !digitalRead(_requestLine);
}

/** \brief Gets one byte from the requesting.
*
*A boolean variable can be passed to the function and used to indicate the success.
*If the function was unable to read the data it returns 0 and the success variable is set to false.
*/
byte JBus::read(bool &success) const
{
  if (requestAvailable())
  {
  sendReadyMessage();
  return  decodeByte(success);
  }
  else
  {
  success = false;	  
  return 0;
  }
}

/** \brief Gets one byte from the requesting.
*
*Same as read(bool success) without a success indication. 
*/
byte JBus::read(void) const
{
	bool unused;
  return read(unused);
}

/** \brief Sends an answer to the requesting MCU.
*/
void JBus::sendReadyMessage(void) const
{
  pinMode(_dataLine, OUTPUT);
  digitalWrite(_dataLine, LOW);
  delayMicroseconds(200);
  digitalWrite(_dataLine, HIGH);
  pinMode(_dataLine, INPUT_PULLUP);
}

/** \brief Decodes a byte from the data line.
*
*The JBus protocol encodes bytes similar to the NEC format, that is also used in multiple IR-transmission applications.
*1's and 0's are defined by the length of each HIGH pulse. 500 microsecons = 1 / 100 microseconds = 0.
*Between each Bit the data line is LOW for 100 microseconds
*If the decoding was unsuccsessfull the function returns 0 and the success parameter is set to false.
*/
byte JBus::decodeByte(bool &success) const
{
  byte data = 0;

  for (int i = 0; i < 8; i++)
  {
    byte bitmask = (1 << i);

    unsigned long bitLength = pulseIn(_dataLine, HIGH,10000);
    
    if (bitLength > 450 && bitLength < 550) 
	{
      data = data | bitmask;
    }
	else if (bitLength > 80 && bitLength < 120) 
	{
      //leave the bit in it's current state 0
    }
	else
	{
      success = false;
      return 0;
    }
  }
  
  success = true;
  return int(data);
}

/** \brief Sends one byte of data to the other MCU.
*
*To send a Byte the sender first sends a request and waits for a response that indicates, 
that the other MCU is ready to receive one byte.
*It then sends the byte and ends the transmission.
*The function returns false if the operation was unsuccsessfull.
*/
bool JBus::write(byte message) const
{
  sendRequestMessage();
  bool success = waitForAnswer();
  if (success) 
  {
	delayMicroseconds(200);
    sendByte(message);
  }
  endTransmission();
  return success;
}

/** \brief Sends a sending request to the receiver by pulling the request line low.
*/
void JBus::sendRequestMessage(void) const
{
  pinMode(_requestLine, OUTPUT);
  digitalWrite(_requestLine, LOW);
}

/** \brief Waits for the receiving MCU to send an pull the data line LOW.

Returns false if no answer was received within the response timeout.
*/
bool JBus::waitForAnswer(void) const
{
return pulseIn(_dataLine, LOW, _responseTimeout);
}

/** \brief Sends one byte over the data line.
*
*The JBus protocol encodes bytes similar to the NEC format, that is also used in multiple IR-transmission applications.
*1's and 0's are defined by the length of each HIGH pulse. 500 microsecons = 1 / 100 microseconds = 0.
*Between each Bit the data line is LOW for 100 microseconds
*If the sending was unsuccsessfull the function returns false.
*/
void JBus::sendByte(byte data) const 
{
#ifdef DEBUG
  Serial.println("Sending NEC Code");
#endif

  pinMode(_dataLine, OUTPUT);
  digitalWrite(_dataLine, LOW);
  
  //sending the data Byte
  delayMicroseconds(100);
  for (int i = 0; i < 8; i++) 
  {
    bool bit = data % 2;
    data = data / 2;

    digitalWrite(_dataLine, HIGH);
    if (bit) 
	{
      delayMicroseconds(500);
    }
    else 
	{
      delayMicroseconds(100);
    }
    digitalWrite(_dataLine, LOW);
    delayMicroseconds(100);
  }
  digitalWrite(_dataLine, HIGH);
  pinMode(_dataLine, INPUT_PULLUP);

#ifdef DEBUG
  Serial.println("finished transmission");
#endif
}

/** \brief Ends the transmission by pulling the request line back up to HIGH.
*/
void JBus::endTransmission(void) const
{
  digitalWrite(_requestLine, HIGH);
  pinMode(_requestLine, INPUT_PULLUP);
}

