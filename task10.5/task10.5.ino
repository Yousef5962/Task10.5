#include <Wire.h>

const int MPU6050 = 0x68;     // I2C address of the MPU6050
const int GYRO_ZOUT = 0x47;   // Register for the high byte of the Z-axis gyroscope data
const int PWR_MGMT_1 = 0x6B;  // Power management register
float yaw = 0.0;
unsigned long lastTime = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  Wire.beginTransmission(MPU6050);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission(true);

  lastTime = millis();
}

void loop() {
  unsigned long currentTime = millis();               // Get the current time
  float deltaTime = (currentTime - lastTime) / 1000.0; // Calculate the time difference in seconds
  lastTime = currentTime;
  int16_t gyroZRaw = readGyroZ();                     // Read the raw gyroscope data for the Z-axis
  float gyroZ = gyroZRaw / 131.0;
  yaw += gyroZ * deltaTime;

  Serial.print("Yaw: ");
  Serial.println(yaw);
  delay(100);
}

int16_t readGyroZ() {
  Wire.beginTransmission(MPU6050);
  Wire.write(GYRO_ZOUT);                              // Request the high byte of the Z-axis gyroscope data
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050, 2, true);                 // Request 2 bytes of data from the MPU6050
  int16_t gyroZ = Wire.read() << 8 | Wire.read();     // Combine the high and low bytes to get the full 16-bit gyroscope data
  return gyroZ;
}
