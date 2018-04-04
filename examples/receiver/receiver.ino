/*The JBus is a very simple bidirectional two wire *communication protocol that can be used to transfer 
*one bit at a time between two Arduino MCU's. 
*What makes the JBus special is that it does not rely on interrupts.
*This example allows you to test a JBus connection between two Arduinos.
*
*To set the connection up you simply connect the request pins on both arduinos together 
*and do the same thing for the data pins.
*You then hook both Arduinos up to the same Ground.
*Flash the receiver example onto the receiving board and the sender sketch onto the other one.
*
*After you power up both boards you should see the LED's flashing. 
*The senders LED flashes two times for every flash of the receiver.
*
*If you have two USB cables you can hook both boards up to your PC simultaniously. 
*Open two instances of the Arduino IDE, which allows you to start one Serial Monitor for each boards port. 
*Now you can watch both boards communicating.
*author: Jan Kettler aka Basement Engineering
*last updated: 04.04.2018
*/

#include "JBus.h"

const uint8_t REQUEST_PIN = 2;
const uint8_t DATA_PIN = 3;

JBus jbus(REQUEST_PIN, DATA_PIN);
  
void setup() {
	pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);
  Serial.println("JBus receiver is ready");
  Serial.println();
}

void flashLed(uint8_t numberOfTimes)
{
for(int i = 0; i < numberOfTimes; i++)
{
	digitalWrite(LED_BUILTIN,HIGH);
	delay(300);
	digitalWrite(LED_BUILTIN,LOW);
	delay(300);
}
}

void loop() {
 if(jbus.requestAvailable())
 {
  Serial.println("A request is available");
  bool successfull;
  uint8_t numberOfFlashes = jbus.read(successfull);

  if(successfull)
  {
  Serial.print("Successfully received: ");
  Serial.println(numberOfFlashes);
  flashLed(numberOfFlashes);
  }
  else
  {
    Serial.println("An error occoured");
  }
 }
}
