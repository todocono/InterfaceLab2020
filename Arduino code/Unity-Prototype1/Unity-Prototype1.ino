
/* 
  Interface Lab 2020 Summer
  Inputs from Arduino (IMU)

  Arduino periodically reports the new status of the pitch, roll and yaw to Unity.
  To run this code, you will need to install the Fusion Library first.
  
  Using code examples and libraries from:

  LSM6DS3 IMU sensor by Riccardo Rizzo https://www.arduino.cc/en/Reference/ArduinoLSM6DS3
  Fusion Library https://github.com/aster94/SensorFusion 
  Madgwick algorithm https://github.com/PaulStoffregen/MadgwickAHRS

  This example code is in the public domain.

  modified 11 June 2020
  by Rodolfo Cossovich
*/

#include "SensorFusion.h" //SF
SF fusion;

float gx, gy, gz, ax, ay, az, mx, my, mz;
float pitch, roll, yaw;
float deltat;

#include <Arduino_LSM6DS3.h>


void setup() {

  Serial.begin(115200); //serial to display data
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

  // now you should read the gyroscope, accelerometer (and magnetometer if you have it also)
  // NOTE: the gyroscope data have to be in radians
  // if you have them in degree convert them with: DEG_TO_RAD example: gx * DEG_TO_RAD


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
}
