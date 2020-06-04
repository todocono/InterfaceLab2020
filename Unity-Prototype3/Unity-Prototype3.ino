
/* Inputs and outputs with Unity
  Interface Lab 2020 Summer


  Using code examples and libraries from:

  Servo http://www.arduino.cc/en/Tutorial/Sweep from BARRAGAN <http://barraganstudio.com>
  LSM6DS3 IMU sensor by Riccardo Rizzo https://www.arduino.cc/en/Reference/ArduinoLSM6DS3
  Fusion Library https://github.com/aster94/SensorFusion  from
  Madgwick algorithm https://github.com/PaulStoffregen/MadgwickAHRS
  Serial Communication Multiple Values by Jiwon at NYU SH Interaction Lab https://wp.nyu.edu/shanghai-ima-interaction-lab/

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
const int NUM_OF_VALUES = 3;    /** YOU MUST CHANGE THIS ACCORDING TO YOUR PROJECT **/


/** DO NOT REMOVE THESE **/
float gx, gy, gz, ax, ay, az, mx, my, mz;
float pitch, roll, yaw;
float deltat;
int tempValue = 0;
int valueIndex = 0;
int values[NUM_OF_VALUES];

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

  getSerialData();

  analogWrite(ledPin, values[0]);
  myservo.write(values[1]);
  digitalWrite(LED_BUILTIN, values[2]);
}


//recieve serial data from Unity
void getSerialData() {
  if (Serial.available()) {
    char c = Serial.read();
    //switch - case checks the value of the variable in the switch function
    //in this case, the char c, then runs one of the cases that fit the value of the variable
    //for more information, visit the reference page: https://www.arduino.cc/en/Reference/SwitchCase
    switch (c) {
      //if the char c from Processing is a number between 0 and 9
      case '0'...'9':
        //save the value of char c to tempValue
        //but simultaneously rearrange the existing values saved in tempValue
        //for the digits received through char c to remain coherent
        //if this does not make sense and would like to know more, send an email to me!
        tempValue = tempValue * 10 + c - '0';
        break;
      //if the char c from Processing is a comma
      //indicating that the following values of char c is for the next element in the values array
      case ',':
        values[valueIndex] = tempValue;
        //reset tempValue value
        tempValue = 0;
        //increment valuesIndex by 1
        valueIndex++;
        break;
      //if the char c from Processing is character 'n'
      //which signals that it is the end of data
      case '\n':
        //save the tempValue
        //this will b the last element in the values array
        values[valueIndex] = tempValue;
        //reset tempValue and valueIndex values
        //to clear out the values array for the next round of readings from Processing
        tempValue = 0;
        valueIndex = 0;
        break;

    }
  }
}
