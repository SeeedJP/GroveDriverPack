// BOARD Seeed Wio
// GROVE I2C <-> Grove - 6-Axis Accelerometer&Gyroscope (SKU#105020012)

#include <GroveDriverPack.h>

#define INTERVAL    (100)

WioCellular Wio;

GroveBoard Board;
GroveAccelerometerLSM6DS3 LSM6DS3(&Board.I2C);

void setup() {
  delay(200);
  SerialUSB.begin(115200);

  Wio.Init();
  Wio.PowerSupplyGrove(true);
  delay(500);

  Board.I2C.Enable();
  if (!LSM6DS3.Init())
  {
    SerialUSB.println("Sensor not found.");
  }
  else
  {
    LSM6DS3.Setup(LSM6DS3.MODE_BASIC_MEASURE);
    SerialUSB.println("Sensor ready.");
  }
}

void loop() {
  char log[80];
  char strAX[10], strAY[10], strAZ[10];
  char strGX[10], strGY[10], strGZ[10];

  LSM6DS3.ReadAccel();
  dtostrf(LSM6DS3.accelX, 4, 2, strAX);
  dtostrf(LSM6DS3.accelY, 4, 2, strAY);
  dtostrf(LSM6DS3.accelZ, 4, 2, strAZ);

  LSM6DS3.ReadGyro();
  dtostrf(LSM6DS3.gyroX, 4, 2, strGX);
  dtostrf(LSM6DS3.gyroY, 4, 2, strGY);
  dtostrf(LSM6DS3.gyroZ, 4, 2, strGZ);

  sprintf(log, "Accel: %6s, %6s, %6s    Gyro: %8s, %8s, %8s",
    strAX, strAY, strAZ, strGX, strGY, strGZ);
  SerialUSB.println(log);

  delay(INTERVAL);
}
