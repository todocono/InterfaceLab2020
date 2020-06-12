
/*
  Interface Lab 2020 Summer
  Inputs from Arduino are still working (IMU)
  Outputs from Unity come in asynchronous mode and they don't interrupt Arduino, they just change it's behavior

  Using code examples and libraries from:

  Servo http://www.arduino.cc/en/Tutorial/Sweep from BARRAGAN <http://barraganstudio.com>
  LSM6DS3 IMU sensor by Riccardo Rizzo https://www.arduino.cc/en/Reference/ArduinoLSM6DS3
  Fusion Library https://github.com/aster94/SensorFusion  from
  Madgwick algorithm https://github.com/PaulStoffregen/MadgwickAHRS

  This example code is in the public domain.

  modified 4 June 2020
  by Rodolfo Cossovich
*/

#include <Servo.h>
#include <Arduino_LSM6DS3.h>
#include "SensorFusion.h" //SF


Servo myservo;  // create servo object to control a servo
SF fusion;

const int ledPin = A2;    // LED connected to digital pin A2
const int servoPin = 12;    // LED connected to digital pin 12

float gx, gy, gz, ax, ay, az, mx, my, mz;
float pitch, roll, yaw;
float deltat;

boolean fading = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object

  Serial.begin(9600); //serial to display data
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");
}

void loop() {

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);

    deltat = fusion.deltatUpdate(); //this have to be done before calling the fusion update
    //choose only one of these two:
    //fusion.MahonyUpdate(gx*DEG_TO_RAD, gy*DEG_TO_RAD, gz*DEG_TO_RAD, ax*DEG_TO_RAD, ay*DEG_TO_RAD, az*DEG_TO_RAD, deltat);  //mahony is suggested if there isn't the mag and the mcu is slow
    fusion.MadgwickUpdate(gx * DEG_TO_RAD, gy * DEG_TO_RAD, gz * DEG_TO_RAD, ax * DEG_TO_RAD, ay * DEG_TO_RAD, az * DEG_TO_RAD, mx, my, mz, deltat); //else use the magwick, it is slower but more accurate

    pitch = fusion.getPitch();
    roll = fusion.getRoll();    //you could also use getRollRadians() ecc
    yaw = fusion.getYaw();

    Serial.print(pitch);
    Serial.print(","); Serial.print(roll);
    Serial.print(","); Serial.print(yaw);
    Serial.println();
  }
  // Change physical outputs when I receive 'A' or a 'Z'.
  switch (Serial.read())
  {
    case 'A':
      //Toggling BUILTIN LED on pin 13
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      break;
    case 'B':
      //Turning ON BUILTIN LED on pin 13
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    case 'C':
      //Turning OFF BUILTIN LED on pin 13
      digitalWrite(LED_BUILTIN, LOW);
      break;
    case 'D':
      //Toggling external LED on Pin
      digitalWrite(ledPin, !digitalRead(ledPin));
      break;
    case 'E':
      //Turning ON external LED on Pin
      digitalWrite(ledPin, HIGH);
      break;
    case 'F':
      //Turning OFF external LED on Pin
      digitalWrite(ledPin, LOW);
      break;
    case 'G':
      //Toggling fading on external LED on Pin
      fading = !fading;
      break;
    case 'Y':
      //That's the last letter of the abecedarium
      moveServo(0);
      break;
    case 'Z':
      //That's the last letter of the abecedarium
      moveServo(90);
      break;
  }

  if (fading) {
    fade(ledPin, 1000 );
  }

}


void moveServo(int pos) {
  myservo.write(pos);              // tell servo to go to position in variable 'pos'
}

void fade(int pin, int interval) {
  int fadeValue = millis() % (interval);
  (fadeValue > interval / 2) ? fadeValue = 255 - int((float(fadeValue - interval / 2) / (interval / 2)) * 255) : fadeValue = int((float(fadeValue) / (interval / 2)) * 255);
  analogWrite(ledPin, fadeValue);
}
