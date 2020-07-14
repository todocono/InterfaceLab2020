/*
 Prototype 0 - Using a button to send a single byte from Arduino to Unity

 Reads a digital input on pin 2, prints the result to Serial
 On my mac, it uses /dev/cu.usbmodem1451301
 The main modification from DigitalReadSerial from http://www.arduino.cc/en/Tutorial/DigitalReadSerial 
 is the delay for 100ms

 This example code is in the public domain and based on 
*/

int pushButton = 2; //naming the pin will help the code to be more portable

void setup() {
 Serial.begin(9600);  // we initiate the serial communication
 pinMode(pushButton, INPUT); // we declare that the button will be used as an input
}

void loop() {
 bool buttonState = digitalRead(pushButton); // we capture whether the button is pressed
 Serial.println(buttonState); 
 delay(100);  // IMPORTANT ---> delay in between reads for this example
}
