# JBus

##Work in Progress!

##What is it?
The JBus protocol is a 2 wire Bus protocol for Arduino that is not time critical. 
Meaning that ist does **not depend on any Interupts**.

##Why did I come up with this? or my use case
I hacked it together for a project that includes adressable RGB LED's and an IR Receiver.

##The problem
As the Adafruit library disables all interupts, it is not possible to receive IR Signals while the LED's are updating.
This is not a problem if i only change the color ones, but if you are running dynamic effects, 
that update every 20ms you will not be able to process any IR signals.

So i moved the IR Receiver part over to a second microcontroller, 
but now i needed to get the received input over to the MCU, that controls the LED's.
As interupts were disabled, I could not use I2C, so i came up with the JBus.
